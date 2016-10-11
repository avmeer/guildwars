/*
 *  CSCI 441, Computer Graphics, Fall 2016
 *
 *  Project: A4
 *  File: main.cpp
 *
 *	Author: Stephen Tracy - Fall 2016
 *
 *  Description:
 *     Contains code for a vehicle for my character, with passive animation, animation based on keyboard input_iterator
 *     And utilizes an arcball style camera centered on the vehicle. User can change car heading and position with WSAD
 *     And the camera can be moved by clicking and dragging. Zooming is adjusted by ctrl clicking and dragging
 *		
 *	   Additionally, there is now a bezier curve and control grid displayed around the vehicle, with a sprite continuously moving
 *	   along this curve. The sprite is animated itself and always moves regardlesss of car movement, but the grid,curves and sprite
 *	   move with the car. Viewing the grid and the bezier curve can be toggled on and off via right clicking.
 *
 */

#ifdef __APPLE__			// if compiling on Mac OS
	#include <GLUT/glut.h>
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
#else					// else compiling on Linux OS
	#include <GL/glut.h>
	#include <GL/gl.h>
	#include <GL/glu.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h> 

// C++ Libraries we'll use
#include <fstream>			// we'll use ifstream	
#include <string>			// for, well strings!
#include <vector>			// and vectors (the storage container, not directional)
using namespace std;

#include <GL/glui.h>			// include our GLUI header

//user defined header files
#include "ArcballCamera.h"
#include "Car.h"
#include "BezierCurve.h"
#include "Sprite.h"
#include "BezierPatch.h"

// GLOBAL VARIABLES ////////////////////////////////////////////////////////////

//variables for window
static size_t windowWidth  = 640;
static size_t windowHeight = 480;
static float aspectRatio;

// status of the mouse buttons
GLint leftMouseButton, rightMouseButton;   
// last known X and Y of the mouse
int mouseX = 0, mouseY = 0;

//Player object
Car myCar = Car();
//sprite object rotating around myCar
Sprite mySprite = Sprite();
//Arcball camera object, looking at player object location
ArcballCamera myArcballCamera = ArcballCamera(myCar.getX(), myCar.getY(), myCar.getZ());

//collection of all control points which will be made into Bcurves
vector<Point> controlPoints;
//collection of bezier curves (consist of 4 points each)
vector<BezierCurve> bezierCurves;

vector<Point> bezPatchPoints;

BezierPatch myBezPatch;

//array of keys to keep track of which keys are currently pressed
bool keys[256];

// display list for the 'city' around the car
GLuint environmentDL; 
// menu identifier
GLint menuId;

//globals for animating sprite along curves
unsigned int whichCurve = 0;
float interpolantValue = 0.0f;
int resolution = 128;
float interpolantStepSize = 1.0f/resolution;

// getRand() ///////////////////////////////////////////////////////////////////
//
//  Simple helper function to return a random number between 0.0f and 1.0f.
//
////////////////////////////////////////////////////////////////////////////////
float getRand() {
   	return rand() / (float)RAND_MAX;
}

// drawGrid() //////////////////////////////////////////////////////////////////
//
//  Function to draw a grid in the XZ-Plane using OpenGL 2D Primitives (GL_LINES)
//
////////////////////////////////////////////////////////////////////////////////
void drawGrid() {
    /*
     *	We will get to why we need to do this when we talk about lighting,
     *	but for now whenever we want to draw something with an OpenGL
     *	Primitive - like a line, quad, point - we need to disable lighting
     *	and then reenable it for use with the GLUT 3D Primitives.
     */
    glDisable( GL_LIGHTING );

    //DRAW A GRID IN THE XZ-PLANE USING GL_LINES
	for (int i = -50; i <= 50; i++ ){
		//randomize color of the lines
		glColor3f(getRand(), getRand(), getRand());
		glBegin(GL_LINES);
		//draw Z lines
		glVertex3f(i,0,-50);
		glVertex3f(i,0,50);
		
		//draw X lines
		glVertex3f(-50,0,i);
		glVertex3f(50,0,i);
		glEnd();
	}
	/*
     *	As noted above, we are done drawing with OpenGL Primitives, so we
     *	must turn lighting back on.
     */
    glEnable( GL_LIGHTING );
}

// drawCity() //////////////////////////////////////////////////////////////////
//
//  Function to draw a random city using GLUT 3D Primitives
//
////////////////////////////////////////////////////////////////////////////////
void drawCity() {
    //Randomly place buildings of varying heights with random colors
	for (int i = -50; i <= 50; i++){
		for (int j = -50; j <= 50; j++){
			if (i % 2 == 0 && j%2 == 0 && getRand() < 0.025){
				glColor3f(getRand(), getRand(), getRand());
				glPushMatrix();
				float futureHeight = getRand() * 10;//get random number between 1 and 10
				while (futureHeight < 1){futureHeight = getRand() * 10;}//make sure not below 1 since getRand can return below 0.1
				glTranslatef(i,futureHeight / 2,j);
				glScalef(1,futureHeight,1); //change heights of cubes placed
				glutSolidCube( 1 );
				glPopMatrix();
			}
		}
	}
}

// generateEnvironmentDL() /////////////////////////////////////////////////////
//
//  This function creates a display list with the code to draw a simple 
//      environment for the user to navigate through.
//
//  And yes, it uses a global variable for the display list.
//  I know, I know! Kids: don't try this at home. There's something to be said
//      for object-oriented programming after all.
//
////////////////////////////////////////////////////////////////////////////////
void generateEnvironmentDL() {
    //Create a Display List & Call our Drawing Functions
    environmentDL = glGenLists( 1 );
	glNewList( environmentDL, GL_COMPILE );
	drawCity();
	
	float stepSize = 0.05;
	int numCurvePoints = floor(1.0f / stepSize);
	
	glPushMatrix();
	glTranslatef(-50,0,-50);
	//glScalef(100,0,100);
	for (float u = 0; u <= 1.0; u+= stepSize){
			//fprintf(stdout, "%f", u);
			myBezPatch.populateBezierPatch(u, stepSize);
	}
	
	vector<Point>* bezPoints = myBezPatch.getCurvePoints();
	
	//fprintf(stdout, "%d", bezPoints->size());
	
	
	glDisable(GL_LIGHTING);
	glColor3ub(153,0,0);
	for (unsigned int j = 0; j < (*bezPoints).size() - numCurvePoints; j+=numCurvePoints){
		glBegin(GL_QUAD_STRIP);
		for (int i = 0; i < numCurvePoints; i++){
			glVertex3f((*bezPoints)[j + i].getX(),(*bezPoints)[j + i].getY(),(*bezPoints)[j + i].getZ());
			glVertex3f((*bezPoints)[j + i + numCurvePoints].getX(),(*bezPoints)[j + i + numCurvePoints].getY(),(*bezPoints)[j + i + numCurvePoints].getZ());
		}
		glEnd();
	}
	
	glEnable(GL_LIGHTING);
	glPopMatrix();
	
	drawGrid();
	glEndList();
}


// resizeWindow() //////////////////////////////////////////////////////////////
//
//  GLUT callback for window resizing. Resets GL_PROJECTION matrix and viewport.
//
////////////////////////////////////////////////////////////////////////////////
void resizeWindow(int w, int h) {
    aspectRatio = w / (float)h;

    windowWidth = w;
    windowHeight = h;

    //update the viewport to fill the window
    glViewport(0, 0, w, h);

    //update the projection matrix with the new window properties
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0,aspectRatio,0.1,100000);
}



// mouseCallback() /////////////////////////////////////////////////////////////
//
//  GLUT callback for mouse clicks. We save the state of the mouse button
//      when this is called so that we can check the status of the mouse
//      buttons inside the motion callback (whether they are up or down).
//
////////////////////////////////////////////////////////////////////////////////
void mouseCallback(int button, int state, int thisX, int thisY) {
    // update the left mouse button states, if applicable
    if(button == GLUT_LEFT_BUTTON){
        leftMouseButton = state;
		mouseX = thisX;
		mouseY = thisY;
	}
}

// mouseMotion() ///////////////////////////////////////////////////////////////
//
//  GLUT callback for mouse movement. We update cameraPhi, cameraTheta, and/or
//      cameraRadiusScale based on how much the user has moved the mouse in the
//      X or Y directions (in screen space) and whether they have held down
//      the left or right mouse buttons. If the user hasn't held down any
//      buttons, the function just updates the last seen mouse X and Y coords.
//
////////////////////////////////////////////////////////////////////////////////
void mouseMotion(int x, int y) {
	//first check if the left mouse button is being held down
    if(leftMouseButton == GLUT_DOWN) {
		
		//then check whether or not control is being held
		if (glutGetModifiers() == GLUT_ACTIVE_CTRL){
			//call arcball zoom function
			myArcballCamera.handleZoom(mouseX, x, mouseY, y);
		}
		
		//if control was not being held during click/drag
		else{
			//call moving arcball function
			myArcballCamera.handleCameraDrag(mouseX, x, mouseY, y);
		}
		//store last position of mouse
		mouseX = x;
		mouseY = y;
	glutPostRedisplay();	    // redraw our scene from our new camera POV
    }
}



// initScene() /////////////////////////////////////////////////////////////////
//
//  A basic scene initialization function; should be called once after the
//      OpenGL context has been created. Doesn't need to be called further.
//
////////////////////////////////////////////////////////////////////////////////
void initScene()  {
    glEnable(GL_DEPTH_TEST);

    //******************************************************************
    // this is some code to enable a default light for the scene;
    // feel free to play around with this, but we won't talk about
    // lighting in OpenGL for another couple of weeks yet.
    float lightCol[4] = { 1, 1, 1, 1};
    float ambientCol[4] = { 0.0, 0.0, 0.0, 1.0 };
    float lPosition[4] = { 10, 10, 10, 1 };
    glLightfv( GL_LIGHT0, GL_POSITION,lPosition );
    glLightfv( GL_LIGHT0, GL_DIFFUSE,lightCol );
    glLightfv( GL_LIGHT0, GL_AMBIENT, ambientCol );
    glEnable( GL_LIGHTING );
    glEnable( GL_LIGHT0 );
	
    // tell OpenGL not to use the material system; just use whatever we 
    // pass with glColor*()
    glEnable( GL_COLOR_MATERIAL );
    glColorMaterial( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );
    //******************************************************************

    glShadeModel(GL_FLAT);

    srand( time(NULL) );	// seed our random number generator
    generateEnvironmentDL();
}

void drawScene(){
	//call display list so dont have to recompute each time
	glCallList( environmentDL );
	
	//draw the car! translate/rotate based on input
    glPushMatrix(); {
		//translate the car to current position (which is changed by user input)
		glTranslatef(myCar.getX(),myCar.getY(),myCar.getZ());
		//adjust cars heading (updated in timer via user input)
		glRotatef(myCar.getTheta(),0,1,0);
	    //myCar.drawCar();
		
		//want the curve/sprite to be drawn around car
		glPushMatrix();
		//get a point on the bezier curves based on an interpolated value from 0 to 1 and which curve
		//both are updated in timer
		Point point = bezierCurves[whichCurve].evaluateBezierCurve((float) interpolantValue - whichCurve);
		//translate the sprite to a position along the bezier curves
		glTranslatef(point.getX(),point.getY(),point.getZ());
		mySprite.drawSprite();
		glPopMatrix();
		
		//for each bezier curve, draw the control points, connect the control points, and draw the bezier curve
		//note that whether or not this gets actually drawn is done internally with bools that are updated
		//via the menu items created in myMenu
		for (unsigned int i = 0; i < bezierCurves.size(); i++){
				bezierCurves[i].drawControlPoints();
				bezierCurves[i].connectControlPoints();
				bezierCurves[i].renderBezierCurve();
		}
		
		
    }; glPopMatrix();
}

// renderScene() ///////////////////////////////////////////////////////////////
//
//  GLUT callback for scene rendering. Sets up the modelview matrix, renders
//      a scene to the back buffer, and switches the back buffer with the
//      front buffer (what the user sees).
//
////////////////////////////////////////////////////////////////////////////////
void renderScene(void)  {
	//clear the render buffer
    glDrawBuffer( GL_BACK );
	glDisable(GL_SCISSOR_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//set the width and height of the camera to fill the screen
	glViewport(0, 0, windowWidth, windowHeight);
	
    //update the modelview matrix based on the camera's position
    glMatrixMode(GL_MODELVIEW);              //make sure we aren't changing the projection matrix!
    glLoadIdentity();
	
	//call camerainfo getter for arcball camera object
	float* c = myArcballCamera.getCameraInfo();
	//c0-c2 are camera position + object position (xyz)
	//c3-c5 are object position xyz (camera keeps track and updates accordingly in timer)
	//c6-c8 are the xyz of the up vector (0,1,0) 
	gluLookAt(c[0], c[1], c[2], c[3], c[4], c[5], c[6], c[7], c[8]);

	drawScene();
	
	
	//Clear the way for the second camera
	glClear(GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, windowWidth/3, windowHeight/3);
	glScissor(0, 0, windowWidth/3, windowHeight/3);
	glEnable(GL_SCISSOR_TEST);
	glClear(GL_COLOR_BUFFER_BIT);
	
	glMatrixMode(GL_MODELVIEW); 
	glLoadIdentity();
    gluLookAt(0, 150, 0, 0, 0, 0, 1.0, 0.0, 0.0);
	
	glRotatef(-myCar.getTheta(), 0, 1, 0);
	glTranslatef(-myCar.getX(), 0, -myCar.getZ());
	
	drawScene();

    //push the back buffer to the screen
    glutSwapBuffers();
}

// normalKeysDown() ////////////////////////////////////////////////////////////
//
//  GLUT keyboard callback; gets called when the user presses a key.
//
////////////////////////////////////////////////////////////////////////////////
void normalKeysDown(unsigned char key, int x, int y) {
	//keys for quitting application
    if(key == 'q' || key == 'Q' || key == 27)
        exit(0);
	
	//keep track of which buttons are being pressed in the keys array
	//array used so multiple button presses can be done together (rotate + move)
	if(key == 'w'){keys['w'] = true;}
	if(key == 's'){keys['s'] = true;}
	if (key == 'a'){keys['a'] = true;}
	if (key == 'd'){keys['d'] = true;}
	
    glutPostRedisplay();		// redraw our scene from our new camera POV
}

//determine which keys are released
void keyboard_up(unsigned char key, int x, int y){
	//update keys array to reflect which keys are not being pressed anymore
	//update motion booleans
	if (key == 'w'){keys['w'] = false; myCar.setForwardCarMotion(false);}
	if (key == 's'){keys['s'] = false; myCar.setBackwardsCarMotion(false);}
	if (key == 'a'){keys['a'] = false;}
	if (key == 'd'){keys['d'] = false;}
}

//timer function handles actions done by button presses,
//updating passive motion variables
//and updating input based variables for animation
void myTimer(int value){
	//get current heading of car, let car object handle updating
	myCar.updateHeading();
	
	//check keys and update position
	//doing in timer for multiple keys being held down 
	//make sure to update the camera variables for object that it is looking at by passing in the camera
	//move forward W is being pressed
	if (keys['w'] == true){
		myCar.handleWKey();
		myArcballCamera.setObjPos(myCar.getPos());
	}
	
	//move backwards if S is being pressed
	if (keys['s'] == true){
		myCar.handleSKey();
		myArcballCamera.setObjPos(myCar.getPos());
	}
	
	//updates step values which are to be used for updating car variables
	myCar.checkMotion();
	
	//change cars heading if a or d is pressed (rotate left or right)
	if (keys['a'] == true){myCar.handleAKey();}
	if (keys['d'] == true){myCar.handleDKey();}
	
	//passive motion for stuff on back of car, car object handles
	myCar.updateBackTorus();
	
	//wheel motion depending on if moving bool, car object handles
	myCar.updateWheelMotion();
	
	//keep car in bounds of grid, which the car object handles
	//make sure to update the camera variables for object that it is looking at by passing in the camera
	myCar.checkCarBounds();
	
	//update interpolant value used for sprite movement along curve
	//render scene handles lopping everything but the decimal portion of the value
	//that is passed into the bezierCurve evaluation function
	interpolantValue += interpolantStepSize;
	//be sure to update which curve we are using (0-1 is curve 1, 1-2 is curve 2 etc.)
	whichCurve = (int) interpolantValue;
	
	//make sure to start from beginning of bezierCurves if reach the end so sprite loops
	if (whichCurve > (bezierCurves.size() - 1)){
		whichCurve = 0.0f;
		interpolantValue = 0.0f;
	}
	
	//update the sprite anims variables, let the class handle this
	mySprite.updateSprite();
	
	//force a redisplay and re register a timer callback!
	glutPostRedisplay();
	glutTimerFunc( 25, myTimer, 0 );
}

// myMenu() /////////////////////////////////////////////////////////////////////
//
//  Handles our Menu events
//
////////////////////////////////////////////////////////////////////////////////
void myMenu( int value ) {
	// handle our menu options based on string,int pairs we passed to the menu
	//0 - exit 1, - toggle control cages, 2 - toggle bez curve
	switch(value){
	case 0:
		//bye bye!
        exit(0);
		break;
	case 1:
		//toggle all the control cages for each bezier curve (toggle variable in class for each curve)
		for(unsigned int i = 0; i < bezierCurves.size(); i++){bezierCurves[i].toggleControlCage();}
		break;
	case 2:
		//toggle all the bezier curves for each bezier curve (toggle variable in class for each curve)
		for(unsigned int i = 0; i < bezierCurves.size(); i++){bezierCurves[i].toggleCurve();}
		break;
	}	
}

// createMenus() ///////////////////////////////////////////////////////////////
//
//  Handles creating a menu, adding menu entries, and attaching the menu to
//  a mouse button
//
////////////////////////////////////////////////////////////////////////////////
void createMenus() {
	//Create a Simple Menu
	
	//menu callback
	glutCreateMenu(myMenu);
	
	//menu options
	glutAddMenuEntry( "Quit", 0 ); 
	glutAddMenuEntry( "Toggle displaying control cage", 1 );
	glutAddMenuEntry( "Toggle displaying bezier curve", 2 );
	
	//attach menu to right mouse button
	glutAttachMenu(2); 
}

// loadControlPoints() /////////////////////////////////////////////////////////
//
//  Load our control points from file and store them in a global variable.
//
////////////////////////////////////////////////////////////////////////////////
bool loadControlPoints( char* filename ) {
	//open file stream to the file
	ifstream myFile(filename);
	
	//if opened successfully, read in the data
	if(myFile.is_open()){
		//read in the first value in the file which denotes the number of points in the file
		int numPoints;
		myFile >> numPoints;
		
		//float values to store the x,y,z values, and char value to catch the commas in the csv
		float x, y, z;
		char comma;
		
		//based on the number of points that we read earlier, start grabbing data and storing into control points
		for (int i = 0; i < numPoints; i++){
			myFile >> x;
			myFile >> comma;
			myFile >> y;
			myFile >> comma;
			myFile >> z;
			//ensure point being read correctly
			//fprintf(stdout, "\nx: %f, y: %f, z: %f\n", x,y,z);
			controlPoints.push_back(Point(x,y,z));	
		} 
		
		//make individual curves from groups of 4 points
		//note that the first 4 points denotes a curve, then the next 3 with the previous point denotes the next curve
		//ie we can have 4, 7, 10 etc points
		int startingIndex = 0;
		//total curves is simply (numpoints - 1) / 3 so only create that many curves
		for (unsigned int i = 0; i < ((controlPoints.size() - 1) / 3); i++){
			//create a bezierCurve object based on 4 points and push onto a collection for later use
			bezierCurves.push_back(BezierCurve(controlPoints[startingIndex], controlPoints[startingIndex + 1], controlPoints[startingIndex + 2], controlPoints[startingIndex + 3]));
			//increment by only 3 to use last point of previous curve as starting point for next curve
			startingIndex += 3;
		}
		
		for (unsigned int i = 0; i < 100; i+=25){
			for (unsigned int j = 0; j < 100; j+=25){
				bezPatchPoints.push_back(Point(i,getRand()*5,j));
			}
		}

		myBezPatch = BezierPatch(&bezPatchPoints);
		
	}else{
		//if here, file was not opened correctly, notify user
		fprintf(stdout, "Error opening file, exiting");
		exit(0);
	}
	return true;
}

// main() //////////////////////////////////////////////////////////////////////
//
//  Program entry point. Does not process command line arguments.
//
////////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv) {
	
	if (argc == 2){
		//make sure a control point CSV file was passed in.  Then read the points from file
		loadControlPoints(argv[1]);
	}else{
		//a csv file wasnt passed in, notify user of error
		fprintf(stdout, "Need to provide a single command line argument (name of file with control points)");
		exit(0);
	}

    // create a double-buffered GLUT window at (50,50) with predefined windowsize
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(50,50);
    glutInitWindowSize(windowWidth,windowHeight);
    glutCreateWindow("a3");

    // register callback functions...
	glutIgnoreKeyRepeat(1);//for multiple buttons to be pressed at same time
    glutKeyboardFunc(normalKeysDown);
    glutDisplayFunc(renderScene);
    glutReshapeFunc(resizeWindow);
    glutMouseFunc(mouseCallback);
    glutMotionFunc(mouseMotion);
	glutKeyboardUpFunc(keyboard_up);
	glutTimerFunc( 25, myTimer, 0 );
	
	// create our menu options and attach to mouse button
    createMenus();

    // do some basic OpenGL setup
    initScene();

    // and enter the GLUT loop, never to exit.
    glutMainLoop();

    return(0);
}
