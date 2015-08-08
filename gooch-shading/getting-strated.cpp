/*#ifdef WIN32
	#include <windows.h>
	#include <gl\gl.h>
	#include <gl\glu.h>
	#include "glut/glut.h"
#endif
#ifdef __APPLE__
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
	#include <Glut/glut.h>
#endif
#include "../math/math3d.h" // Taken from Super Bible's online resources

enum { RED, GREEN, BLUE, BLACK };
static M3DVector3f colors[6] = {{255,   0,   0},  // R		0
								{  0, 255,   0},  // G		1
								{  0,	0, 255},  // B		2
								{ 25,  25,  25}}; // Black	3


//=============================================================================

void drawAxis() {
	glBegin(GL_LINES);
		glColor3fv(colors[BLUE]);
		// Or, glColor3f(0.0f, 0.0f, 255.0f);
		glVertex3f(-150.0f,    0.0f, 0.0f);
		glVertex3f( 150.0f,    0.0f, 0.0f);
	glEnd();glBegin(GL_LINES);
		glVertex3f(   0.0f, -100.0f, 0.0f);
		glVertex3f(   0.0f,  100.0f, 0.0f);
	glEnd();
}
void drawRect() {
	M3DVector3f corners1[4] = { {-25,  25, 0},
								{-25, -25, 0},
								{ 25, -25, 0},
								{ 25,  25, 0}};
	M3DVector3f corners2[4] = { { -35, 35, 0},
								{ -35, 25, 0},
								{ -25, 25, 0},
								{ -25, 35, 0}};
	
	glBegin(GL_TRIANGLES);
		// You can make rectangles using GL_QUADS as well.
		// Depending on style preference you can have multiple triangles
		// declared in one GL_TRIANGLES block like this particular one,
		// or seperate them out like the lines in drawAxis() are defined.
		glColor3fv (colors[RED]);
		glVertex3fv(corners1[0]);
		glVertex3fv(corners1[1]);
		glVertex3fv(corners1[2]);
		
		glVertex3fv(corners1[3]);
		glVertex3fv(corners1[0]);
		glVertex3fv(corners1[2]);
	glEnd();
	glBegin(GL_TRIANGLES);
		glColor3fv (colors[GREEN]);
		glVertex3fv(corners2[0]);
		glVertex3fv(corners2[1]);
		glVertex3fv(corners2[2]);
		
		glVertex3fv(corners2[3]);
		glVertex3fv(corners2[0]);
		glVertex3fv(corners2[2]);
	glEnd();
}

void renderScene () {
	glClear(GL_COLOR_BUFFER_BIT);
	
	//drawRect();
	//drawAxis();
    glutSolidCube(5.0);
    gluLookAt(0, 0, 10, 0, 0, 0, 0, 1, 0);
    
	
	// Flsuh drawing commands (draws the images on the screen)
	glFlush();
}

void createView (int w, int h) {
	GLfloat fAspect = (GLfloat) w / (GLfloat) h; // Aspect ratio
	glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	
	// Setting up orthographic projection
    //if (w <= h) glOrtho (-100.0, 100.0, -100 / fAspect, 100.0 / fAspect, 100.0, -1.0);
	//else glOrtho (-100 * fAspect, 100.0 * fAspect, -100.0, 100.0, 100.0, -1.0);
    gluPerspective(45, fAspect, 1, 1000);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void setup () {
	// Set background to black
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f );
}

int main(int argc, char* argv[]) {	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(800,600);
	glutCreateWindow("Getting Started with 3D");
	
	glutReshapeFunc(createView);
	glutDisplayFunc(renderScene);
	setup();
	glutMainLoop();
	
	return 0;
}*/
