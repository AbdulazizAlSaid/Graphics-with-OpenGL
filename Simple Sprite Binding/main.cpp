
#include <string.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <iostream>
#include <GL/SOIL.h>

#include <math.h>
#include <cmath>
#include <time.h>
using namespace std;

bool WireFrame= false;
bool mirror = false;
float xmin=0;
float xmax=.125;
float ymin=0;
float ymax=.25;
clock_t prevTime;
int currentFrame = 0;
void textureLoader(char *, GLuint &);
const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };


/* GLUT callback Handlers */
GLuint tex;
GLuint bgTex;

static void resize(int width, int height)
{
     double Ratio;

   if(width<=height)
            glViewport(0,(GLsizei) (height-width)/2,(GLsizei) width,(GLsizei) width);
    else
          glViewport((GLsizei) (width-height)/2 ,0 ,(GLsizei) height,(GLsizei) height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	gluPerspective (50.0f,1,0.1f, 100.0f);
 }

static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(0,0,4,0.0,0.0,0.0,0.0,1.0,0.0);

    if(WireFrame)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);		//Draw Our Mesh In Wireframe Mesh
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);		//Toggle WIRE FRAME

    // your code here
    float tempMin = xmin;
    float tempMax = xmax;
    if(mirror)
{
  tempMin = xmax;
  tempMax = xmin;
}
    glBindTexture(GL_TEXTURE_2D, tex);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f); // Normal Front Face
    glTexCoord2f(tempMin, ymax); glVertex3f(-1.0f, -1.0f, 1.0f); // Bottom Left Of The Texture and Quad
    glTexCoord2f(tempMax, ymax); glVertex3f( 1.0f, -1.0f, 1.0f); // Bottom Right Of The Texture and Quad
    glTexCoord2f(tempMax, ymin); glVertex3f( 1.0f, 1.0f, 1.0f); // Top Right Of The Texture and Quad
    glTexCoord2f(tempMin, ymin); glVertex3f(-1.0f, 1.0f, 1.0f); // Top Left Of The Texture and Quad
    glEnd();

    glBindTexture(GL_TEXTURE_2D, bgTex);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f); // Normal Front Face
    glTexCoord2f(0, 2); glVertex3f(-2.0f, -2.0f, .98f); // Bottom Left Of The Texture and Quad
    glTexCoord2f(2, 2); glVertex3f( 2.0f, -2.0f, .98f); // Bottom Right Of The Texture and Quad
    glTexCoord2f(2, 0); glVertex3f( 2.0f, 2.0f, .98f); // Top Right Of The Texture and Quad
    glTexCoord2f(0, 0); glVertex3f(-2.0f, 2.0f, .98f); // Top Left Of The Texture and Quad
    glEnd();
    glutSwapBuffers();
}

static void key(unsigned char key, int x, int y)
{
     clock_t currTime = clock();
    if(currTime-prevTime>40){
        prevTime=currTime;
    switch (key)
    {
            case 27 :
            case 'q':
            exit(0);
            break;

            case 'w':
            ymin=.75;
            ymax=1;
            currentFrame++;
            currentFrame%8;
            xmin=currentFrame*.125;
            xmax=currentFrame*.125+.125;
            break;

            case 'a':
            ymin=.25;
            ymax=.50;
            currentFrame++;
            currentFrame%8;
            xmin=currentFrame*.125;
            xmax=currentFrame*.125+.125;
            break;

            case 's':
            ymin=0;
            ymax=.25;
            currentFrame++;
            currentFrame%8;
            xmin=currentFrame*.125;
            xmax=currentFrame*.125+.125;
            break;

            case 'd':
            ymin=.50;
            ymax=.75;
            currentFrame++;
            currentFrame%8;
            xmin=currentFrame*.125;
            xmax=currentFrame*.125+.125;
            break;
    }
    }
}

void Specialkeys(int key, int x, int y)
{
    clock_t currTime = clock();
    if(currTime-prevTime>75){
        prevTime=currTime;

        switch(key)
        {
        case GLUT_KEY_UP:
            ymin=.75;
            ymax=1;
            currentFrame++;
            currentFrame%8;
            xmin=currentFrame*.125;
            xmax=currentFrame*.125+.125;
        break;

        case GLUT_KEY_DOWN:
            ymin=0;
            ymax=.25;
            currentFrame++;
            currentFrame%8;
            xmin=currentFrame*.125;
            xmax=currentFrame*.125+.125;
        break;

        case GLUT_KEY_LEFT:
            mirror = false;
            ymin=.25;
            ymax=.50;
            currentFrame++;
            currentFrame%8;
            xmin=currentFrame*.125;
            xmax=currentFrame*.125+.125;
        break;

        case GLUT_KEY_RIGHT:
            mirror = true;
            ymin=.25;
            ymax=.50;
            currentFrame++;
            currentFrame%8;
            xmin=currentFrame*.125;
            xmax=currentFrame*.125+.125;
        break;
       }

    }
  glutPostRedisplay();


}

static void idle(void)
{
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

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glGenTextures(1, &tex);
    textureLoader("images/run2.png", tex);
    textureLoader("images/grass2.png", bgTex);

    prevTime=clock();
}

void textureLoader(char *filename, GLuint &tex)
{

 glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
 glBindTexture(GL_TEXTURE_2D, tex); // images are 2D arrays of pixels, bound to the GL_TEXTURE_2D target.
 int width, height; // width & height for the image reader
 unsigned char* image;
 image = SOIL_load_image(filename, &width, &height, 0, SOIL_LOAD_RGBA);
 if(!image){cout<<filename << " Not Found" << endl;}
 glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
// binding image data
 SOIL_free_image_data(image);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

/* Program entry point */

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);

    glutInitWindowSize(800,600);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("GLUT Shapes");
    init();
    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutSpecialFunc(Specialkeys);

    glutIdleFunc(idle);
    glutMainLoop();

    return EXIT_SUCCESS;
}
