/*
Abdulaziz Zyad Al-Said
ID:110023088
CSCI 172
Project 2
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
using namespace std;

bool WireFrame = false;
bool Active = false;
float roll = 0;
double SunR = 0;

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
	double R = 0;
	double G = 0;
	double B = 0;
	double rs = 0;
	double rsc = 0;
	double rad = 0;
	double s = 0;
	double d = 0;
	double dc = 0;

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
double moon_x; // moon x position
double moon_y; // moon y position
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
            glutSolidSphere(planets[0]->s,24,24);

            glPushMatrix();// EARTH

            glRotated(planets[2]->rsc,0,1,0);
            glTranslated(planets[2]->rad,0,0);
            glRotated(planets[2]->dc,0,1,0);

                glPushMatrix(); // MOON

                glRotated(planets[3]->rsc,0,1,0);
                glTranslated(planets[3]->rad,0,0);
                glRotated(planets[3]->dc,0,1,0);

                glColor3d(planets[3]->R,planets[3]->G,planets[3]->B);
                glutSolidSphere(planets[3]->s,24,24);

                glPopMatrix();

            glColor3d(planets[2]->R,planets[2]->G,planets[2]->B);
            glutSolidSphere(planets[2]->s,24,24);
            glPopMatrix();

            glPushMatrix();// MARS

            glRotated(planets[1]->rsc,0,1,0);
            glTranslated(planets[1]->rad,0,0);
            glRotated(planets[1]->dc,0,1,0);

            glColor3d(planets[1]->R,planets[1]->G,planets[1]->B);
            glutSolidSphere(planets[1]->s,24,24);

            glPopMatrix();
        glPopMatrix();
        glPopMatrix();





        /*

        glPushMatrix();
        glRotated( roll,1,0,0);

        for(int i = 0; i < 3; i++) {
            (*planets[i]).glStuff(t, closeness);
        }

        // moon orbit calculation
        moon_x = -sin(t)*0.5+ planets[2]->x;
        moon_y = cos(t)*0.5+ planets[2]->y;

        // "Moon"
        glColor3d(0.7,0.7,0.7);
        glPushMatrix();
        glTranslated(moon_x , moon_y , closeness);
        glRotated(60,1,0,0);
        glRotated(50.0*t,0,0,1);
        glutSolidSphere(0.1,20,20);
        glPopMatrix();


        glPopMatrix();

        */


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


         case ' ': //Undoes wireframe
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

    if (Active)
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
}


/* Program entry point */

int main(int argc, char *argv[])
{
    // Although the sun is not a planet it is
	// convenient to keep in our array of planets
	planets = new Planet*[3];
	Planet A("sun", 1.0, 0.6, 0.0, 0.0, 0.0, 2.0, 0.1);        planets[0] = &A;
	Planet B("Planet B", 0.0, 1.0, 1.0, 2, 3.0, 0.30, 1);   planets[1] = &B;
	Planet C("Earth", 0.6, 0.6, 0.2, 0.5, 8.0, 0.38, 1);     planets[2] = &C;
    Planet D("Moon", 0.5, 0.5, 0.5, 0.5, 1.0, 0.15, 0.1);     planets[3] = &D;

    glutInit(&argc, argv);
    glutInitWindowSize(800,600);
    glutInitWindowPosition(100,20);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

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

