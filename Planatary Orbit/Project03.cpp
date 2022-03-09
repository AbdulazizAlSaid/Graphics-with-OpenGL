/*
Abdulaziz Zyad Al-Said
ID:110023088
CSCI 172
Project 4
*/
#include <cmath>
#ifdef __SUN__
#else
#endif
#include <cstdio>
#include <string>
#include <string.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <GL/SOIL.h>
using namespace std;

bool WireFrame = false;
bool Active = false;
float roll = 0;
double SunR = 0;
GLUquadricObj	*sphere=	NULL;
GLuint sun;
GLuint moon;
GLuint earth;
GLuint mars;
GLfloat scaleCz=-3.0,scaleSz=-1.0,scaleTz=2.0;//variables used to zoom on the z axis
GLfloat viewangleTz=0,viewangleCz=0,viewangleSz=0;//variables used to rotate the objects
void loadTexture(char *filename, GLuint tex);
const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { -20.0f, 10.0f, 0.0f, 0.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 50.0f };

float rot =0;
/* GLUT callback Handlers */
void glutSolidSphere ( GLdouble radius, GLint slices, GLint stacks); // (1.3,24,24)

// Planet class that will hold all our planetary data
class Planet {
public:
	std::string name = "";
	double x = 0;
	double y = 0;
	double R = 0; //RED
	double G = 0; //GREEN
	double B = 0;  //BLUE
	double rs = 0; //ROTATION SPEED
	double rsc = 0; //ROTATION SPEED CURRRENT
	double rad = 0; //RADIUS
	double s = 0; //SIZE
	double d = 0; //DAY
	double dc = 0; //DAY CURRENT

	Planet(std::string,double,double,double,double,double,double,double);
	void glStuff(double t, double closeness);
	void printPlanet();
};

// Simple planet constructor
Planet::Planet(std::string name="",double R=0,double G=0,double B=0,double rs=0,double rad=0,double s=0, double d=0) {

	this->name = name;
	this->x = 0;
	this->y = 0;
	this->R = R;
	this->G = G;
	this->B = B;
	this->rs = rs;
	this->rsc = 0;
	this->rad = rad;
	this->s = s;
	this->d = d;
	this->dc = 0;
}



static double closeness = 20.0;
Planet** planets;
static double t; // time

static void resize(int width, int height)
{
     const float ar = (float) width / (float) height;

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-ar, ar, -1.0, 1.0, 2.0, 100.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity() ;
 }

static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_LIGHTING);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

        gluLookAt(0,closeness,closeness,0.0,0.0,0.0,0.0,1.0,0.0);



        if(WireFrame)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);		//Draw Our Mesh In Wireframe Mesh
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);		//Toggle WIRE FRAME



        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glPushMatrix();
         glRotated(roll, 0, 0, 1);

        glPushMatrix();  // SUN
            glRotated(SunR, 0, 1, 0);
            glColor3d(planets[0]->R,planets[0]->G,planets[0]->B);
            glBindTexture(GL_TEXTURE_2D, sun);
            gluSphere(sphere, planets[0]->s, 40,40);

            glPushMatrix();// EARTH

            glRotated(planets[2]->rsc,0,1,0);
            glTranslated(planets[2]->rad,0,0);
            glRotated(planets[2]->dc,0,1,0);

                glPushMatrix(); // MOON

                glRotated(planets[3]->rsc,0,1,0);
                glTranslated(planets[3]->rad,0,0);
                glRotated(planets[3]->dc,0,1,0);

                glColor3d(planets[3]->R,planets[3]->G,planets[3]->B);
                glBindTexture(GL_TEXTURE_2D, moon);
                gluSphere(sphere, planets[3]->s,24,24);

                glPopMatrix();

            glColor3d(planets[2]->R,planets[2]->G,planets[2]->B);
            glBindTexture(GL_TEXTURE_2D, earth);
           gluSphere(sphere, planets[2]->s,24,24);
            glPopMatrix();

            glPushMatrix();// PLANET B

            glRotated(planets[1]->rsc,0,1,0);
            glTranslated(planets[1]->rad,0,0);
            glRotated(planets[1]->dc,0,1,0);

            glColor3d(planets[1]->R,planets[1]->G,planets[1]->B);
            glBindTexture(GL_TEXTURE_2D, mars);
            gluSphere(sphere, planets[1]->s,24,24);

            glPopMatrix();
        glPopMatrix();
        glPopMatrix();

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

        case 'w': // Enables wireframe
          WireFrame = !WireFrame;
            break;


         case ' ': //PAUSES
             Active = !Active;
            break;
}
}

void Specialkeys(int key, int x, int y)
{

    switch(key)
    {
        case GLUT_KEY_UP:
            closeness -= 0.5;
			break;
        case GLUT_KEY_DOWN:
			closeness += 0.5;
			break;
        case GLUT_KEY_LEFT:
              roll-=5;
              break;

        case GLUT_KEY_RIGHT:
                roll+=5;
                break;


    }
  glutPostRedisplay();
}

static void idle(void){

    if (Active) // ENABLES PAUSE
    {
    for(int i=0; i<4 ; i++)
    {
        planets[i]->rsc += planets[i]->rs;
        planets[i]->dc += planets[i]->d;

    }
    }
    glutPostRedisplay();
}

static void init(void)
{
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

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);

    sphere = gluNewQuadric();
    gluQuadricDrawStyle(sphere, GLU_FILL);
    gluQuadricTexture(sphere, GL_TRUE);
    gluQuadricNormals(sphere, GLU_SMOOTH);

    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &sun);

    loadTexture("images/Sun.png", sun);
    loadTexture("images/Earth.jpg",earth);
    loadTexture("images/Moon.jpg", moon);
    loadTexture("images/Mars.jpg", mars);
}

void loadTexture(char *filename, GLuint tex)
{

 glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
 glBindTexture(GL_TEXTURE_2D, tex); // images are 2D arrays of pixels, bound to the GL_TEXTURE_2D target.
 int width, height; // width & height for the image reader
 unsigned char* image;
 image = SOIL_load_image(filename, &width, &height, 0, SOIL_LOAD_RGBA);
 glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
// binding image data
 SOIL_free_image_data(image);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

/* Program entry point */

int main(int argc, char *argv[])
{
	planets = new Planet*[3];
	Planet A("sun", 1.0, 0.6, 0.0, 0.0, 0, 3.0, 0);        planets[0] = &A;
	Planet B("Planet B", 0.0, 1.0, 1.0, 2, 10.0, 1.30, 4);   planets[1] = &B;
	Planet C("Earth", 0.6, 0.6, 0.2, 0.5, 25.0, 1.38, 2);     planets[2] = &C;
    Planet D("Moon", 0.5, 0.5, 0.5, 0.15, 8.0, 1.15, 1.2);     planets[3] = &D;

    glutInit(&argc, argv);
    glutInitWindowSize(1100,1000);
    glutInitWindowPosition(100,20);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("Solar System");

    init();
    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutSpecialFunc(Specialkeys);
    glutIdleFunc(idle);
    glutMainLoop();

    glClearColor(0,0,0,0);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);




	glutMainLoop();

    return EXIT_SUCCESS;
}

