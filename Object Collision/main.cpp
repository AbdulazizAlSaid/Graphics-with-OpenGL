#include <string.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <GL/SOIL.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include<time.h>


#define PI 3.14159
#define nBalls 3

using namespace std;
GLUquadricObj* sphere = NULL;
typedef struct{
float x;
float y;
float z;
}vec3;

typedef struct{
vec3 position;
vec3 direction;
float radius;
GLuint tex;

}balls;

bool WireFrame= false;
bool Active = false;
float i =0;
const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 0.2f, 0.2f, 0.2f, 1.0f };
const GLfloat light_diffuseRed[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 5.0f, 5.0f, 5.0f, 1.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };

    float start, t;
    float xpos =0;
    float ypos =0;
    float Wwidth,Wheight;
    float spin;
    float temp, temp2;
    float position[] = {10.0,10.0,10.0,1};//Light position



    float xt,yt,zt,vx,vy,vz,upx,upy,upz,eyex,eyey,eyez;


    //float position[]={500.0,0.0,0.0,1.0};
    float rotX = 0;
    float rotY =0;

    GLuint tex[6];//texture array
    balls myBalls[3];//ball array
    vec3 roomScale;//scale values for skybox




/* GLUT callback Handlers */
/*int isCollide(){

    if(ballPosx > 4){return 1;}
    if(ballPosx < -4){return 2;}
    if(ballPosy > 4){return 3;}
    if(ballPosy < -4){return 4;}
    return -1;

}*/
void loadTexture(char *fileName, GLuint tex){ //load texture
    unsigned char *image;
    int  width, height;


    //glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);


    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    image = SOIL_load_image(fileName, &width, &height, 0, SOIL_LOAD_RGBA);
    if(!image)cout << "File Not Found!";

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    SOIL_free_image_data(image);

    glEnable(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


}

static void resize(int width, int height)
{
     double Ratio;

     Wwidth = (float)width;
     Wheight = (float)height;

     Ratio= (double)width /(double)height;

    glViewport(0,0,(GLsizei) width,(GLsizei) height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

	gluPerspective (60.0f,Ratio,0.1f, 1500.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

 }

static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(eyex,eyey,eyez,vx,vy,vz,upx,upy,upz);

    if(WireFrame)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);		//Draw Our Mesh In Wireframe Mesh
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);		//Toggle WIRE FRAME

    // your code here


    glRotatef(rotX, 1, 0, 0);
    glRotatef(rotY, 0, 1, 0);

    glPushMatrix(); //Skybox matrix
        glEnable(GL_TEXTURE_2D);
        // Front face
        glScalef(roomScale.x,roomScale.y,roomScale.z);
        glBindTexture(GL_TEXTURE_2D, tex[0]);
        glBegin(GL_QUADS);
            glNormal3f(0.0f,0.0f,-1.0f);

            glTexCoord2f(0.0,0.0);
            glVertex3f(-1.0f, 1.0f, 1.0f);

            glTexCoord2f(1.0,0.0);
            glVertex3f(1.0f, 1.0f, 1.0f);

            glTexCoord2f(1.0,1.0);
            glVertex3f(1.0f, -1.0f, 1.0f);

            glTexCoord2f(0.0,1.0);
            glVertex3f(-1.0f, -1.0f, 1.0f);

        glEnd();

        //Back face
        glBindTexture(GL_TEXTURE_2D, tex[0]);
        glBegin(GL_QUADS);
            glNormal3f(0.0f,0.0f,-1.0f);

            glTexCoord2f(1.0,0.0);
            glVertex3f(-1.0f, 1.0f, -1.0f);

            glTexCoord2f(1.0,1.0);
            glVertex3f(-1.0f, -1.0f, -1.0f);

            glTexCoord2f(0.0,1.0);
            glVertex3f(1.0f, -1.0f, -1.0f);

            glTexCoord2f(0.0,0.0);
            glVertex3f(1.0f, 1.0f, -1.0f);

        glEnd();

        //Top face
        glBindTexture(GL_TEXTURE_2D, tex[1]);
        glBegin(GL_QUADS);
            glNormal3f(0.0f,-1.0f,0.0f);

            glTexCoord2f(0.0,1.0);
            glVertex3f(1.0f, 1.0f, -1.0f);

            glTexCoord2f(0.0,0.0);
            glVertex3f(1.0f, 1.0f, 1.0f);

            glTexCoord2f(1.0,0.0);
            glVertex3f(-1.0f, 1.0f, 1.0f);

            glTexCoord2f(1.0,1.0);
            glVertex3f(-1.0f, 1.0f, -1.0f);

        glEnd();


        //Bottom
        glBindTexture(GL_TEXTURE_2D, tex[2]);
        glBegin(GL_QUADS);
            glNormal3f(0.0f,1.0f,0.0f);

            glTexCoord2f(1.0,1.0);
            glVertex3f(-1.0f, -1.0f, 1.0f);

            glTexCoord2f(0.0,1.0);
            glVertex3f(1.0f, -1.0f, 1.0f);

            glTexCoord2f(0.0,0.0);
            glVertex3f(1.0f, -1.0f, -1.0f);

            glTexCoord2f(1.0,0.0);
            glVertex3f(-1.0f, -1.0f, -1.0f);

        glEnd();
         //left
         glBindTexture(GL_TEXTURE_2D, tex[0]);
        glBegin(GL_QUADS);
            glNormal3f(1.0f,0.0f,0.0f);

            glTexCoord2f(0.0,0.0);
            glVertex3f(-1.0f, 1.0f, -1.0f);

            glTexCoord2f(1.0,0.0);
            glVertex3f(-1.0f, 1.0f, 1.0f);

            glTexCoord2f(1.0,1.0);
            glVertex3f(-1.0f, -1.0f, 1.0f);

            glTexCoord2f(0.0,1.0);
            glVertex3f(-1.0f, -1.0f, -1.0f);

        glEnd();

        //Right
        glBindTexture(GL_TEXTURE_2D, tex[0]);
        glBegin(GL_QUADS);
            glNormal3f(-1.0f,0.0f,0.0f);

            glTexCoord2f(1.0,0.0);
            glVertex3f(1.0f, 1.0f, -1.0f);

            glTexCoord2f(1.0,1.0);
            glVertex3f(1.0f, -1.0f, -1.0f);

            glTexCoord2f(0.0,1.0);
            glVertex3f(1.0f, -1.0f, 1.0f);

            glTexCoord2f(0.0,0.0);
            glVertex3f(1.0f, 1.0f, 1.0f);

        glEnd();

    glPopMatrix();



    /*glPushMatrix();
    glColor3f(0.0,0.1,0.2);
    glutSolidTorus(0.5,1.0,30,30),
    glPopMatrix();*/

    glPushMatrix();//BALL 1
        glTranslatef(myBalls[0].position.x,myBalls[0].position.y,myBalls[0].position.z);
        glRotatef(spin,0,1,0);
      //  glColor3f(1.0,1.0,0.0);
        glLightfv(GL_LIGHT0+1,GL_POSITION,position);
        glBindTexture(GL_TEXTURE_2D, tex[3]);
        gluSphere(sphere, myBalls[0].radius, 50, 50);

    glPopMatrix();

    glPushMatrix();//BALL 2
        glTranslatef(myBalls[1].position.x,myBalls[1].position.y,myBalls[1].position.z);
        glRotatef(spin,0,1,0);
        //glColor3f(0.0,1.0,0.0);
        glLightfv(GL_LIGHT0+2,GL_POSITION,position);
        glBindTexture(GL_TEXTURE_2D, tex[4]);
        gluSphere(sphere, myBalls[1].radius, 50, 50);
    glPopMatrix();

    glPushMatrix();//BALL3
        glTranslatef(myBalls[2].position.x,myBalls[2].position.y,myBalls[2].position.z);
        glRotatef(spin,0,1,0);
        // glColor3f(1.0,0.0,0.0);
        glLightfv(GL_LIGHT0+3,GL_POSITION,position);
        glBindTexture(GL_TEXTURE_2D, tex[5]);
        gluSphere(sphere, myBalls[2].radius, 50, 50);
    glPopMatrix();

    glColor3f(1.0,1.,1.0);
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

        case ' ':
		Active = !Active;
	       break;

        case '1':
		eyex +=3.0;
	       break;

	       case '2':
		eyey +=3.0;
	       break;

	       case '3':
		eyez +=3.0;
	       break;

	       case '4':
		vx +=3.0;
	       break;

	       case '5':
		vy+=3.0;
	       break;

	       case '6':
		vz+=3.0;
	       break;

	       case '7':
		upx+=3.0;
	       break;

	       case '8':
		upy+=3.0;
	       break;

	       case '9':
		upz+=3.0;
	       break;



	       default:
                vx=vy=vz=upx=upz=eyex=eyey = 0 ;
                eyez=10;
                upy = 1.0;
            break;
    }
}

void Specialkeys(int key, int x, int y)
{
    switch(key)
    {
        case GLUT_KEY_UP:
            rotX=(-5+(int)rotX)%360;

        break;

        case GLUT_KEY_DOWN:
            rotX=(5+(int)rotX)%360;
        break;

        case GLUT_KEY_LEFT:
            rotY =((int)rotY -5)%360;
        break;

        case GLUT_KEY_RIGHT:
            rotY =((int)rotY+5)%360;

        break;
   }
  glutPostRedisplay();
}


static void idle(void)
{
    // Use parametric equation with t increment for xpos and y pos

    // Don't need a loop

    spin+=8;
    if(spin>360)spin=spin-360;

    //Movement
    myBalls[0].position.x+= myBalls[0].direction.x;
    myBalls[0].position.y+= myBalls[0].direction.y;
    myBalls[0].position.z+= myBalls[0].direction.z;

    myBalls[1].position.x+= myBalls[1].direction.x;
    myBalls[1].position.y+= myBalls[1].direction.y;
    myBalls[1].position.z+= myBalls[1].direction.z;

    myBalls[2].position.x+= myBalls[2].direction.x;
    myBalls[2].position.y+= myBalls[2].direction.y;
    myBalls[2].position.z+= myBalls[2].direction.z;


       // cout <<  myBalls[0].position.x << endl;

            //Wall Collision
             for(int i = 0; i < 3; i++){
                if(myBalls[i].position.x + myBalls[i].radius >= roomScale.x || myBalls[i].position.x - myBalls[i].radius<= -roomScale.x){
                    myBalls[i].direction.x*=-1;
                    myBalls[i].position.x += myBalls[i].direction.x;
                }

                if(myBalls[i].position.y + myBalls[i].radius >= roomScale.y || myBalls[i].position.y - myBalls[i].radius<= -roomScale.y){
                    myBalls[i].direction.y*=-1;
                    myBalls[i].position.y += myBalls[i].direction.y;
                }

                if(myBalls[i].position.z + myBalls[i].radius >= roomScale.z || myBalls[i].position.z - myBalls[i].radius<= -roomScale.z){
                    myBalls[i].direction.z*=-1;
                    myBalls[i].position.z += myBalls[i].direction.z;
                }


             }

            //Ball Collision
             for(int i = 0; i < (3 - 1); i++){
                for( int j = i+1; j < 3; j++ ){
                   temp = sqrt(
                            (myBalls[i].position.x - myBalls[j].position.x)*(myBalls[i].position.x - myBalls[j].position.x) +
                            (myBalls[i].position.y - myBalls[j].position.y)*(myBalls[i].position.y - myBalls[j].position.y) +
                            (myBalls[i].position.z - myBalls[j].position.z)*(myBalls[i].position.z - myBalls[j].position.z)
                         );
                    temp2 = myBalls[i].radius + myBalls[j].radius;
                  // cout << myBalls[i].position.x << "  "<< temp << "  " << temp2 << endl;
                    if(temp < temp2){
                            cout << i << "  " << j << "  " "Collision Occured" << endl;
                        myBalls[i].direction.x*=-1;
                        myBalls[i].position.x += myBalls[i].direction.x;
                        myBalls[i].direction.y*=-1;
                        myBalls[i].position.y += myBalls[i].direction.y;
                        myBalls[i].direction.z*=-1;
                        myBalls[i].position.z += myBalls[i].direction.z;

                        myBalls[j].direction.x*=-1;
                        myBalls[j].position.x += myBalls[j].direction.x;
                        myBalls[j].direction.y*=-1;
                        myBalls[j].position.y += myBalls[j].direction.y;
                        myBalls[j].direction.z*=-1;
                        myBalls[j].position.z += myBalls[j].direction.z;

                    }
                }
            }



    glutPostRedisplay();

    }


void mouse(int btn, int state, int x, int y){

    float scale = 100*(Wwidth/Wheight);

    switch(btn){
        case GLUT_LEFT_BUTTON:

        if(state==GLUT_DOWN){

               // get new mouse coordinates for x,y
               // use scale to match right
            }
            break;
    }
     glutPostRedisplay();
};



static void init(void)
{
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glClearColor(0.5f, 0.5f, 1.0f, 0.0f);                 // assign a color you like

    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);

    glEnable(GL_DEPTH_TEST);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glShadeModel(GL_SMOOTH);

    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);

    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0+1, GL_DIFFUSE,  light_diffuseRed);
    glLightfv(GL_LIGHT0+2, GL_DIFFUSE,  light_diffuseRed);
    glLightfv(GL_LIGHT0+3, GL_DIFFUSE,  light_diffuseRed);



    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);

    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT0+1);
    glEnable(GL_LIGHT0+2);
    glEnable(GL_LIGHT0+3);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);


    sphere = gluNewQuadric();
    gluQuadricDrawStyle(sphere, GLU_FILL);
    gluQuadricTexture(sphere, GL_TRUE);
    gluQuadricNormals(sphere, GLU_SMOOTH);

    glGenTextures(6, tex);
    glEnable(GL_TEXTURE_2D);
    start = clock();
    t=0;

    roomScale.x = 800.0;
    roomScale.y = 100.0;
    roomScale.z = 800.0;
    myBalls[0].direction.x = -1.0;
    myBalls[0].direction.y = 0.0;
    myBalls[0].direction.z = 0.0;
    myBalls[0].position.x = -12.0;
    myBalls[0].position.y = 0.0;
    myBalls[0].position.z = 0.0;
    myBalls[0].radius = 5;
   // myBalls[0].tex =

    myBalls[1].direction.x = 1.0;
    myBalls[1].direction.y = 0.0;
    myBalls[1].direction.z = 0.0;
    myBalls[1].position.x = 12.0;
    myBalls[1].position.y = 0.0;
    myBalls[1].position.z = 0.0;
    myBalls[1].radius = 5;
   // myBalls[1].tex =

    myBalls[2].direction.x = 1.5;
    myBalls[2].direction.y = 0.0;
    myBalls[2].direction.z = 1.0;
    myBalls[2].position.x = .0;
    myBalls[2].position.y = 0.0;
    myBalls[2].position.z = 0.0;
    myBalls[2].radius = 5;
  //  myBalls[2].tex

    vx=vy=vz=upx=upy=upz=eyex=eyey=eyez = 0 ;
    eyez=10;
    upy = 1.0;
    loadTexture("Images/myWall.png", tex[0]);
    loadTexture("Images/proof.png", tex[1]);
    loadTexture("Images/floors.jpg", tex[2]);

    loadTexture("Images/pb_0.jpg", tex[3]);
    loadTexture("Images/rpb.png", tex[4]);
    loadTexture("Images/bpg.jpg", tex[5]);




}

/* Program entry point */

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);

    glutInitWindowSize(800,600);
    glutInitWindowPosition(0,0);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("Project Assignment 2");
    init();
    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutKeyboardFunc(key);
    glutSpecialFunc(Specialkeys);

    glutIdleFunc(idle);

    glutMainLoop();

    return EXIT_SUCCESS;
}
