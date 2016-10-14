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
    #include <ALUT/alut.h>  // OpenAL Headers
    #include <OpenAL/al.h>
    #include <OpenAL/alc.h>

    #include <GLUT/glut.h>  // OpenGL Headers
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
#else                       // else compiling on Linux OS
    #include <AL/alut.h>    // OpenAL Headers
    #include <AL/al.h>
    #include <AL/alc.h>

    #include <GL/glut.h>    // OpenGL Headers
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
#include "Vector3f.h"
#include "FreeCamera.h"
#include "Material.h"

//Heroes
#include "Hero3.h"
#include "Transport.h"

//Lighting
#include "Light.h"

// GLOBAL VARIABLES ////////////////////////////////////////////////////////////

//globals for arc length param
int arcStep = 10;
int whichArcCurve = 0;
Vector3f arcParamTangent;

//collection of all control points which will be made into Bcurves
vector<Point> trackBezPoints;
//collection of bezier curves (consist of 4 points each)
vector<BezierCurve> trackBezCurves;

Vector3f tangentVec;


//cameraselection
bool usingArcball = true;
bool usingFirstPersonCam = true;

//fps global vars
time_t lastTime = time(NULL);
int nbFrames = 0;
int displayValue = 0;

//sprite orientation
float spriteAngle = 0.0f;
Vector3f spriteAxisOfRotation = Vector3f();



//variables for window
static size_t windowWidth  = 640;
static size_t windowHeight = 480;
static float aspectRatio;

// status of the mouse buttons
GLint leftMouseButton, rightMouseButton;   
// last known X and Y of the mouse
int mouseX = 0, mouseY = 0;

//Player objects
Car myCar = Car();
Car carSprite = Car();
Hero3 hero3 = Hero3();
Transport transport = Transport();

Hero* currentHero = &myCar;
Hero* currentFirstPersonHero = &myCar;

//sprite object rotating around myCar
Sprite mySprite = Sprite();
//Arcball camera object, looking at player object location
ArcballCamera myArcballCamera = ArcballCamera(myCar.getX(), myCar.getY(), myCar.getZ());
FreeCamera myFreeCam = FreeCamera();

//collection of all control points which will be made into Bcurves
vector<Point> controlPoints;
//collection of bezier curves (consist of 4 points each)
vector<BezierCurve> bezierCurves;

vector<BezierCurve> innerBezierCurves;
vector<BezierCurve> outerBezierCurves;

vector<Point> bezPatchPoints;

BezierPatch myBezPatch;

Vector3f surfaceNormal = Vector3f();
Vector3f carAxisOfRotation = Vector3f();
float carAngle = 0.0f;

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

// Globals for OpenAL ---------------------------------------------
#define NUM_BUFFERS 3
#define NUM_SOURCES 3

ALCdevice *device;
ALCcontext *context;
ALuint buffers[ NUM_BUFFERS ];
ALuint sources[ NUM_SOURCES ];

GLint windowId;                             // id for our main window

struct worldObject {
  float x;
  float y;
  float z;

  char type;
} ;

vector<worldObject> worldObjects;

//lighting
Light mrLightington = Light(GL_LIGHT0);
Light headLight = Light(GL_LIGHT1);


// getRand() ///////////////////////////////////////////////////////////////////
//
//  Simple helper function to return a random number between 0.0f and 1.0f.
//
////////////////////////////////////////////////////////////////////////////////
float getRand() {
   	return rand() / (float)RAND_MAX;
}


// positionListener() //////////////////////////////////////////////////////////
//
// This function updates the listener's position and orientation.  First, the
//  global listener variables are updated.  Then the position and orientation
//  are set through the approriate OpenAL calls.
//
////////////////////////////////////////////////////////////////////////////////
void positionListener( float posX, float posY, float posZ,
                       float dirX, float dirY, float dirZ,
                       float upX = 0, float upY = 1, float upZ = 0 ) {
    /* TODO #04: Position Our Listener */
	ALfloat listenerPosition[3] = { posX, posY, posZ };
	ALfloat listenerOrientation[6] = { dirX, dirY, dirZ, upX, upY, upZ };
	alListenerfv(AL_POSITION, listenerPosition);
	alListenerfv(AL_ORIENTATION, listenerOrientation);
}

// positionSource() ////////////////////////////////////////////////////////////
//
// This function updates the sources's position.  The position
//  is set through the approriate OpenAL calls.
//
////////////////////////////////////////////////////////////////////////////////
void positionSource( ALuint src, float posX, float posY, float posZ ) {
    /* TODO #09: Position a Source */
	ALfloat srcPosition[3] = { posX, posY, posZ };
	alSourcefv(src, AL_POSITION, srcPosition);
}

// cleanupOpenAL() /////////////////////////////////////////////////////////////
//
//  At exit, clean up all of our OpenAL objects
//
////////////////////////////////////////////////////////////////////////////////
void cleanupOpenAL() {
    /* TODO #03: Cleanup OpenAL & ALUT */
	alcMakeContextCurrent(NULL);
	alcDestroyContext(context);
	alcCloseDevice(device);
    /* TODO #07: Delete our Buffers and Sources */
	alDeleteBuffers(NUM_BUFFERS, buffers);
	alDeleteSources(NUM_SOURCES, sources);
}

// cleanupOpenGL() /////////////////////////////////////////////////////////////
//
//  At exit, clean up all of our OpenGL objects
//
////////////////////////////////////////////////////////////////////////////////
void cleanupOpenGL() {
    glutDestroyWindow( windowId );  // destroy our window
}

// initializeOpenAL() //////////////////////////////////////////////////////////
//
//  Do all of our one time OpenAL & ALUT setup
//
////////////////////////////////////////////////////////////////////////////////
void initializeOpenAL( int argc, char *argv[] ) {
    ALsizei size, freq;
    ALenum 	format;
    ALvoid 	*data;
    ALboolean loop;
    
    /* TODO #01: Setup ALUT and OpenAL */
	alutInit(&argc, argv);
	device = alcOpenDevice(NULL);
	context = alcCreateContext(device, NULL);
	alcMakeContextCurrent(context);

    /* TODO #06: Generate Buffers & Sources */
	alGenBuffers(NUM_BUFFERS, buffers);
	alGenSources(NUM_SOURCES, sources);

    /* TODO #08: Create Our Stationary Sound */
	#ifdef __APPLE__
		alutLoadWAVFile((ALbyte*)"wavs/ambiance.wav", &format, &data, &size, &freq);
	#else
		alutLoadWAVFile((ALbyte*)"wavs/ambiance.wav", &format, &data, &size, &freq, &loop);
	#endif

	alBufferData(buffers[0], format, data, size, freq);;
	alutUnloadWAV(format, data, size, freq);

	alSourcei(sources[0], AL_BUFFER, buffers[0]);
	alSourcei(sources[0], AL_LOOPING, AL_TRUE);

	#ifdef __APPLE__
		alutLoadWAVFile((ALbyte*)"wavs/carengine.wav", &format, &data, &size, &freq);
	#else
		alutLoadWAVFile((ALbyte*)"wavs/carengine.wav", &format, &data, &size, &freq, &loop);
	#endif

	alBufferData(buffers[1], format, data, size, freq);;
	alutUnloadWAV(format, data, size, freq);

	alSourcei(sources[1], AL_BUFFER, buffers[1]);
	alSourcei(sources[1], AL_LOOPING, AL_TRUE);

	#ifdef __APPLE__
		alutLoadWAVFile((ALbyte*)"wavs/heylisten.wav", &format, &data, &size, &freq);
	#else
		alutLoadWAVFile((ALbyte*)"wavs/heylisten.wav", &format, &data, &size, &freq, &loop);
	#endif

	alBufferData(buffers[2], format, data, size, freq);;
	alutUnloadWAV(format, data, size, freq);

	alSourcei(sources[2], AL_BUFFER, buffers[2]);
	alSourcei(sources[2], AL_LOOPING, AL_TRUE);
    
    //PrintOpenALInfo();					// print our OpenAL versioning information
}

// drawCity() //////////////////////////////////////////////////////////////////
//
//  Function to draw a random city using GLUT 3D Primitives
//
////////////////////////////////////////////////////////////////////////////////
void drawCity() {
    // Randomly place trees of varying sizes
	for (int i = -50; i < 51; i++) {
		for (int j = -50; j < 51; j++) {
			if (i%2 == 0 && j%2 == 0 && getRand() < 0.005) {
				float u = (i + 50.0) / 100.0f;
				float v = (j + 50.0) / 100.0f;
				float y =myBezPatch.getYPosition(u,v);

				float cylHeight = getRand() + 1;

				//draw tree stump
				glPushMatrix();
				glTranslatef(i, y - cylHeight/2.0f, j);
				glColor3ub(105, 66, 0);
				glRotatef(-90, 1, 0, 0);
				GLUquadricObj *myCyl;
				myCyl = gluNewQuadric();
				gluQuadricDrawStyle(myCyl, GLU_FILL);
				gluCylinder(myCyl, 0.5, 0.5, cylHeight, 10, 2);
				glPopMatrix();

				//draw tree leaves
				glPushMatrix();
				glColor3ub(40, 132, 0);
				glTranslatef(i, cylHeight + (y - cylHeight/2.0f), j);
				glRotatef(-90, 1, 0, 0);
				glutSolidCone(cylHeight * 1.1, cylHeight * 7, 15, 15);
				glPopMatrix();
			}
		}
	}
	 
	glDisable( GL_COLOR_MATERIAL );
	glShadeModel(GL_SMOOTH);
	Material myMat = Material();
	for(int i=0; i<worldObjects.size();i++){
		glPushMatrix();
		worldObject currentObj = worldObjects[i];
		glTranslatef(currentObj.x,currentObj.y,currentObj.z);

		//aligning objects with surface
		float u = (currentObj.x + 50.0) / 100.0f;
		float v = (currentObj.z + 50.0) / 100.0f;
		Point tempP = myBezPatch.dVBezier(u,v);
		Vector3f dV = Vector3f(tempP.getX(),tempP.getY(),tempP.getZ());
		tempP = myBezPatch.dUBezier(u,v);
		Vector3f dU = Vector3f(tempP.getX(),tempP.getY(),tempP.getZ());
		Vector3f tempNormal = dU.crossProduct(dV);
		//X and Z are flipped...
		float tempVar = tempNormal.getX();
		tempNormal.setX(surfaceNormal.getZ());
		tempNormal.setZ(tempVar);
		Vector3f axisOfRot = tempNormal.crossProduct(Vector3f(0.0f, 1.0f, 0.0f));
		float angleRot = tempNormal.getAngleBetween(Vector3f(0.0f, 1.0f, 0.0f));

		glRotatef(-angleRot, axisOfRot.getX(),axisOfRot.getY(),axisOfRot.getZ());
		glTranslatef(0,3,0);

		if(currentObj.type=='t'){
			myMat.gold();
			myMat.setMaterial();
			glutSolidTeapot(6);
		}else{
			myMat.cyanrubber();
			myMat.setMaterial();
			glutWireTeapot(6);
		}
		glPopMatrix();
	}
	glShadeModel(GL_FLAT);
	glEnable( GL_COLOR_MATERIAL );
}

void drawCylinderBetweenPoints(Point a, Point b){
 glPushMatrix();
// This is the default direction for the cylinders to face in OpenGL
Vector3f z = Vector3f(0,0,1);         
// Get diff between two points you want cylinder along
Point temp = (a - b);
Vector3f p =  Vector3f(temp.getX(),temp.getY(),temp.getZ());                
// Get CROSS product (the axis of rotation)
Vector3f t = z.crossProduct(p);
 
// Get angle. LENGTH is magnitude of the vector
double angle = 180 / M_PI * acos((z.dotProduct(p)/p.getLength()));
 
glTranslated(b.getX(),b.getY(),b.getZ());
glRotated(angle,t.getX(),t.getY(),t.getZ());

gluCylinder(gluNewQuadric(),.5,.5, p.getLength(), 10, 10);
glPopMatrix();
}



void drawTrack(){
		for (unsigned int i = 0; i < innerBezierCurves.size(); i++){
				glColor3f(1.0, 0.0, 0.0);
				innerBezierCurves[i].renderBezierCurve();
				outerBezierCurves[i].renderBezierCurve();
				for(float j = 0; j <1.0; j+=.05 ){
					Point p1 = innerBezierCurves[i].evaluateBezierCurve(j);
					Point p2 = outerBezierCurves[i].evaluateBezierCurve(j);
					glLineWidth(2.5); 
					glColor3f(1.0, 0.0, 0.0);
					glBegin(GL_LINES);
					glVertex3f(p1.getX(), p1.getY(), p1.getZ());
					glVertex3f(p2.getX(), p2.getY(), p2.getZ());
					glEnd();
					drawCylinderBetweenPoints(Point(p1.getX(), p1.getY(), p1.getZ()),Point(p2.getX(), p2.getY(), p2.getZ()));
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
	glDisable( GL_COLOR_MATERIAL );
	glShadeModel(GL_SMOOTH);
	Material myMat = Material();
	myMat.emerald();
	myMat.setMaterial();
	//draw the bezier surface!
	for (int i = -50; i < 51; i++) {
		for (int j = -50; j < 51; j++) {
			float u = (i + 50.0) / 100.0f;
			float v = (j + 50.0) / 100.0f;
			float y =myBezPatch.getYPosition(u,v);
			glPushMatrix();
				glTranslatef(i, y, j);
				glutSolidCube(1);
			glPopMatrix();
		}
	}
	glPopMatrix();
	glShadeModel(GL_FLAT);
	glEnable( GL_COLOR_MATERIAL );
	drawTrack();
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
			if (usingArcball){
				myArcballCamera.handleCameraDrag(mouseX, x, mouseY, y);
			}else{myFreeCam.handleCameraDrag(mouseX, x, mouseY, y);}
		}
		//store last position of mouse
		mouseX = x;
		mouseY = y;
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
    /*float lightCol[4] = { 1, 1, 1, 1};
    float ambientCol[4] = { 0.0, 0.0, 0.0, 1.0 };
    float lPosition[4] = { 10, 10, 10, 1 };
    glLightfv( GL_LIGHT0, GL_POSITION,lPosition );
    glLightfv( GL_LIGHT0, GL_DIFFUSE,lightCol );
    glLightfv( GL_LIGHT0, GL_AMBIENT, ambientCol );
    glEnable( GL_LIGHTING );
    glEnable( GL_LIGHT0 );*/
    glEnable( GL_LIGHTING ); 
    //glLightf( GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.5 );
    mrLightington.enable();
    mrLightington.setPosition(0,0,0);
    mrLightington.setColors(Color(0,0,0),Color(1,1,1),Color(0,0,0));

    headLight.enable();
    //headLight.setPosition(0,0,0);
    headLight.setColors(Color(0,0,0),Color(1,1,1),Color(0,0,0));
    headLight.setCutoffAngle(35);

    glClearColor( 0, 0, 1, 1);

	
    // tell OpenGL not to use the material system; just use whatever we 
    // pass with glColor*()
    glEnable( GL_COLOR_MATERIAL );
    glColorMaterial( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );
    //******************************************************************

    //Material myMat = Material(Color(0.19225,  0.19225, 0.19225), Color(0.50754,  0.50754, 0.50754), Color(0.508273, 0.508273,  0.508273), .4);

    glShadeModel(GL_FLAT);

    srand( time(NULL) );	// seed our random number generator
    generateEnvironmentDL();
}

void drawScene(bool drawCar=true, bool drawHero3=true, bool drawTransport=true){

	float pos[4] = {myCar.getX(),myCar.getY(),myCar.getZ(), 1};
	glLightfv(GL_LIGHT1, GL_POSITION, pos);

	//call display list so dont have to recompute each time
	glCallList( environmentDL );
	
	//draw the car! translate/rotate based on input
	//fprintf(stdout, "\ny: %f\n", myCar.getY());
    glPushMatrix(); {
		//translate the car to current position (which is changed by user input)
		glTranslatef(myCar.getX(),myCar.getY(),myCar.getZ());

		//adjust cars heading (updated in timer via user input)
		glRotatef(-carAngle,carAxisOfRotation.getX(),carAxisOfRotation.getY(),carAxisOfRotation.getZ());
		glRotatef(myCar.getTheta(),0,1,0);


		
		if(drawCar)
	    	myCar.draw();
    }; glPopMatrix();

    

    glPushMatrix();
    float arcParamTVal = trackBezCurves[whichArcCurve].getParamTVal(arcStep);
    Point arcParamPoint = trackBezCurves[whichArcCurve].evaluateBezierCurve(arcParamTVal);
  	arcParamTangent = trackBezCurves[whichArcCurve].evaluateTangentPoint(arcParamTVal);
  	spriteAxisOfRotation = tangentVec.crossProduct(Vector3f(0.0f, 1.0f, 0.0f));
	spriteAngle = tangentVec.getAngleBetween(Vector3f(0.0f, 1.0f, 0.0f));
    glTranslatef(arcParamPoint.getX(),arcParamPoint.getY() + 3,arcParamPoint.getZ());
    glRotatef(-spriteAngle, spriteAxisOfRotation.getX(), spriteAxisOfRotation.getY(), spriteAxisOfRotation.getZ());
    //orient with curve
    if(drawTransport){
    	transport.draw();
    }
    transport.setPos(arcParamPoint);

 

    glPopMatrix();

    glPushMatrix();
    //begin roller coaster hero stuff NON PARAM
    Point derpyPoint = trackBezCurves[whichCurve].evaluateBezierCurve((float) interpolantValue - whichCurve);
	tangentVec = trackBezCurves[whichCurve].evaluateTangentPoint((float) interpolantValue - whichCurve);
    spriteAxisOfRotation = tangentVec.crossProduct(Vector3f(0.0f, 1.0f, 0.0f));
	spriteAngle = tangentVec.getAngleBetween(Vector3f(0.0f, 1.0f, 0.0f));
    glTranslatef(derpyPoint.getX(),derpyPoint.getY() + 3,derpyPoint.getZ());
	glRotatef(-spriteAngle, spriteAxisOfRotation.getX(), spriteAxisOfRotation.getY(), spriteAxisOfRotation.getZ());
	hero3.setX(derpyPoint.getX());
	hero3.setY(derpyPoint.getY() + 3);
	hero3.setZ(derpyPoint.getZ());
	if(drawHero3){hero3.draw();}
    glPopMatrix();
}

void drawFPS(){
		glDisable(GL_LIGHTING);
    	glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		gluOrtho2D(0.0, windowWidth, 0.0, windowHeight);

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();

	    glColor3ub(255, 255, 255); // Green
	    glRasterPos2i(10, windowHeight - 10);
	    char buffer [33];
	    itoa (displayValue,buffer,10);
	    string s(buffer);
	    string derp = "FPS: ";
	    s = derp + s;
		void * font = GLUT_BITMAP_9_BY_15;
		for (string::iterator i = s.begin(); i != s.end(); ++i)
		{
		    char c = *i;
		    glutBitmapCharacter(font, c);
		}

		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();

		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glEnable(GL_LIGHTING);

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
    float* c;
    if(usingArcball){
    	c = myArcballCamera.getCameraInfo();
    }else{c = myFreeCam.getCameraInfo();}
	
	//c0-c2 are camera position + object position (xyz)
	//c3-c5 are object position xyz (camera keeps track and updates accordingly in timer)
	//c6-c8 are the xyz of the up vector (0,1,0) 
	gluLookAt(c[0], c[1], c[2], c[3], c[4], c[5], c[6], c[7], c[8]);

	drawScene();
	drawFPS();

	//OpenAL sound stuff
	 c = myArcballCamera.getCameraInfo();
    /* TODO #05: Place our Listener every frame */
	positionListener(c[0], c[1], c[2],
					 myArcballCamera.getDirX(),  myArcballCamera.getDirY(),  myArcballCamera.getDirZ(),
					 0.0f, 1.0f, 0.0f);

	positionSource(sources[1], myCar.getX(), myCar.getY(), myCar.getZ());
	positionSource(sources[0], myArcballCamera.getX(), myArcballCamera.getY(), myArcballCamera.getZ());

	positionSource(sources[2], hero3.getX(), hero3.getY(), hero3.getZ());
	
	
	if (usingFirstPersonCam){
		//Clear the way for the second camera
		glClear(GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, windowWidth/3, windowHeight/3);
		glScissor(0, 0, windowWidth/3, windowHeight/3);
		glEnable(GL_SCISSOR_TEST);
		glClear(GL_COLOR_BUFFER_BIT);
		
		glMatrixMode(GL_MODELVIEW); 
		glLoadIdentity();

		//first person cam stuff
		
		if (currentFirstPersonHero == &myCar){
			glRotatef(-myCar.getTheta()-90,0,1,0);
			glRotatef(-carAngle,carAxisOfRotation.getX(),carAxisOfRotation.getY(),carAxisOfRotation.getZ());
			gluLookAt(myCar.getX(),myCar.getY(),myCar.getZ(), 
					  myCar.getX(),myCar.getY(),myCar.getZ()+1, 
												  0.0, 1.0, 0.0);
			drawScene(false,true,true);
		}

		if (currentFirstPersonHero == &hero3){
			gluLookAt(hero3.getX(),hero3.getY() + 3,hero3.getZ(), 
				  	  hero3.getX() + tangentVec.getX()*50,hero3.getY() + tangentVec.getY()*50,hero3.getZ()+tangentVec.getZ()*50, 
											  0.0, 1.0, 0.0);
			
			//passing in false so first person cam is inside of hero, no get blocked
			drawScene(true,false,true);
		}	

		if (currentFirstPersonHero == &transport){
			gluLookAt(transport.getX(),transport.getY() + 3,transport.getZ(), 
				  	  transport.getX() + arcParamTangent.getX()*50,transport.getY() + arcParamTangent.getY()*50,transport.getZ()+arcParamTangent.getZ()*50, 
											  0.0, 1.0, 0.0);
			
			//passing in false so first person cam is inside of hero, no get blocked
			drawScene(true,true,false);
		}	

	}
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
	if (key == 'i'){keys['i'] = true;}
	if (key == 'k'){keys['k'] = true;}
}

//determine which keys are released
void keyboard_up(unsigned char key, int x, int y){
	//update keys array to reflect which keys are not being pressed anymore
	//update motion booleans
	if (key == 'w'){keys['w'] = false; myCar.setForwardCarMotion(false);}
	if (key == 's'){keys['s'] = false; myCar.setBackwardsCarMotion(false);}
	if (key == 'a'){keys['a'] = false;}
	if (key == 'd'){keys['d'] = false;}
	if (key == 'i'){keys['i'] = false;}
	if (key == 'k'){keys['k'] = false;}
}

void orientCar(){
		float tempX = myCar.getX();
		float tempZ = myCar.getZ();

		float u = (tempX + 50.0) / 100.0f;
		float v = (tempZ + 50.0) / 100.0f;

		Point tempP = myBezPatch.dVBezier(u,v);
		Vector3f dV = Vector3f(tempP.getX(),tempP.getY(),tempP.getZ());
		tempP = myBezPatch.dUBezier(u,v);
		Vector3f dU = Vector3f(tempP.getX(),tempP.getY(),tempP.getZ());

		surfaceNormal = dU.crossProduct(dV);

		//X and Z are flipped...
		float tempVar = surfaceNormal.getX();
		surfaceNormal.setX(surfaceNormal.getZ());
		surfaceNormal.setZ(tempVar);

		//fprintf(stdout, "\nx: %f, y: %f, z: %f\n", surfaceNormal.getX(), surfaceNormal.getY(), surfaceNormal.getZ());
		carAxisOfRotation = surfaceNormal.crossProduct(Vector3f(0.0f, 1.0f, 0.0f));
		carAngle = surfaceNormal.getAngleBetween(Vector3f(0.0f, 1.0f, 0.0f));
		//fprintf(stdout, "\nangle: %f\n", carAngle);
		myCar.setCarNormal(surfaceNormal);//for first person view


		//fprintf(stdout, "\nu: %f, v: %f\n", u,v);

		myCar.setY(myBezPatch.getYPosition(u,v) + 2.5f);

	
}


//timer function handles actions done by button presses,
//updating passive motion variables
//and updating input based variables for animation
void myTimer(int value){

	//get current heading of car, let car object handle updating
	myCar.updateHeading();

	//OpenAL sound stuff for car motor
	ALenum sourceState;
	alGetSourcei(sources[1], AL_SOURCE_STATE, &sourceState);
	if(keys['w'] == true|| keys['a'] == true || keys['s'] == true ||keys['d'] == true){
		if (sourceState != AL_PLAYING) {
			alSourcePlay(sources[1]);
		}
	}
	else{
		alSourceStop(sources[1]);
	}
	
	//check keys and update position
	//doing in timer for multiple keys being held down 
	//make sure to update the camera variables for object that it is looking at by passing in the camera
	//move forward W is being pressed
	if (keys['w'] == true){
		myCar.handleWKey();
		orientCar();
		
	}
	
	//move backwards if S is being pressed
	if (keys['s'] == true){
		myCar.handleSKey();
		orientCar();
	}
	
	//updates step values which are to be used for updating car variables
	myCar.checkMotion();
	
	//change cars heading if a or d is pressed (rotate left or right)
	if (keys['a'] == true){myCar.handleAKey();}
	if (keys['d'] == true){myCar.handleDKey();}

	//handle free cam movement
	if (keys['i'] == true){myFreeCam.handleForwardKey();}
	if (keys['k'] == true){myFreeCam.handleBackwardsKey();}
	
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
	arcStep += 1;
	if (arcStep == 128){
		whichArcCurve++;
		arcStep = 0;
		if(whichArcCurve > trackBezCurves.size() - 1){
			whichArcCurve = 0;
		}
	}
	//be sure to update which curve we are using (0-1 is curve 1, 1-2 is curve 2 etc.)
	whichCurve = (int) interpolantValue;
	
	//make sure to start from beginning of bezierCurves if reach the end so sprite loops
	if (whichCurve > (bezierCurves.size() - 1)){
		whichCurve = 0.0f;
		interpolantValue = 0.0f;
	}
	
	//update the sprite anims variables, let the class handle this
	mySprite.updateSprite();


	time_t currentTime = time(NULL);
	nbFrames++;
	if (difftime(currentTime, lastTime) >= 1.0f){
		//fprintf(stdout, "\n%d\n", nbFrames);
		displayValue = nbFrames;
		nbFrames = 0;
		lastTime = time(NULL);


	}

	myArcballCamera.setObjPos(currentHero->getPos());

	hero3.updateAnimation();
	transport.updateAnimation();
	mrLightington.setPosition(hero3.getX(),hero3.getY(),hero3.getZ());
	//force a redisplay and re register a timer callback!
	glutPostRedisplay();
	glutTimerFunc(1000.0f / 60.0f, myTimer, 0);
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
	case 3: 
		usingArcball = !usingArcball;
		break;
	case 4:
		usingFirstPersonCam = ! usingFirstPersonCam;
		break;
	}	
}

void mySubMenuArcball(int value){
	switch(value){
	case 0:
		currentHero = &myCar;
		break;
	case 1:
		currentHero = &transport;
		break;
	case 2:
		currentHero = &hero3;
		break;
	}
}

void mySubMenuFirstPerson(int value){
	switch(value){
	case 0:
		currentFirstPersonHero = &myCar;
		break;
	case 1:
		currentFirstPersonHero = &transport;
		break;
	case 2:
		currentFirstPersonHero = &hero3;
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
	

	//Make sub-menu for first person camera
	int subMenuFirstPerson = glutCreateMenu(mySubMenuFirstPerson);
	glutAddMenuEntry("Hero 1", 0);
	glutAddMenuEntry("Hero 2", 1);
	glutAddMenuEntry("Hero 3", 2);

	//Make sub-menu for arcball camera
	int subMenuArcball = glutCreateMenu(mySubMenuArcball);
	glutAddMenuEntry("Hero 1", 0);
	glutAddMenuEntry("Hero 2", 1);
	glutAddMenuEntry("Hero 3", 2);

	//menu callback
	int mainMenu = glutCreateMenu(myMenu);

	//menu options
	glutAddMenuEntry( "Quit", 0 ); 
	glutAddMenuEntry( "Toggle displaying control cage", 1 );
	glutAddMenuEntry( "Toggle displaying bezier curve", 2 );
	glutAddMenuEntry( "Toggle Free Camera", 3 );
	glutAddMenuEntry( "Toggle First Person Camera", 4 );
	glutAddSubMenu("First Person Camera Hero", subMenuFirstPerson);
	glutAddSubMenu("Arcball Camera Hero", subMenuArcball);

	//attach menu to right mouse button
	glutAttachMenu(2); 
}


// loadControlPoints() /////////////////////////////////////////////////////////
//
//  Load our control points from file and store them in a global variable.
//
////////////////////////////////////////////////////////////////////////////////
bool loadWorldFile( char* filename ) {
		//open file stream to the file
	ifstream myFile(filename);
	
	//if opened successfully, read in the data
	if(myFile.is_open()){
		//read in the first value in the file which denotes the number of points in the file
		int numPoints;
		
		//float values to store the x,y,z values, and char value to catch the commas in the csv
		float x, y, z;
		char comma;
		
		//assume that file has 16 points for curve
		for (int i = 0; i < 16; i++){
			myFile >> x;
			myFile >> comma;
			myFile >> y;
			myFile >> comma;
			myFile >> z;
			//ensure point being read correctly
			bezPatchPoints.push_back(Point(x,y,z));
		} 

		myBezPatch = BezierPatch(&bezPatchPoints);

		myFile >> numPoints;
		//based on the number of points that we read earlier, start grabbing data and storing into control points
		for (int i = 0; i < numPoints; i++){
			myFile >> x;
			myFile >> comma;
			myFile >> y;
			myFile >> comma;
			myFile >> z;
			//ensure point being read correctly
			controlPoints.push_back(Point(x,y,z));	
			trackBezPoints.push_back(Point(x,y,z));
		} 

		for (unsigned int j = 0; j < trackBezPoints.size(); j++){ 
			trackBezPoints[j].setXYZ(trackBezPoints[j].getX() , trackBezPoints[j].getY(), trackBezPoints[j].getZ());
		}
		
		//make individual curves from groups of 4 points
		//note that the first 4 points denotes a curve, then the next 3 with the previous point denotes the next curve
		//ie we can have 4, 7, 10 etc points
		int startingIndex = 0;
		//total curves is simply (numpoints - 1) / 3 so only create that many curves
		for (unsigned int i = 0; i < ((controlPoints.size() - 1) / 3); i++){
			//create a bezierCurve object based on 4 points and push onto a collection for later use
			bezierCurves.push_back(BezierCurve(controlPoints[startingIndex], controlPoints[startingIndex + 1], controlPoints[startingIndex + 2], controlPoints[startingIndex + 3]));
			trackBezCurves.push_back(BezierCurve(trackBezPoints[startingIndex], trackBezPoints[startingIndex + 1], trackBezPoints[startingIndex + 2], trackBezPoints[startingIndex + 3]));
			
			innerBezierCurves.push_back(BezierCurve(trackBezPoints[startingIndex]*.9, trackBezPoints[startingIndex + 1]*.9, trackBezPoints[startingIndex + 2]*.9, trackBezPoints[startingIndex + 3]*.9));
			outerBezierCurves.push_back(BezierCurve(trackBezPoints[startingIndex]*1.1, trackBezPoints[startingIndex + 1]*1.1, trackBezPoints[startingIndex + 2]*1.1, trackBezPoints[startingIndex + 3]*1.1));



			//increment by only 3 to use last point of previous curve as starting point for next curve
			//increment by only 3 to use last point of previous curve as starting point for next curve
			startingIndex += 3;
		}
		
		int numObjects;
		myFile >> numObjects;
		char obj;
		for (int i = 0; i < numObjects; i++){
			myFile >> obj;
			myFile >> comma;
			myFile >> x;
			myFile >> comma;
			myFile >> z;

			float u = (x + 50.0) / 100.0f;
			float v = (z + 50.0) / 100.0f;
			float y = myBezPatch.getYPosition(u,v);

			worldObject tempObj;
			tempObj.x=x;
			tempObj.y=y;
			tempObj.z=z;
			tempObj.type=obj;

			worldObjects.push_back(tempObj);
		} 


	for (int i = 0; i < trackBezCurves.size(); i++){
		trackBezCurves[i].calculateStepDistances();
	}	
		
		
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
	
	printf("in main");
	
	if (argc == 2){
		//make sure a control point CSV file was passed in.  Then read the points from file
		loadWorldFile(argv[1]);
	}else{
		//a csv file wasnt passed in, notify user of error
		fprintf(stdout, "Need to provide a single command line argument (name of file with world info)");
		exit(0);
	}

    // create a double-buffered GLUT window at (50,50) with predefined windowsize
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(50,50);
    glutInitWindowSize(windowWidth,windowHeight);
    windowId=glutCreateWindow("a3");

    // register callback functions...
	glutIgnoreKeyRepeat(1);//for multiple buttons to be pressed at same time
    glutKeyboardFunc(normalKeysDown);
    glutDisplayFunc(renderScene);
    glutReshapeFunc(resizeWindow);
    glutMouseFunc(mouseCallback);
    glutMotionFunc(mouseMotion);
	glutKeyboardUpFunc(keyboard_up);
	glutTimerFunc(1000.0f / 60.0f, myTimer, 0);

	initializeOpenAL( argc, argv );     // do all of our setup for OpenAL

	//currentHero= &hero3;

	//atexit( cleanupOpenGL );            // callback that gets called right before the program exits
	atexit( cleanupOpenAL );            // callback that gets called right before the program exits

	orientCar();

	//play sound forever
	alSourcePlay(sources[2]);
	alSourcePlay(sources[0]);
	
	// create our menu options and attach to mouse button
    createMenus();

    // do some basic OpenGL setup
    initScene();


    // and enter the GLUT loop, never to exit.
    glutMainLoop();

    return(0);
}
