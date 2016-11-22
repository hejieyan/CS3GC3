#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
// #include "mathLibrary.h"

#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif


// MARK: Callback Functions
void display() {
	glClearColor(0,0,0,1); 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);

	glColor3f(1,0,0);
	glutWireTeapot(1.0);

	glutSwapBuffers();
}

int initCallbacks() {
	glutDisplayFunc(display);
}

int main(int argc,char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Modeller");

	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-2.0, 2.0, -2.0, 2.0, -2.0, 2.0);
	// gluPerspective(60, 1, 1, 10);

	initCallbacks();
	glutMainLoop();

	return 0;
}