//opengl cross platform includes
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif

int gridSize = 40;
int citizens[40][40], citizens2[40][40], speedi = 1;

//OpenGL functions

void makeGrid(){
	glColor3f(1,1,1);

	glBegin(GL_LINES); 
		for (int x = 0; x <= gridSize-2; x++) {
			glVertex2f(x,0);
			glVertex2f(x,gridSize);
			glVertex2f(0, x);
			glVertex2f(gridSize, x);
		}
	glEnd();
}

void clear(){
	for (int i=0; i< gridSize; i++)
		for (int j=0; j<gridSize; j++)
			citizens[i][j] = 0;
}


void randomCitizens(){
	for (int i=0; i< gridSize; i++)
		for (int j=0; j<gridSize; j++){
			if (i == 0 || j == 0 || i == gridSize -1 || j == gridSize - 1)
				citizens[i][j] = 0;
			else 
				citizens[i][j] = rand() % 2;
		}
}

// Transfer from buffer to original citizens
void transferOriginal(){
	for (int i=0; i< gridSize; i++)
		for (int j=0; j<gridSize; j++)
			citizens2[i][j] = citizens[i][j];
}

void edgeFix(){
	for (int i=0; i< gridSize; i++){
		citizens[i][0] = 0;
		citizens[0][i] = 0;
		citizens[gridSize-1][i] = 0;
		citizens[0][gridSize-1] = 0;
	}
}

int neighbours = 0;
void conditionForLife(){

	transferOriginal();
	int debugneighbour = -1;

	for (int i=0; i< gridSize; i++)
		for (int j=0; j<gridSize; j++){
			neighbours = citizens2[i-1][j-1]
							+ citizens2[i][j-1]
							+ citizens2[i+1][j-1]
							+ citizens2[i-1][j]
							+ citizens2[i+1][j]
							+ citizens2[i-1][j+1]
							+ citizens2[i][j+1]
							+ citizens2[i+1][j+1];

			if (citizens2[i][j] == 1){ //ALIVE
				
				if (neighbours < 2 || neighbours > 3)
					citizens[i][j] = 0;
				else
					citizens[i][j] = 1;
			}

			else { //DEAD
				if (neighbours == 3)
					citizens[i][j] = 1;
				else 
					citizens[i][j] = 0;
			}

		}
	edgeFix();
}

void drawCitizens(){

	glPushMatrix();
		glColor3f(1 ,1, 0);
		glPointSize(9);

		glBegin(GL_POINTS);
		for (int i = 0; i < gridSize; i++){
			for (int j = 0; j < gridSize; j++)
				if (citizens[i][j]==1)
						glVertex2f(i-0.5, j-0.5);
		}
		glEnd();
	glPopMatrix();
}

void display(void)
{
	
	glClearColor(0,0,0,0);
	glClear(GL_COLOR_BUFFER_BIT);

	makeGrid();
	drawCitizens();
	glutSwapBuffers();
}

bool play = true;
int speed = 500;
int speedModes[] = {1000, 100};
void FPS(int val){
	if (play)
		conditionForLife();
	
	glutPostRedisplay();
	glutTimerFunc(speed, FPS, 0);
}

int mousePositionX, mousePositionY;
void mouse(int btn, int state, int x, int y){
	//printf("mouseFunc coords: %i,%i\n", x, y);
	if (btn == GLUT_LEFT_BUTTON){
		

		if (state == GLUT_UP){
			mousePositionX = x/10 +1;
			mousePositionY = gridSize - 2 - y/10;
			if (citizens[mousePositionX][mousePositionY] == 1)
				citizens[mousePositionX][mousePositionY] = 0;
			else
				citizens[mousePositionX][mousePositionY] = 1;
		}
	}
}

void keyboard(unsigned char key, int xIn, int yIn)
{
	switch (key)
	{
		case 'q':
		case 27:	//27 is the esc key
			exit(0);
			break;
		case 32:
			play = !play;
			break;
		case 'r':
			randomCitizens();
			break;
		case 'c':
			clear();
			break;
	}
}

void special(int key, int x, int y){
	switch (key){
	case GLUT_KEY_UP:		
		if(speedi !=2)
			speedi++;
		speed = speedModes[speedi];
		//printf("SPEED UP!%d\n", speedi);
		break;

	case GLUT_KEY_DOWN:
		if(speedi !=0)
			speedi--;
		speed = speedModes[speedi];
		//printf("SPEED DOWN! %d\n", speedi);
		break;
	
	case GLUT_KEY_LEFT:
		break;
	
	case GLUT_KEY_RIGHT:
		break;		
	}
}

void glutCallbacksInit(){
	glutDisplayFunc(display);	//registers "display" as the display callback function
	gluOrtho2D(0,gridSize-2,0,gridSize-2);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);
	glutMouseFunc(mouse);
	glutTimerFunc(0, FPS, 0);
}

void menuProc(int value){
	if (value == 1){ //RANDOMIZE
		randomCitizens();
	}

	if (value == 2) //CLEAR ALL
		clear();

	if (value == 3){ //PLAY / PAUSE
		play = !play;
	}

	if (value == 4) //QUIT
		exit(0);
		//printf(" ");

	if (value == 5){ //SPEED = SLOW
		speed = speedModes[0];
		speedi = 0;
		//printf("SLOW\n");
	}

	if (value == 6){ //SPEED = MEDIUM
		speed = speedModes[1];
		speedi = 1;
		//printf("MEDIUM\n");
	}
}

void createOurMenu(){
	//int subMenu_id = glutCreateMenu(menuProc2);
	int subMenu_id = glutCreateMenu(menuProc);
	glutAddMenuEntry("Slow", 5);
	glutAddMenuEntry("Fast", 6);


	int main_id = glutCreateMenu(menuProc);
	glutAddMenuEntry("Randomize", 1);
	glutAddMenuEntry("Clear Grid", 2);
	glutAddMenuEntry("Run/Pause", 3);
	// glutAddMenuEntry("Randomize", 3);
	glutAddSubMenu("Speed", subMenu_id);
	glutAddMenuEntry("Quit", 4);
	glutAttachMenu(GLUT_RIGHT_BUTTON);


}


/* main function - program entry point */
int main(int argc, char** argv)
{
	printf("\n/*****************************************************/\n");
	printf("Conway's Game of Life\n");
	printf("Instrutions: \n");
	printf(">>> Mouse left-click\n");
	printf(">>> Mouse right-click to access menu options\n");
	printf(">>> Keyboard arrow up to increase speed\n");
	printf(">>> Keyboard arrow down to decrease speed\n");
	printf(">>> 'c' to reset the grid\n");
	printf(">>> 'r' to randomly place citizens on grid\n");
	printf(">>> <space> to toggle between play and pause\n");
	printf(">>> q to Quit\n");
	printf("\n/*****************************************************/\n");


	clear();
	randomCitizens();
	//play = false;
	// citizens[3][3] = 1;
	// citizens[2][3] = 1;
	// citizens[3][2] = 1;
	// citizens[4][4] = 1;
	//conditionForLife();
	glutInit(&argc, argv);		//starts up GLUT
	glutInitWindowPosition(50, 50);
	glutInitWindowSize((gridSize-2)*10, (gridSize-2)*10);
	glutCreateWindow("Conway's Game of Life");	//creates the window
	
	glutCallbacksInit();
	createOurMenu();

	glutMainLoop();				//starts the event loop

	return(0);					//return may not be necessary on all compilers
}