// Author: Matthew Wraith

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <cmath>

#include "Camera.h"

using namespace std;

#define FWDSPEED   0.05
#define ROLLSPEED  0.1
#define PITCHSPEED 0.1

Camera camera;
bool moving = true;

float sealevel;
float polysize;
float mountainsize = 100;


int nFPS = 30;

int seed(float x, float y) {
    static int a = 1588635695, b = 1117695901;
    int xi = *(int *)&x;
    int yi = *(int *)&y;
    return ((xi * a) % b) - ((yi * b) % a);
}

// Create a random mountain
void mountain(
        float x0, float y0, float z0, 
        float x1, float y1, float z1, 
        float x2, float y2, float z2, 
        float s) {
    float x01,y01,z01,x12,y12,z12,x20,y20,z20;

    if (s < polysize) {
        x01 = x1 - x0;
        y01 = y1 - y0;
        z01 = z1 - z0;

        x12 = x2 - x1;
        y12 = y2 - y1;
        z12 = z2 - z1;

        x20 = x0 - x2;
        y20 = y0 - y2;
        z20 = z0 - z2;

        float nx = y01*(-z20) - (-y20)*z01;
        float ny = z01*(-x20) - (-z20)*x01;
        float nz = x01*(-y20) - (-x20)*y01;

        float den = sqrt(nx*nx + ny*ny + nz*nz);

        if (den > 0.0) {
            nx /= den;
            ny /= den;
            nz /= den;
        }

        glNormal3f(nx,ny,nz);
        glBegin(GL_TRIANGLES);
            glVertex3f(x0,y0,z0);
            glVertex3f(x1,y1,z1);
            glVertex3f(x2,y2,z2);
        glEnd();

        return;
    }

    x01 = 0.5*(x0 + x1);
    y01 = 0.5*(y0 + y1);
    z01 = 0.5*(z0 + z1);

    x12 = 0.5*(x1 + x2);
    y12 = 0.5*(y1 + y2);
    z12 = 0.5*(z1 + z2);

    x20 = 0.5*(x2 + x0);
    y20 = 0.5*(y2 + y0);
    z20 = 0.5*(z2 + z0);

    s *= 0.5;

    srand(seed(x01,y01));
    z01 += 0.3*s*(2.0*((float)rand()/(float)RAND_MAX) - 1.0);
    srand(seed(x12,y12));
    z12 += 0.3*s*(2.0*((float)rand()/(float)RAND_MAX) - 1.0);
    srand(seed(x20,y20));
    z20 += 0.3*s*(2.0*((float)rand()/(float)RAND_MAX) - 1.0);

    mountain(x0,y0,z0,x01,y01,z01,x20,y20,z20,s);
    mountain(x1,y1,z1,x12,y12,z12,x01,y01,z01,s);
    mountain(x2,y2,z2,x20,y20,z20,x12,y12,z12,s);
    mountain(x01,y01,z01,x12,y12,z12,x20,y20,z20,s);
}

// Initialization
void init(void) {
    GLfloat white[] = {1.0,1.0,1.0,1.0};
    GLfloat lpos[] = {0.0,0.0,1.0,0.0};

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glLightfv(GL_LIGHT0, GL_POSITION, lpos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, white);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT0, GL_SPECULAR, white);

    glClearColor (0.5, 0.5, 1.0, 0.0);
    /* glShadeModel (GL_FLAT); */
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    glPointSize(3.0f);

    sealevel = 0.0;
    polysize = 0.04*mountainsize;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(90.0,1.0,0.1,300.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Initialize the camera
    camera.setPosition(0.0, 2.5, 5.0, 0.0, 2.5, 0.0, 0.0, 1.0, 0.0);
}

// If I want gridlines for reference
void drawGrid() {
    glBegin(GL_LINES);
    for(float i = -500; i <= 500; i += 5) {
        glColor3ub(150, 190, 150);
        glVertex3f(-500, 0, i); 
        glVertex3f(500, 0, i); 
        glVertex3f(i, 0,-500);
        glVertex3f(i, 0, 500);
    }   
    glEnd();
}

// The display function
void display(void) {
    // Material properties
    GLfloat tanamb[] = {0.2,0.15,0.1,1.0};
    GLfloat tandiff[] = {0.4,0.3,0.2,1.0};

    GLfloat seaamb[] = {0.0,0.0,0.2,1.0};
    GLfloat seadiff[] = {0.0,0.0,0.8,1.0};
    GLfloat seaspec[] = {0.5,0.5,1.0,1.0};

    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0);
    glLoadIdentity();             /* clear the matrix */

    // Move forward and set the camera position
    if (moving) camera.move(FWDSPEED);
    camera.drawCamera();

    // If I want a grid
    //glDisable(GL_LIGHTING);
    //drawGrid();
    //glEnable(GL_LIGHTING);

    // Show the mountain
    glPushMatrix();
    glTranslatef (-mountainsize/2, -mountainsize/2, 0.0); // modeling transformation 
    
    // I originally wrote this code with y being up, 
    // so I just rotate the model into this coordinate system
    glRotatef(90.0,-1.0,0.0,0.0);   
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, tanamb);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, tandiff);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, tandiff);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 10.0);

    // Variable size mountain
    mountain(0.0,0.0,0.0, mountainsize,0.0,0.0, 0.0,mountainsize,0.0, mountainsize);
    mountain(mountainsize,mountainsize,0.0, 0.0,mountainsize,0.0, mountainsize,0.0,0.0, mountainsize);

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, seaamb);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, seadiff);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, seaspec);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 10.0);

    glNormal3f(0.0,0.0,1.0);
    glBegin(GL_QUADS);
        glVertex3f(0.0,0.0,sealevel);
        glVertex3f(mountainsize,0.0,sealevel);
        glVertex3f(mountainsize,mountainsize,sealevel);
        glVertex3f(0.0,mountainsize,sealevel);
        //glVertex3f(0.0, sealevel, 0.0);
        //glVertex3f(mountainsize, sealevel, 0.0);
        //glVertex3f(mountainsize, sealevel, mountainsize);
        //glVertex3f(0.0,sealevel, mountainsize);
    glEnd();
    glPopMatrix();

    glutSwapBuffers();
    glFlush ();

    glutPostRedisplay();
}

void reshape (int w, int h) {
    glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective(90.0,1.0,0.1,300.0);
    glMatrixMode (GL_MODELVIEW);
}

// The time keeping function
void timer(int v) {
    // trigger display function by sending redraw into message queue
    glutPostRedisplay();    
    glutTimerFunc(1000/nFPS,timer,v); // restart timer again
}

// The mouse function:
// If the user clicks in the upper part of the window, the camera will pitch up.
// if the user clicks in the lower part of the window, the camera will pitch
// down.
//
// If the user clicks in the right part of the screen, 
// the camera will roll right.
// If the user clicks in the left part of the screen, 
// the camera will roll left.
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        float w, h;
        GLint m_viewport[4];

        glGetIntegerv(GL_VIEWPORT, m_viewport);

        w = (float) m_viewport[2];
        h = (float) m_viewport[3];

        // Window coords
        float wx = (2.0f*(float) x) / w - 1.0f;
        float wy = (-2.0f*(float) y ) / h + 1.0f;

        if (wy > 0.1)
            camera.pitch(-PITCHSPEED);
        else if (wy < -0.1)
            camera.pitch(PITCHSPEED);

        if (wx > 0.1)
            camera.roll(PITCHSPEED);
        else if (wx < -0.1)
            camera.roll(-PITCHSPEED);
    }   
}

// Special keyboard function,
// up and down arrows for pitching.
// left and right arrows for rolling
void special(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_LEFT:
            camera.roll(-ROLLSPEED);
            break;
        case GLUT_KEY_RIGHT:
            camera.roll(ROLLSPEED);
            break;
        case GLUT_KEY_UP:
            camera.pitch(PITCHSPEED);
            break;
        case GLUT_KEY_DOWN:
            camera.pitch(-PITCHSPEED);
            break;
    }
}

// Keyboard function
//
// -, +, and = control the sealevel
// f and c control the number of polygons on the mountain
// z and x control the size of the mountain
//
// spacebar stops the motion of the camera
// e gives a boost to the speed
// a and d roll
// w and s pitch
//
// q or ESC to quit
void keyboard(unsigned char key, int x, int y) {
   switch (key) {
        case '-':
            sealevel -= 0.001*mountainsize;
            break;
        case '+':
        case '=':
            sealevel += 0.001*mountainsize;
            break;
        case 'f':
            polysize *= 0.5;
            break;
        case 'c':
            polysize *= 2.0;
            break;
        case 'z':
            mountainsize -= 1.0;
            break;
        case 'x':
            mountainsize += 1.0;
            break;
        case 0x20:
            moving = !moving;
            break;
        case 'e':
            camera.move(10*FWDSPEED);
            break;
        case 'a':
            camera.roll(-ROLLSPEED);
            break;
        case 'd':
            camera.roll(ROLLSPEED);
            break;
        case 'w':
            camera.pitch(PITCHSPEED);
            break;
        case 's':
            camera.pitch(-PITCHSPEED);
            break;
        case 'q':
        case 27:
            exit(0);
   }
}

int main(int argc, char** argv) {
   glutInit(&argc, argv);

   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize(500, 500); 
   glutInitWindowPosition(100, 100);
   glutCreateWindow("OpenGL");

   init();

   glutDisplayFunc(display); 
   glutReshapeFunc(reshape);
   glutKeyboardFunc(keyboard);
   glutSpecialFunc(special);
   glutMouseFunc(mouse);
   glutTimerFunc(100,timer,nFPS);

   glutMainLoop();

   return 0;
}
