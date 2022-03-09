#include <string.h>
#define GLEW_STATIC
#include<GL/glew.h>
#include<GL/SOIL.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <sstream>
#define PI 3.14159

using namespace std;

typedef struct{
    float s;
    float t;
}vec2;

typedef struct{
    float x;
    float y;
    float z;
}vec3;

typedef struct Flist {
    int A1, B1, C1, A2, B2, C2, A3, B3, C3;
}Flist;

vec3 v1;
vec3 v2;
vec3 verti[3][100000];
vec3 norm[3][100000];
vec2 texCoords[3][100000];
Flist faceIndex[3][100000];

GLuint tex;
int numberOfVertices = 8;
int numberOfIndices = 12;
int totalIndices = numberOfIndices*3;
vec2 texCoordEpd[8];
vec3 normalsEpd[8];
int Vcount[3];
int t = 1;
int location;
float size1 = 3.0;
int indices[36];
int objInd;
bool WireFrame= false;
float i =0;
unsigned vs, fs, program;

const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };

    float rotx, roty, rotz;
    float xpos =0;
    float ypos =0;
    float RZscale=1,RXscale=0,RYscale=0;
    float Wwidth,Wheight;
    GLuint vbo[3];
    unsigned int EBO[3];

/* GLUT callback Handlers */
GLuint  createVBO(int objNum){
    int bufferSize;
    int sizeofBuffer;

    sizeofBuffer = sizeof(vec3)* Vcount[objNum];

    glGenBuffersARB(1, &vbo[objNum]);
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, vbo[objNum]);

    glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeofBuffer, 0, GL_DYNAMIC_DRAW);
    glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, 0, Vcount[objNum]*sizeof(vec3), (void*)verti[objNum]);

    glGetBufferParameterivARB(GL_ARRAY_BUFFER_ARB, GL_BUFFER_SIZE_ARB, &bufferSize);

    if(sizeofBuffer!=bufferSize){
        glDeleteBuffersARB(1, &vbo[objNum]);
        vbo[objNum] = 0;
        cout << "Buffer size did not match" << endl;

    }

     glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);


     glGenBuffers(1, &EBO[objNum]);
     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[objNum]);
     glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);
     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

     return vbo[objNum];
}

void deleteVBO(int objNum){//Deletes the VBO
    glBindBuffer(1, vbo[objNum]);
    glDeleteBuffers(1, &vbo[objNum]);
}

void BindTexture(char * fileName){//Binds texture onto model
   unsigned char* image;
    int width, height;
    glGenTextures(1, &tex);

    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_MODULATE);

    glBindTexture(GL_TEXTURE_2D, tex);
    image = SOIL_load_image(fileName, &width, &height, 0,  SOIL_LOAD_RGBA);

    if(!image)cout<<"no image found";
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

    SOIL_free_image_data(image);


    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

}

void loadFile(char * fileName, string & str)//Load .txt file functions
{
    char temp[1024];
    ifstream in(fileName);

    if(!in.is_open()){
        cout << "File not found" << endl;
        return;
    }

    while(!in.eof()){
        in.getline(temp, 1024);
        str += temp;
        str += '\n';
    }
    cout << str << endl;

}
void normalCalc(int objNum) //calculates normals
{
 for(int i = 0; i < 30337; i++){
    vec3 v1;
    vec3 v2;
    float Nx, Ny, Nz;
    int vert1 = faceIndex[objNum][i].A1-1;// line i of the set of faces first vertex
    int vert2 = faceIndex[objNum][i].A2-1;// line i of the set of faces second vertex
    int vert3 = faceIndex[objNum][i].A3-1;// line i of the set of faces third vertex


            v1.x = verti[objNum][vert2].x - verti[objNum][vert1].x;//Surface normals
            v1.y = verti[objNum][vert2].y - verti[objNum][vert1].y;
            v1.z = verti[objNum][vert2].z - verti[objNum][vert1].z;

            v2.x = verti[objNum][vert3].x - verti[objNum][vert2].x;
            v2.y = verti[objNum][vert3].y - verti[objNum][vert2].y;
            v2.z = verti[objNum][vert3].z - verti[objNum][vert2].z;

            Nx = (v1.y * v2.z) - (v1.z * v2.y); //Cross products                                                //Cross product the vectors
            Ny = (v1.z * v2.x) - (v1.x * v2.z);
            Nz = (v1.x * v2.y) - (v1.y * v2.x);
     float length = sqrt((Nx*Nx)+(Ny*Ny)+(Nz*Nz)); // length
            norm[objNum][i].x = Nx/length;  // vertex normal calculations                                                               //Normalize the normals
            norm[objNum][i].y = Ny/length;
            norm[objNum][i].z = Nz/length;
    faceIndex[objNum][i].C1 = i+1;
    faceIndex[objNum][i].C2 = i+1;
    faceIndex[objNum][i].C3 = i+1;
 }
}

void ObjParser(const char * objStr, int objNum)//Parses .obj files and saves them to different arrays
{
    ifstream fin;
    fin.open(objStr);
    int vc = 0, fc = 0, vtc = 0, counter = 0, facevalue;
    float Tx,Ty,Tz;
    float factor = 200;


    string line, tempstring;
    while (getline(fin, line))
    {

      if (line.substr(0,2)=="v "){ //if 'v' scan and load string
        istringstream v(line.substr(1));//make substring to convert to int
        v >> Tx >> Ty >> Tz;
        verti[objNum][vc].x = Tx * 0.005;
        verti[objNum][vc].y = Ty * 0.005;
        verti[objNum][vc].z = Tz * 0.005;
        vc++;

      }
      else if(line.substr(0,3)=="vt "){//if 'vt' scan and load string
        istringstream v(line.substr(3));//make substring to convert to int
        v >> Tx >> Ty;
        texCoords[objNum][vtc].s = Tx;
        texCoords[objNum][vtc].t = Ty;
        vtc++;

      }
      else if(line.substr(0,2)=="f "){//if 'f' scan and load string using different method
        counter= 0;
        tempstring = "";
        istringstream v(line.substr(2));
            for(int i = 2; i < line.length(); i++){

                if(line.at(i) == '/' || line.at(i) == ' '){
                        istringstream face(tempstring); // convert string to int
                        face >> facevalue;
                        if(counter==0){  // separating each string individually between white spaces and back slashes
                            faceIndex[objNum][fc].A1 = facevalue;
                            counter++;
                            tempstring = "";
                        }
                        else if(counter==1){
                            faceIndex[objNum][fc].B1 = facevalue;
                            counter++;
                            tempstring = "";
                        }
                        else if(counter==2){
                            faceIndex[objNum][fc].C1 = facevalue;
                            counter++;
                            tempstring = "";
                        }
                        else if(counter==3){
                            faceIndex[objNum][fc].A2 = facevalue;
                            counter++;
                            tempstring = "";
                        }
                        else if(counter==4){
                            faceIndex[objNum][fc].B2 = facevalue;
                            counter++;
                            tempstring = "";
                        }
                        else if(counter==5){
                            faceIndex[objNum][fc].C2 = facevalue;
                            counter++;
                            tempstring = "";
                        }
                        else if(counter==6){
                            faceIndex[objNum][fc].A3 = facevalue;
                            counter++;
                            tempstring = "";
                        }
                        else if(counter==7){
                            faceIndex[objNum][fc].B3 = facevalue;
                            counter++;
                            tempstring = "";
                        }

                }
                else {
                    tempstring += line.at(i);
                }

            }
            istringstream face(tempstring);
            face >> facevalue;
            faceIndex[objNum][fc].C3 = facevalue;
        fc++;
      }

    }
    Vcount[objNum] = fc;
    normalCalc(objNum);

}
unsigned int shaderCompiler(string & source, unsigned int mode) // shows if the shader loads properly
{
    unsigned int id;
    const char * cSource;

    char error[1024];
    id = glCreateShader(mode);
    cSource = source.c_str();

    glShaderSource(id,1,&cSource,NULL);
    glCompileShader(id);

    glGetShaderInfoLog(id, 1024, NULL, error);
    cout << "Compile Status: "<< error << endl;
    return id;
}

void initShader(char * vsShader, char * fsShader) // initializes shader
{
    string source;

    loadFile(vsShader, source);
    vs = shaderCompiler(source, GL_VERTEX_SHADER);
    source = " ";

    loadFile(fsShader, source);
    vs = shaderCompiler(source, GL_FRAGMENT_SHADER);


    program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);

    glLinkProgram(program);


}

void clean() //deletes all VBOS and cleans the residue of the models
{
    glDetachShader(program, vs);
    glDetachShader(program, fs);
    glDeleteShader(vs);
    glDeleteShader(fs);
    glDeleteProgram(program);
    deleteVBO(0);
    deleteVBO(1);
    deleteVBO(2);

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

    if(WireFrame)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);		//Draw Our Mesh In Wireframe Mesh
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);		//Toggle WIRE FRAME

    glUseProgram(0);
    glUniform1f(location, size1);

    glPushMatrix();
        //glScalef(0.2, 0.2, 0.2);
        glTranslated(RXscale,RYscale,RZscale);//allows us to translate it further
        glRotatef(rotx, 1,0,0);
        glRotatef(roty, 0,1,0);
        glRotatef(rotz, 0,0,1);

            glBindBufferARB(GL_ARRAY_BUFFER_ARB, vbo[objInd]);

            glEnable(GL_VERTEX_ARRAY);
              glVertexPointer(3, GL_FLOAT, 0, 0);

              glBindTexture(GL_TEXTURE_2D, tex);

               for(int i = 0; i < Vcount[objInd]; i++){
                    glBegin(GL_TRIANGLES);

            glNormal3f(norm[objInd][(faceIndex[objInd][i].C1)-1].x, norm[objInd][(faceIndex[objInd][i].C1)-1].y, norm[objInd][(faceIndex[objInd][i].C1)-1].z);// draws normals
            glTexCoord2f(texCoords[objInd][(faceIndex[objInd][i].B1)-1].s, texCoords[objInd][(faceIndex[objInd][i].B1)-1].t);//draws textures

            //glVertex3f(verti[objInd][(faceIndex[objInd][i].A1)-1].x,verti[objInd][(faceIndex[objInd][i].A1)-1].y,verti[objInd][(faceIndex[objInd][i].A1)-1].z); //WITHOUT VBO
            glArrayElement(faceIndex[objInd][i].A1-1); //WITH VBO

            glNormal3f(norm[objInd][(faceIndex[objInd][i].C2)-1].x, norm[objInd][(faceIndex[objInd][i].C2)-1].y, norm[objInd][(faceIndex[objInd][i].C2)-1].z);// draws normals
            glTexCoord2f(texCoords[objInd][(faceIndex[objInd][i].B2)-1].s, texCoords[objInd][(faceIndex[objInd][i].B2)-1].t); //draws textures
           // glVertex3f(verti[objInd][(faceIndex[objInd][i].A2)-1].x,verti[objInd][(faceIndex[objInd][i].A2)-1].y,verti[objInd][(faceIndex[objInd][i].A2)-1].z);//WITHOUT VBO
           glArrayElement(faceIndex[objInd][i].A2-1);//WITH VBO

            glNormal3f(norm[objInd][(faceIndex[objInd][i].C3)-1].x, norm[objInd][(faceIndex[objInd][i].C3)-1].y, norm[objInd][(faceIndex[objInd][i].C3)-1].z);// draws normals
            glTexCoord2f(texCoords[objInd][(faceIndex[objInd][i].B3)-1].s, texCoords[objInd][(faceIndex[objInd][i].B3)-1].t);//draws textures
           // glVertex3f(verti[objInd][(faceIndex[objInd][i].A3)-1].x,verti[objInd][(faceIndex[objInd][i].A3)-1].y,verti[objInd][(faceIndex[objInd][i].A3)-1].z);//WITHOUT VBO
           glArrayElement(faceIndex[objInd][i].A3-1);//WITH VBO

                    /*glNormal3f(norm[faceIndex[i][2]-1].x, norm[faceIndex[i][2]-1].y, norm[faceIndex[i][2]-1].z);
                       glTexCoord2f(texCoords[faceIndex[i][1]-1].s,texCoords[faceIndex[i][1]-1].t);
                        glArrayElement(faceIndex[objInd][i][0]-1);

                    glNormal3f(norm[faceIndex[i][5]-1].x, norm[faceIndex[i][5]-1].y, norm[faceIndex[i][5]-1].z);
                       glTexCoord2f(texCoords[faceIndex[i][4]-1].s,texCoords[faceIndex[i][4]-1].t);
                        glArrayElement(faceIndex[objInd][i][3]-1);

                    glNormal3f(norm[faceIndex[i][8]-1].x, norm[faceIndex[i][8]-1].y, norm[faceIndex[i][8]-1].z);
                       glTexCoord2f(texCoords[faceIndex[i][7]-1].s,texCoords[faceIndex[i][7]-1].t);
                        glArrayElement(faceIndex[objInd][i][6]-1);
                        */

                    glEnd();
                }

             glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[objInd]);
             glDrawElements(GL_TRIANGLES, Vcount[objInd], GL_UNSIGNED_INT, 0);

            glDisable(GL_VERTEX_ARRAY);

    glPopMatrix();
    glUseProgram(0);
    // your code here
    glutSwapBuffers();
}


static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27 :
        case 'q':
            clean();
            exit(0);
            break;

        case 'w':
            WireFrame =!WireFrame;
            break;

        case '1':
            objInd = 0;
            //objStr = "bunny.obj";
            break;
        case '2':
            objInd = 1;
            //objStr = "";
            break;
        case '3':
            objInd = 2;
            //objStr = "";
            break;

    }
}

void Specialkeys(int key, int x, int y)
{
    switch(key)
    {
    case GLUT_KEY_LEFT:
        roty-=5;
    break;
    case GLUT_KEY_RIGHT:
        roty+=5;
    break;
    case GLUT_KEY_UP:
        rotx-=5;
    break;
    case GLUT_KEY_DOWN:
         rotx+=5;
    break;
    case GLUT_KEY_PAGE_UP:
         rotz-=5;
    break;
    case GLUT_KEY_PAGE_DOWN:
         rotz+=5;
    break;

    case GLUT_KEY_HOME:
     RZscale=(RZscale -1);//Zoom out
     break;
    case GLUT_KEY_END:
     RZscale=(RZscale +1);//Zoom in
     break;

   }
  glutPostRedisplay();
}


static void idle(void)
{
    // Use parametric equation with t increment for xpos and y pos
    // Don't need a loop
    size1>3.0? t=-1:(size1< 0.5? t=1:NULL);
    size1 += 0.01*t;

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
    glewInit();
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
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);

    initShader("v.vs.txt","f.fs.txt");
    glUseProgram(program);
    location = glGetUniformLocation(program,"Scale");

    if(location != -1)
    {
        glUniform1f(location, size1);
    }

    else
    {
        cout << "Location not found" << endl;
    }

    glUseProgram(0);

     int j = 0;



     glEnable(GL_TEXTURE_2D);

    ObjParser("bunny.obj", 0);
    ObjParser("venusm.obj", 1);
    ObjParser("elephal.obj", 2);
    createVBO(0);
    createVBO(1);
    createVBO(2);
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

    glutIdleFunc(idle);

    glutMainLoop();

    clean();
    return EXIT_SUCCESS;
}
