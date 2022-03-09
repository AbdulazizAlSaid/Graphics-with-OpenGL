#include <string.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <stdlib.h>
#include <iostream>
#include <math.h>

#define PI 3.14159

using namespace std;

bool WireFrame= false;
float i =0;
const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };


    float xpos =0;
    float ypos =0;
    float Wwidth,Wheight;

    int mouse_old_x, mouse_old_y;
    int mouse_buttons = 0;
    float RotateX =0;
    float RotateY =10;
    float RotateZ =0;
    float translate_z =2;
    float translate_x = 0;
    float translate_y =0;
    float tmp;

  const C_STRUCT  aiScene *scene =NULL;
  GLuint scene_list =0;
  C_STRUCT aiVector3D scene_min, scene_max, scene_center;

   static float angle =0.f;

   #define aisgl_min(x,y) (x<y?x:y)
   #define aisgl_max(x,y) (y>x?y:x)

 void get_bounding_for_node( const C_STRUCT aiNode* nd,
                            C_STRUCT aiVector3D* min,
                            C_STRUCT aiVector3D* max,
                            C_STRUCT aiMatrix4x4* trafo
                            )
{
    C_STRUCT aiMatrix4x4 prev;
    unsigned  int n=0;
    prev = *trafo;

    aiMultiplyMatrix4(trafo,&nd->mTransformation);

    for(; n<nd->mNumMeshes;n++)
    {
      const C_STRUCT aiMesh* mesh = scene->mMeshes[nd->mMeshes[n]];
		for (int t = 0; t < mesh->mNumVertices; ++t) {

			C_STRUCT aiVector3D tmp = mesh->mVertices[t];
			aiTransformVecByMatrix4(&tmp,trafo);

			min->x = aisgl_min(min->x,tmp.x);
			min->y = aisgl_min(min->y,tmp.y);
			min->z = aisgl_min(min->z,tmp.z);

			max->x = aisgl_max(max->x,tmp.x);
			max->y = aisgl_max(max->y,tmp.y);
			max->z = aisgl_max(max->z,tmp.z);
    } // end for
    } //end for
    for (n = 0; n < nd->mNumChildren; ++n) {
		get_bounding_for_node(nd->mChildren[n],min,max,trafo);
	}
	*trafo = prev;
}


/* ---------------------------------------------------------------------------- */
void get_bounding_box(C_STRUCT aiVector3D* min, C_STRUCT aiVector3D* max)
{
	C_STRUCT aiMatrix4x4 trafo;
	aiIdentityMatrix4(&trafo);

	min->x = min->y = min->z =  1e10f;
	max->x = max->y = max->z = -1e10f;
	get_bounding_for_node(scene->mRootNode,min,max,&trafo);
}

/* ---------------------------------------------------------------------------- */
void color4_to_float4(const C_STRUCT aiColor4D *c, float f[4])
{
	f[0] = c->r;
	f[1] = c->g;
	f[2] = c->b;
	f[3] = c->a;
}

/* ---------------------------------------------------------------------------- */
void set_float4(float f[4], float a, float b, float c, float d)
{
	f[0] = a;
	f[1] = b;
	f[2] = c;
	f[3] = d;
}

/* ---------------------------------------------------------------------------- */
int loadasset (const char* path)
{
	/* we are taking one of the postprocessing presets to avoid
	   spelling out 20+ single postprocessing flags here. */
	scene = aiImportFile(path,aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene) {
		get_bounding_box(&scene_min,&scene_max);
		scene_center.x = (scene_min.x + scene_max.x) / 2.0f;
		scene_center.y = (scene_min.y + scene_max.y) / 2.0f;
		scene_center.z = (scene_min.z + scene_max.z) / 2.0f;
		return 0;
	}
	return 1;
}
/* ---------------------------------------------------------------------------- */
void apply_material(const C_STRUCT aiMaterial *mtl)
{
	float c[4];

	GLenum fill_mode;
	int ret1, ret2;
	C_STRUCT aiColor4D diffuse;
	C_STRUCT aiColor4D specular;
	C_STRUCT aiColor4D ambient;
	C_STRUCT aiColor4D emission;
	ai_real shininess, strength;
	int two_sided;
	int wireframe;
	unsigned int max;

	set_float4(c, 0.8f, 0.8f, 0.8f, 1.0f);
	if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_DIFFUSE, &diffuse))
		color4_to_float4(&diffuse, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, c);

	set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
	if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_SPECULAR, &specular))
		color4_to_float4(&specular, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c);

	set_float4(c, 0.2f, 0.2f, 0.2f, 1.0f);
	if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_AMBIENT, &ambient))
		color4_to_float4(&ambient, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, c);

	set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
	if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_EMISSIVE, &emission))
		color4_to_float4(&emission, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, c);

	max = 1;
	ret1 = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS, &shininess, &max);
	if(ret1 == AI_SUCCESS) {
    	max = 1;
    	ret2 = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS_STRENGTH, &strength, &max);
		if(ret2 == AI_SUCCESS)
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess * strength);
        else
        	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
    }
	else {
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0f);
		set_float4(c, 0.0f, 0.0f, 0.0f, 0.0f);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c);
	}

	max = 1;
	if(AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_ENABLE_WIREFRAME, &wireframe, &max))
		fill_mode = wireframe ? GL_LINE : GL_FILL;
	else
		fill_mode = GL_FILL;
    //    glPolygonMode(GL_FRONT_AND_BACK, fill_mode);

	max = 1;
	if((AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_TWOSIDED, &two_sided, &max)) && two_sided)
		glDisable(GL_CULL_FACE);
	else
		glEnable(GL_CULL_FACE);
}

void recursive_render (const C_STRUCT aiScene *sc, const C_STRUCT aiNode* nd,bool WireFrame)
{
	unsigned int i;
	unsigned int n = 0, t;
	C_STRUCT aiMatrix4x4 m = nd->mTransformation;

	/* update transform */
	aiTransposeMatrix4(&m);
	glPushMatrix();
	glMultMatrixf((float*)&m);



	/* draw all meshes assigned to this node */
	for (; n < nd->mNumMeshes; ++n) {
		const C_STRUCT aiMesh* mesh = scene->mMeshes[nd->mMeshes[n]];

		apply_material(sc->mMaterials[mesh->mMaterialIndex]);

		if(mesh->mNormals == NULL) {
			glDisable(GL_LIGHTING);
		} else {
			glEnable(GL_LIGHTING);
		}

		for (t = 0; t < mesh->mNumFaces; ++t) {
			const C_STRUCT aiFace* face = &mesh->mFaces[t];
			GLenum face_mode;

			switch(face->mNumIndices) {
				case 1: face_mode = GL_POINTS; break;
				case 2: face_mode = GL_LINES; break;
				case 3: face_mode = GL_TRIANGLES; break;
				default: face_mode = GL_POLYGON; break;
			}

			glBegin(face_mode);

			for(i = 0; i < face->mNumIndices; i++) {
				int index = face->mIndices[i];
				if(mesh->mColors[0] != NULL)
					glColor4fv((GLfloat*)&mesh->mColors[0][index]);
				if(mesh->mNormals != NULL)
					glNormal3fv(&mesh->mNormals[index].x);
				glVertex3fv(&mesh->mVertices[index].x);
			}
			glEnd();
		}
	}

	/* draw all children */
	for (n = 0; n < nd->mNumChildren; ++n) {
		recursive_render(sc, nd->mChildren[n],WireFrame);
	}
	glPopMatrix();
}

/* ---------------------------------------------------------------------------- */

static void resize(int width, int height)
{
     double Ratio;

     Wwidth = (float)width;
     Wheight = (float)height;

     Ratio= (double)width /(double)height;

    glViewport(0,0,(GLsizei) width,(GLsizei) height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

	gluPerspective (45.0f,Ratio,0.1f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
 }

static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(0,0,10,0.0,0.0,0.0,0.0,1.0,100.0);

     glTranslatef(translate_x,translate_y,translate_z);

       glRotatef(RotateX,1.f,0.f,0.f);
       glRotatef(RotateY,0.f,1.f,0.f);
       glRotatef(RotateZ,0.f,0.f,1.f);

       glScalef(tmp, tmp, tmp);

    if(WireFrame)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);		//Draw Our Mesh In Wireframe Mesh
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);		//Toggle WIRE FRAME


		if(scene_list == 0) {

	    scene_list = glGenLists(1);
	    glNewList(scene_list, GL_COMPILE);
            /* now begin at the root node of the imported data and traverse
               the scenegraph by multiplying subsequent local transforms
               together on GL's matrix stack. */

	    recursive_render(scene, scene->mRootNode,WireFrame);
	    glEndList();
	}

	glCallList(scene_list);



    // your code here
    glutSwapBuffers();
}


static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27 :
        case 'q':
            exit(0);
            break;

	  case 'w':
		WireFrame =!WireFrame;
	       break;

	  case'1':

        translate_z += (float) 1.0f;
		break;

       case '2':
	    translate_z -= (float) 1.0f;
		break;
    }
  glutPostRedisplay();
}

void Specialkeys(int key, int x, int y)
{
    switch(key)
    {
     case GLUT_KEY_END:
	  translate_z += (float) 1.0f;
		break;
	case GLUT_KEY_HOME:
	  translate_z -= (float) 1.0f;
		break;
    case GLUT_KEY_UP:					/* Clockwise rotation over X */
	 	RotateX = ((int)RotateX +2)%360;
		break;
	case GLUT_KEY_DOWN:					/* Counter Clockwise rotation over X */
		RotateX = ((int)RotateX -2)%360;
		  break;
	case GLUT_KEY_LEFT:					/* Clockwise rotation over Y */
    	RotateY =((int)RotateY +2)%360;
		break;
	case GLUT_KEY_RIGHT:
		RotateY = ((int)RotateY -2)%360;/* Counter Clockwise rotation over Y */
		 break;
   }
  glutPostRedisplay();
}



static void idle(void)
{
    // Use parametric equation with t increment for xpos and y pos
    // Don't need a loop
    glutPostRedisplay();
}



void mouse(int btn, int state, int x, int y){

    float scale = 100*(Wwidth/Wheight);

     if(state==GLUT_DOWN){
                 mouse_buttons |= 1<<btn;
            }
         else if (state == GLUT_UP)
        {
        mouse_buttons = 0;
        }

     switch(btn){


        case GLUT_LEFT_BUTTON:

          mouse_buttons = 2;
        break;

        case GLUT_MIDDLE_BUTTON:  break;

        case GLUT_RIGHT_BUTTON:

          mouse_buttons = 4;
        break;

    }
    mouse_old_x = x;
    mouse_old_y = y;

    glutPostRedisplay();
}

void motion(int x, int y)
{
    float dx, dy;
    dx = (float)(x - mouse_old_x);
    dy = (float)(y - mouse_old_y);

    if (mouse_buttons == 2)
    {
        RotateX += -dy * 0.2f;
        RotateY += dx * 0.2f;
    }
    else if (mouse_buttons ==4)
    {
        translate_y += -dy * 0.02f;
		translate_x += dx*0.02f;
    }

    mouse_old_x = x;
    mouse_old_y = y;
    glutPostRedisplay();
}


static void init(void)
{
     glClearColor(0.5f, 0.5f, 1.0f, 0.0f);                 // assign a color you like

    C_STRUCT aiLogStream stream;

    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);

    glEnable(GL_DEPTH_TEST);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glShadeModel(GL_SMOOTH);

    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);



    /* get a handle to the predefined STDOUT log stream and attach
	   it to the logging system. It remains active for all further
	   calls to aiImportFile(Ex) and aiApplyPostProcessing. */
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_STDOUT,NULL);
	aiAttachLogStream(&stream);

	/* ... same procedure, but this stream now writes the
	   log messages to assimp_log.txt */
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_FILE,"assimp_log.txt");
	aiAttachLogStream(&stream);

	loadasset( "models/ven.obj");

	/* scale the whole asset to fit into our view frustum */
	tmp = scene_max.x-scene_min.x;
	tmp = aisgl_max(scene_max.y - scene_min.y,tmp);
	tmp = aisgl_max(scene_max.z - scene_min.z,tmp);
	tmp = 1.f / tmp;

	if(getenv("MODEL_IS_BROKEN"))
		glFrontFace(GL_CW);

	//glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);


}

/* Program entry point */

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);

    glutInitWindowSize(800,600);
    glutInitWindowPosition(0,0);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("Project Assignment 2");
    init();
    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutKeyboardFunc(key);
    glutSpecialFunc(Specialkeys);
    glutMotionFunc(motion);
    glutIdleFunc(idle);

    glutMainLoop();

    return EXIT_SUCCESS;
}
