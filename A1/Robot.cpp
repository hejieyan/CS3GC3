//opengl cross platform includes
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "mathLibrary.cpp"

#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif

int gridHeight = 0, gridWidth = 0;

void bresenham(Point2D p1, Point2D p2){
	
	bool vertical=false;

	int p1y = p1.y;
	int p1x = p1.x;
	int p2y = p2.y;
	int p2x = p2.x;

	if (abs(p2y - p1y) > abs(p2x - p1x)){ 
		std::swap(p1y, p1x);
		std::swap(p2y, p2x);
		vertical=true;
	}
	if (p1x > p2x){
		std::swap(p1x,p2x);
		std::swap(p1y,p2y);
	}
	//Bresenham's algorithm from slides
	int dx = p2x - p1x;
	int dy = abs(p2y - p1y);
	int d = 2*dy - dx;

	int incrX = dy*2;
	int incrXY = 2*dy - 2*dx;
	int x = p1x;
	int y = p1y;

	//Assign first point.
	if (vertical) 
		glVertex2i(y,x); //Switch Y and X coordinates, if they were swapped earlier

	else 
		glVertex2i(x,y);

	//Assign the rest of the points
	while (x < p2x){
		if (d <= 0){
			d += incrX;
			x += 1;
		}
		else {
			d += incrXY;
			x += 1;
			y += 1;
		}
		if (vertical) 
			glVertex2i(y,x);
		else 
			glVertex2i(x, y);
	}
}

int gridLines;
void drawGrid(){
	gridLines = 400/gridWidth;
	for(int k = 0; k <= ((400/gridHeight)*gridHeight); k = k + 400/gridHeight){
		Point2D beginline =  Point2D(k,0);
		Point2D endline =  Point2D(k, gridWidth*gridLines);
		glBegin(GL_POINTS);
			bresenham(beginline, endline);
		glEnd();
	}
	gridLines = 400/gridHeight;
	for(int k = 0; k <= ((400/gridWidth)*gridWidth); k = k + 400/gridWidth){
		Point2D beginline = Point2D(0,k);
		Point2D endline = Point2D(gridLines*gridHeight, k);
		glBegin(GL_POINTS);
			bresenham(beginline, endline);
		glEnd();
	}
}
/* display function - GLUT display callback function
 *		clears the screen, draws a square, and displays it
 */
float headOrientation = 0;
int robotPositionX = 0;
int robotPositionY = 0;
void resetRobot(bool rotates){
	srand ( time(NULL) );
	robotPositionX = (400/gridHeight) * (rand() % gridHeight);
	robotPositionY = (400/gridWidth) * (rand() % gridWidth);
	if(rotates)
		headOrientation = 0;

	//printf ("Grids: %d,%d\n", gridHeight, gridWidth);
	//printf ("Reset: %d,%d\n", robotPositionX, robotPositionY);

}
//OpenGL functions
int x, acty;
int printx, printy;
void fill(int x, int y){
	//int blx = x, int bly = y;
	//int brx = x+100, int bry = y;
	//int tlx = x, int tly = y+100;
	//int trx = x+100, int try = y+100;

	glBegin(GL_POINTS); 
		glColor3f(0,1,0);
		for (printx = x; printx <= x+100; x++){
			for (printy = y; printy <= y+100; y++){
				glVertex2f(printx,printy);
			}
		}
	glEnd();
}

void FPS(int val){
	glutPostRedisplay();
	glutTimerFunc(17, FPS, 0);//render every 17 ms
		//1sec = 1000, 60fps = 1000/60 =~17
}

void keyboard(unsigned char key, int xIn, int yIn)
{
	switch (key)
	{
		case 'q':
		case 27:	//27 is the esc key
			exit(0);
			break;

		case 'r':
			resetRobot(true);
			//printf("Resetted\n");
			break;

		case 't':
			resetRobot(false);
			break;

		case 'x':
			headOrientation -= 90;
			//printf("Rotate-\n");
			break;

		case 'c':
			headOrientation += 90;
			//printf("Rotate+\n");
			break;
	}
}

void special(int key, int x, int y){
	switch (key){
	case GLUT_KEY_RIGHT:
		gridLines = 400/gridHeight ;
		gridLines = gridLines * gridHeight;

		if(robotPositionX < gridLines-400/gridHeight) //Move RIGHT
			robotPositionX=robotPositionX + 400/gridHeight;
		break;

	case GLUT_KEY_UP:

		gridLines = 400/gridWidth ;
		gridLines = gridLines * gridWidth;

		if (robotPositionY < gridLines - 400/gridWidth)
			robotPositionY=robotPositionY+400/gridWidth;
		break;

	case GLUT_KEY_DOWN:
		if(robotPositionY > 0) //Move DOWN
			robotPositionY=robotPositionY - 400/gridWidth;
		break;
	
	case GLUT_KEY_LEFT:
		if(robotPositionX > 0) //Move LEFT
			robotPositionX= robotPositionX - 400/gridHeight;
		break;
	}
}

void callBackInit(){
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);
	//glutMouseFunc(mouse);
	//glutMotionFunc(motion);
	//glutPassiveMotionFunc(passive);
	//glutReshapeFunc(reshape);
	glutTimerFunc(0, FPS, 0);
}

void menuProc(int value){
	if (value == 1) {// && robotPositionY < 400-400/gridWidth){ //Move UP
		gridLines = 400/gridWidth ;
		gridLines = gridLines * gridWidth;
		//printf ("%d, %d, %d\n", robotPositionY, gridLines - 400/gridWidth, gridLines);
		if (robotPositionY < gridLines - 400/gridWidth)
			robotPositionY=robotPositionY+400/gridWidth;
	}

	if (value == 2 && robotPositionX > 0) //Move LEFT
		robotPositionX= robotPositionX - 400/gridHeight;

	if (value == 3 && robotPositionX < 400-400/gridHeight) //Move RIGHT
		robotPositionX=robotPositionX + 400/gridHeight;

	if (value == 4 && robotPositionY > 0) //Move DOWN
		robotPositionY=robotPositionY - 400/gridWidth;

	if (value == 5)//Rotate RIGHT
		headOrientation -= 90;
	if (value == 6)//Rotate LEFT
		headOrientation += 90;
}

void createOurMenu(){
	//int subMenu_id = glutCreateMenu(menuProc2);
	int subMenu_id = glutCreateMenu(menuProc);
	glutAddMenuEntry("Up", 1);
	glutAddMenuEntry("Left", 2);
	glutAddMenuEntry("Right", 3);
	glutAddMenuEntry("Down", 4);

	int subMenu2_id = glutCreateMenu(menuProc);
	glutAddMenuEntry("Right", 5);
	glutAddMenuEntry("Left", 6);

	int main_id = glutCreateMenu(menuProc);
	//glutAddMenuEntry("Move", 1);
	glutAddSubMenu("Move", subMenu_id);
	glutAddSubMenu("Turn", subMenu2_id);
	glutAttachMenu(GLUT_LEFT_BUTTON);


}


void display(void)
{	
	glClearColor(0,0,0,0);
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0,1,0);
	//glPointSize();

	drawGrid();

	// Drawing the robot;
	glPushMatrix();
		glTranslatef(robotPositionX,robotPositionY,0);
		glPushMatrix();
			glColor3f(0 ,1, 0);
			
			for (int i = 0; i <= 400/gridHeight; i++){
				Point2D bottomend = Point2D(i, 0);
				Point2D topend = Point2D(i, 400/gridWidth);
				glBegin(GL_POINTS);
					bresenham(bottomend, topend);
				glEnd();
				
			}
	glPopMatrix();

	int headOffsetX = 250/gridHeight, headOffsetY = -250/gridWidth;

	glTranslatef(headOffsetX,-headOffsetY,0);
	
	glPushMatrix(); //draw moon
		
		//glTranslatef(headOffsetX,headOffsetY,0);
		//printf("Angle: %f\n", headOrientation);
		
		glPushMatrix();
			glRotatef(headOrientation, 0, 0, 1);
			glTranslatef(headOffsetX, headOffsetY, 0);
			//headOrientation++;
			//glScalef(moon1Scale, moon1Scale, moon1Scale);
			////draw the blue  teapot
			glColor3f(1 ,0, 0);

			for (int tempi2 = 0; tempi2 <= 400/gridHeight; tempi2++){
				Point2D bottom2end = Point2D(tempi2, 0);
				Point2D top2end = Point2D(tempi2, 400/gridWidth);
				glBegin(GL_POINTS);
					bresenham(bottom2end, top2end);
				glEnd();
				
			}
		glPopMatrix();


	glPopMatrix();

	glPopMatrix();

	glFlush();
}


/* main function - program entry point */
int main(int argc, char** argv)
{
	bool valid = false;

	while (!valid)
	{
		valid = true;

		std::cout << "Enter width: ";
		std::cin >> gridHeight;

		if(std::cin.fail() || gridHeight > 50 || gridHeight < 10) 
		{
			std::cin.clear(); //This corrects the stream.
			std::cin.ignore(); //This skips the left over stream data.
			printf ("Please enter a valid input\n");
			valid = false; //The cin was not an integer so try again.
		}
	}
	valid = false;

	while (!valid)
	{
		valid = true; //Assume the cin will be an integer.

		std::cout << "Enter length: ";
		std::cin >> gridWidth;

		if(std::cin.fail() || gridWidth > 50 || gridWidth < 10) 
		{
			std::cin.clear(); //This corrects the stream.
			std::cin.ignore(); //This skips the left over stream data.
			printf ("Please enter a valid input\n");
			valid = false; //The cin was not an integer so try again.
		}
	}
	//gridHeight = gridHeight *100;
	//gridWidth=gridWidth*100;

	printf("\n/*****************************************************/\n");
	printf("Conway's Game of Life\n");
	printf("Instrutions: \n");
	printf(">>> Mouse left-click for robot options\n");
	printf(">>> Mouse right-click for menu options\n");
	printf(">>> Keyboard arrows to move robot in different directions\n");
	printf(">>> 'x' to turn robot counterclockwise'\n");
	printf(">>> 'c' to turn robot clockwise\n");
	printf(">>> 'r' to reset the robot direction\n");
	printf(">>> 't' reset the robot position\n");
	printf(">>> q to Quit\n");
	printf("\n/*****************************************************/\n");




	resetRobot(true);

	glutInit(&argc, argv);		//starts up GLUT
	glutInitWindowSize(400,400);
	glutInitWindowPosition(100, 100);

	glMatrixMode(GL_PROJECTION);


	glutCreateWindow("Robot");	//creates the window
	//glutDisplayFunc(display);	//registers "display" as the display callback function

	//gluOrtho2D(0,gridHeight,0,gridWidth);
	glutDisplayFunc(display);	//registers "display" as the display callback function
	float pwidth = 400;
	gluOrtho2D(0.0f,400.0f,0.0f,400.0f);
	drawGrid();
	callBackInit();
	createOurMenu();

	glutMainLoop();				//starts the event loop

	return(0);					//return may not be necessary on all compilers
}