#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include "mathLibrary.h"

#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif

// MARK: Global Variables
// Constant scene window size 
const int sceneLength = 500;
const int sceneHeight = 500;

// User input terrain space variables
int terrainLength;		// X-axis
int terrainWidth;		// Z-axis
int terrainMaxHeight;	// Maximum possible height for hills

// Points to the height map 2D array and mapped normalsArray 2D array
float** heightMap = NULL;
Vector3D** normalsArray = NULL;

// Camera Settings
float cameraPosition[] = {0, 0, 0};
float cameraUpVector[] = {0, 1, 0};
float cameraTarget[] = {0, 0, 0};
float cameraSpeedFactor = 5.0f;
float cameraRotationX = 0;
float cameraRotationY = 0;

// Viewing Modes represented as enum
enum ViewingModes {
	VIEW_POLYGON = 0,
	VIEW_WIREFRAME = 1,
	VIEW_POLYFRAME = 2
};
ViewingModes viewingMode = VIEW_POLYGON; //static_cast<ViewingModes>(0);

// Shading Modes represented as enum
enum ShadingModes {
	SHADING_FLAT = 0,
	SHADING_SMOOTH = 1
};
ShadingModes shadingMode = SHADING_FLAT; //static_cast<ShadingModes>(0);

// Shape Modes represented as enum
enum ShapeModes {
	SHAPE_QUAD_STRIPS = 0,
	SHAPE_TRIANGLE_STRIPS = 1
};
ShapeModes shapeMode = SHAPE_QUAD_STRIPS; //static_cast<ShapeModes>(0);

// Lighting settings
bool lightOn = true;
// First light properties
Light light0 = {
	{-1, 0, 0},		// Position
	{0.5, 0, 0, 1},	// Ambient
	{1, 1, 0, 1},	// Diffuse
	{1, 1, 0, 1}	// Specular
};
Material material0 = {
	{0.33, 0.22, 0.03, 1.0},	// Ambient
	{0.78, 0.57, 0.11, 1.0},	// Diffuse
	{0.99, 0.91, 0.81, 1.0},	// Specular
	{0, 0, 0, 1},				// Emission
	27.8						// Shininess
};
// Second light properties
Light light1 = {
	{0, 0, -1},		// Position
	{0, 0, 0, 1},	// Ambient
	{0, 1, 1, 1},	// Diffuse
	{0, 1, 1, 1}	// Specular
};
Material material1 = {
	{0.33, 0.22, 0.03, 1.0},	// Ambient
	{0.78, 0.57, 0.11, 1.0},	// Diffuse
	{0.99, 0.91, 0.81, 1.0},	// Specular
	{0, 0, 0, 1},				// Emission
	100.8						// Shininess
};

// Instructions
void showInstructions() {
	printf("\n\n~~~TERRAIN MODELLER~~~\n"
		"This terrain modeller generates random 3D terrains according to the circle algorithm. \n"
		"Please enter the desired dimensions when prompt.\n\n"
		"The program supports the following commands: \n"
		"\t arrow keys - rotate camera around the x and y axes \n"
		"\t q - toggle between flat and Gouraud shading \n"
		"\t w - change between filled, wireframe, or filled + wireframe terrain view \n"
		"\t e - toggle lighting on or off \n"
		"\t r - generates a new random terrain \n"
		"\t t - toggle between triangle strips and quad strips\n\n"
		"Improved camera commands: \n"
		"\t z, c - rotate camera in the X-axis\n"
		"\t a, d - rotate camera in the Y-axis\n"
		"\t s, x - rotate camera in the Z-axis\n"
		"\t i, k - move viewport up/down\n"
		"\t j, l - move viewport left/right\n"
		"\t u, o - move viewport closer/farther\n"
		);
}

// Requests dimensions of the terrain space
// Ensures user inputs are in the valid range
void getUserInput() {
	do {
		std::cout << "Please enter the desired terrain length [50 - 300]: ";
		std::cin >> terrainLength;
	} while (terrainLength < 50 || terrainLength > 300);

	do {
		std::cout << "Please enter the desired terrain width [50 - 300]: ";
		std::cin >> terrainWidth;		
	}
	while (terrainWidth < 50 || terrainWidth > 300);

	do {
		std::cout << "Please enter the desired maximum terrain height [1 - 20]: ";
		std::cin >> terrainMaxHeight;
	}
	while (terrainMaxHeight < 1 || terrainMaxHeight > 20);
}

// MARK: Initialization Functions
// Circles algorithm for creating heightMap in height map
void circlesAlgorithm (float** heightMap, float centerX, float centerZ, float radius, float displacement) {
	// Cycles through each index of the heightMap array and stores a random height based on Circles Algorithm
	for (int z = 0; z < terrainWidth; z++) {
		for (int x = 0; x < terrainLength; x++) {
			float distanceFromCircleCenter = sqrt(pow(centerZ - z, 2) + pow(centerX - x, 2));
			float terrainCircleSize = radius * 2; // The diameter of the circle
			float pb = distanceFromCircleCenter * 2 / terrainCircleSize;
			if (abs(pb) < 1.0) {
				heightMap[x][z] += displacement + cos(pb * 3.14) * displacement;
			}

		}
	}

	// for (int x = 0; x < terrainWidth; x++) {
	// 	for (int z = 0; z < terrainLength; z++) {
	// 		float distanceFromCircleCenter = sqrt(pow(centerZ - z, 2) + pow(centerX - x, 2));
	// 		float terrainCircleSize = radius * 2; // The diameter of the circle
	// 		float pb = distanceFromCircleCenter * 2 / terrainCircleSize;
	// 		if (abs(pb) < 1.0) {
	// 			heightMap[x][z] += displacement + cos(pb * 3.14) * displacement;
	// 		}

	// 	}
	// }
}

// Creates randomly generated height map based on user's inputs
// TODO: MORE EFFICIENT METHOD 
// http://stackoverflow.com/questions/936687/how-do-i-declare-a-2d-array-in-c-using-new
void initHeightMap() {
	// Points to the height map 2D array
	heightMap = new float*[terrainLength]; // X-by-Z Array
	for (int x = 0; x < terrainLength; x++) {
		heightMap[x] = new float[terrainLength];
		for (int z = 0; z < terrainLength; z++) {
			heightMap[x][z] = 0;
		}
	}

	// Random values
	float centerX;
	float centerZ;
	float radius;
	float displacement;

	// Random numbers are proportional to the size of the terrain
	int numberOfHills = rand() % ((terrainLength*terrainWidth)/200) + 5;
	for (int i = 0; i < numberOfHills; i++) {
		centerX = rand() % terrainWidth;
		centerZ = rand() % terrainLength;
		radius = rand() % 10 + 10;
		displacement = rand() % terrainMaxHeight;
		// Perform the Circles Algorithm on the heightMap based on the random numbers
		circlesAlgorithm(heightMap, centerX, centerZ, radius, displacement);
	}
}

// Computers the normals for each index of the heightMap
void computeNormals() {
	normalsArray = new Vector3D*[terrainWidth];
	for (int x = 0; x < terrainLength; x++) {
		normalsArray[x] = new Vector3D[terrainLength];

		for (int z = 0; z < terrainWidth; z++) {
			// Vertex Normal Vector3D at (x, z)
			Vector3D* vertexNormal = new Vector3D();

			// Creates the vectors to be used in the calculation for the Normal Vector
  			Vector3D *edge1;
  			if (x < (terrainLength-1)) edge1 = vBetween(new Point3D(x,heightMap[x][z],z),new Point3D(x+1,heightMap[x+1][z],z));
  			Vector3D *edge2;
  			if (z < (terrainWidth-1)) edge2 = vBetween(new Point3D(x,heightMap[x][z],z),new Point3D(x,heightMap[x][z+1],z+1));
  			Vector3D *edge3;
  			if (x>0) edge3 = vBetween(new Point3D(x,heightMap[x][z],z),new Point3D(x-1,heightMap[x-1][z],z));
  			Vector3D *edge4;
  			if (z>0) edge4 = vBetween(new Point3D(x,heightMap[x][z],z),new Point3D(x,heightMap[x][z-1],z-1));

  			// Gets the normals for the vectors and adds it to vertexNormal
  			if (x < (terrainLength-1) && z < (terrainWidth-1)) vertexNormal->add(edge1->crossProduct(edge2));
  			if (x > 0 && z < (terrainWidth-1)) vertexNormal->add(edge2->crossProduct(edge3));
  			if (x > 0 && z > 0) vertexNormal->add(edge3->crossProduct(edge4));
  			if (x < (terrainLength-1) && z > 0) vertexNormal->add(edge4->crossProduct(edge1));

  			// 4) Normalizes the vertexNormal vector
  			vertexNormal->normalize();
  			normalsArray[x][z] = *vertexNormal;
		}

	}
}

// Sets up the initial scene by initializing the camera 
void initCamera() {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	cameraPosition[0] = terrainLength*2.5;
	cameraPosition[1] = terrainWidth;
	cameraPosition[2] = terrainWidth/2;

	gluLookAt(cameraPosition[0], cameraPosition[1], cameraPosition[2],  
			cameraTarget[0], cameraTarget[1], cameraTarget[2], 
			cameraUpVector[0], cameraUpVector[1], cameraUpVector[2]);
	glRotatef(cameraRotationY,1,0,0);
	glRotatef(cameraRotationX,0,1,0);
	glTranslatef(-terrainLength/2, 0, -terrainLength/2);

}

// Lighting Functions
void enableLighting() {
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);

	// Sets lighting and material parameters using helper functions
	setLighting(light0, 0);
	// setMaterial(material0);

	setLighting(light1, 1);
	// setMaterial(material1);
}

// Disables Lighting
void disableLighting() {
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHT1);
}

// Drawing functions
void drawTerrain() {
	// Filled polygon mode
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	switch (shapeMode) {
		// Draws the terrain using quad strips
		case SHAPE_QUAD_STRIPS:
			glBegin(GL_QUADS);
			for (int x = 0; x < terrainWidth - 1; x++) {
				for (int z = 0; z < terrainLength - 1; z++) {
					// Colour assignment
					float colour1 = heightMap[x][z]/terrainMaxHeight;
					float colour2 = heightMap[x+1][z]/terrainMaxHeight;
					float colour3 = heightMap[x+1][z+1]/terrainMaxHeight;
					float colour4 = heightMap[x][z+1]/terrainMaxHeight;

					// Cycling through vertex counter-clockwise
					glColor3f(colour1, colour1, colour1);
					glNormal3f(normalsArray[x][z].x,normalsArray[x][z].y,normalsArray[x][z].z);
					glVertex3f(x, heightMap[x][z], z);

					glColor3f(colour2, colour2, colour2);
					glNormal3f(normalsArray[x+1][z].x,normalsArray[x+1][z].y,normalsArray[x+1][z].z);
					glVertex3f(x+1, heightMap[x+1][z], z);

					glColor3f(colour3, colour3, colour3);
					glNormal3f(normalsArray[x+1][z+1].x,normalsArray[x+1][z+1].y,normalsArray[x+1][z+1].z);
					glVertex3f(x+1, heightMap[x+1][z+1], z+1);

					glColor3f(colour4, colour4, colour4);
					glNormal3f(normalsArray[x][z+1].x,normalsArray[x][z+1].y,normalsArray[x][z+1].z);
					glVertex3f(x, heightMap[x][z+1], z+1);
				}
			}
			glEnd();
			break;

		case SHAPE_TRIANGLE_STRIPS:
			// Draws the terrain using triangle strips
			glBegin(GL_TRIANGLE_STRIP);
			for (int x = 0; x < terrainWidth - 1; x++) {
				for (int z = 0; z < terrainLength - 1; z++) {
					// Colour assignment
					float colour1 = heightMap[x][z]/terrainMaxHeight;
					float colour2 = heightMap[x+1][z]/terrainMaxHeight;
					float colour3 = heightMap[x+1][z+1]/terrainMaxHeight;
					float colour4 = heightMap[x][z+1]/terrainMaxHeight;

					// Cycling through vertex counter-clockwise
					glColor3f(colour1, colour1, colour1);
					glNormal3f(normalsArray[x][z].x,normalsArray[x][z].y,normalsArray[x][z].z);
					glVertex3f(x, heightMap[x][z], z);

					glColor3f(colour2, colour2, colour2);
					glNormal3f(normalsArray[x+1][z].x,normalsArray[x+1][z].y,normalsArray[x+1][z].z);
					glVertex3f(x+1, heightMap[x+1][z], z);

					glColor3f(colour4, colour4, colour4);
					glNormal3f(normalsArray[x][z+1].x,normalsArray[x][z+1].y,normalsArray[x][z+1].z);
					glVertex3f(x, heightMap[x][z+1], z+1);


					glColor3f(colour2, colour2, colour2);
					glNormal3f(normalsArray[x+1][z].x,normalsArray[x+1][z].y,normalsArray[x+1][z].z);
					glVertex3f(x+1, heightMap[x+1][z], z);
				}
			}
			glEnd();
			break;
	}

}
// Function to draw the Wireframe version of the terrain, disables lighting
void drawWireframe() {
	//enable lines mode, disables lighting
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	disableLighting();

	//drawing with quads
	glBegin(GL_QUADS);
		glColor3f(0,1,0);
		for (int x = 0; x < terrainWidth - 1; x++) { 
			for (int z = 0; z < terrainLength - 1; z++) {

				//draw quad vertices CCW
				glVertex3f(x, heightMap[x][z], z);
				glVertex3f(x+1, heightMap[x+1][z], z);
				glVertex3f(x+1, heightMap[x+1][z+1], z+1);
				glVertex3f(x, heightMap[x][z+1], z+1);
			}
		}
	glEnd();

	// glBegin(GL_POINTS);
	// 	glColor3f(1,1,1);
	// 	glVertex3f(0, 0, 0);
	// 	glColor3f(1,0,0);
	// 	glVertex3f(terrainLength, 0, 0);
	// 	glColor3f(0,1,0);
	// 	glVertex3f(0, 0, terrainWidth);
	// 	glColor3f(0,0,1);
	// 	glVertex3f(terrainLength, 0, terrainWidth);
	// glEnd();
}

// MARK: Callback Functions
void display() {
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(cameraPosition[0], cameraPosition[1], cameraPosition[2],  
			cameraTarget[0], cameraTarget[1], cameraTarget[2], 
			cameraUpVector[0], cameraUpVector[1], cameraUpVector[2]);
	glRotatef(cameraRotationY,1,0,0);
	glRotatef(cameraRotationX,0,1,0);

	// glPointSize(5);
	// glBegin(GL_POINTS);
	// 	glColor3f(1,0,0);
	// 	for (int x = -100; x < 100; x ++) {
	// 		glVertex3f(x,0,0);
	// 	}
	// 	glColor3f(0,1,0);
	// 	for (int y = -100; y < 100; y ++) {
	// 		glVertex3f(0,y,0);
	// 	}
	// 	glColor3f(0,0,1);
	// 	for (int z = -100; z < 100; z ++) {
	// 		glVertex3f(0,0,z);
	// 	}
	// glEnd();

	switch (lightOn) {
		case true:
			enableLighting();
			break;
		case false:
			disableLighting();
			break;
	}

	switch (shadingMode) {
		case SHADING_FLAT:
			glShadeModel(GL_FLAT);
			break;
		case SHADING_SMOOTH:
			glShadeModel(GL_SMOOTH);
			break;
	}

	switch (viewingMode) {
		case VIEW_POLYGON:
			drawTerrain();
			break;

		case VIEW_WIREFRAME:
			drawWireframe();
			break;

		case VIEW_POLYFRAME:
			drawTerrain();
			drawWireframe();
			break;
	}

	glutSwapBuffers();
}

void keyboard(unsigned char key, int xIn, int yIn)
{
	int mod = glutGetModifiers();
	switch (key)
	{
		// ESC: to exit program
		case 27:	//27 is the esc key
			exit(0);
			break;

		// q: to toggle between Smooth and Flat shading
		case 'q':
			switch (shadingMode) {
				case SHADING_FLAT:
					shadingMode = SHADING_SMOOTH;
					break;
				case SHADING_SMOOTH:
					shadingMode = SHADING_FLAT;
					break;
			}
			break;

		// w: Toggles between polygon, wireframe, or both drawings
		case 'w':
			// TODO: Make increment function for enums
			switch (viewingMode) {
				case VIEW_POLYGON:
					viewingMode = VIEW_WIREFRAME;
					break;
				case VIEW_WIREFRAME:
					viewingMode = VIEW_POLYFRAME;
					break;
				case VIEW_POLYFRAME:
					viewingMode = VIEW_POLYGON;
					break;
			}
			break;

		// e: Toggle light On/Off
		case 'e':
			switch (lightOn) {
				case false:
					lightOn = true;
					enableLighting();
					break;
				case true:
					lightOn = false;
					disableLighting();
					break;
			} 
			break;

		// r: Initialize new terrain heightMap with normals and redraw the scene
		case 'r':
			// Delete allocated pointers and re-initialize
			for(int i = 0; i < terrainWidth; ++i) {
			    delete [] heightMap[i];
			}
			delete [] heightMap;
			initHeightMap();
			computeNormals();
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			break;

		// t: Toggle QUAD/TRIANGLE Shapes
		case 't':
			switch (shapeMode) {
				case SHAPE_QUAD_STRIPS:
					shapeMode = SHAPE_TRIANGLE_STRIPS;
					break;
				case SHAPE_TRIANGLE_STRIPS:
					shapeMode = SHAPE_QUAD_STRIPS;
					break;
			}
			break;

		//camera control
		case 's':
			cameraPosition[0] += cameraSpeedFactor;
			break;
		case 'z':
			cameraPosition[1] -= cameraSpeedFactor;
			break;
		case 'x':
			cameraPosition[0] -= cameraSpeedFactor;
			break;
		case 'c':
			cameraPosition[1] += cameraSpeedFactor;
			break;
		case 'a':
			cameraPosition[2] -= cameraSpeedFactor;
			break;
		case 'd':
			cameraPosition[2] += cameraSpeedFactor;
			break;


		//move camera target
		case 'i':
			cameraTarget[0] += cameraSpeedFactor;
			break;
		case 'j':
			cameraTarget[1] -= cameraSpeedFactor;
			break;
		case 'k':
			cameraTarget[0] -= cameraSpeedFactor;
			break;
		case 'l':
			cameraTarget[1] += cameraSpeedFactor;
			break;
		case 'u':
			cameraTarget[2] -= cameraSpeedFactor;
			break;
		case 'o':
			cameraTarget[2] += cameraSpeedFactor;
			break;
	}
	glutPostRedisplay();
}

void special(int key, int x, int y){
  	switch(key){

    	 case GLUT_KEY_LEFT:
    	 	cameraRotationX -= 10;
        	break;

     	 case GLUT_KEY_RIGHT:
     	 	cameraRotationX += 10;
        	break;

      	case GLUT_KEY_UP:
      		cameraRotationY += 10;
        	break;

      	case GLUT_KEY_DOWN:
      		cameraRotationY -= 10;
          	break;

        // case GLUT_KEY_PAGE_UP:
        // 	if (editRadius>=5){
        // 		editRadius += 5;
        // 	}
        // 	break;

        // case GLUT_KEY_PAGE_DOWN:
        // 	if (editRadius <=mapSize){
        // 		editRadius -= 5;
        // 	}
        // 	break;

	}
	// TODO: Swap Buffer instead of redisplay for efficiency
	glutPostRedisplay();
}

// void FPS(int val){
// 	glutPostRedisplay();
// 	glutTimerFunc(17, FPS, 0); // 1sec = 1000, 60fps = 1000/60 = ~17
// }

void initFunc() {
	initCamera(); // Function to setup camera
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);
	// glutTimerFunc(0,FPS,0);
}

int main(int argc, char** argv) {
	showInstructions();
	getUserInput();
	initCamera();
	srand(time(NULL)); //Allows for a random seed based on the time; different every time
	initHeightMap();
	computeNormals();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(50, 50);
	glutCreateWindow("Terrain");

	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, 1, 1, 2000);

	initFunc();
	glutMainLoop();

	return 0;
}