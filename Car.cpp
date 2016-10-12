#include "Car.h"

//default constructor for car, sets up all the instance variables to nice values as per the grid setup in A3
Car::Car(){
	//position
	carX = 0;
	carY = 2.5;
	carZ = 0;
	
	//heading
	carTheta = 0;
	carDirZ = 1;
	carDirX = 0;

	carNormal = Vector3f(0.0f, 1.0f, 0.0f);
	
	//passive animation value that gets updated
	backTorusRotation = 0;
	
	//bools for user interaction animation
	forwardCarMotion = false;
	backwardsCarMotion = false;
	
	//user interaction animation values
	wheelRotation = 0;
	carHeadingStep = 1.5;
	carTranslationStepSize = 1;
}

//constructor with extra parameters if want to change car to different starting values from a3
Car::Car(float x, float y, float z, float theta, float dirZ, float dirX, float backTRot){
	//position
	carX = x;
	carY = y;
	carZ = z;
	
	//heading
	carTheta = theta;
	carDirZ = dirZ;
	carDirX = dirX;
	
	//passive anims value that gets updated
	backTorusRotation = backTRot;
	
	//bools for user interaction animation
	forwardCarMotion = false;
	backwardsCarMotion = false;
	
	//user interaction animation values
	wheelRotation = 0;
	carHeadingStep = 1.5;
	carTranslationStepSize = 1;
}

//setters for all of the instance variables of the car
//dont need for a4, but useful for debugging and may need later
void Car::setX(float x){carX = x;}
void Car::setY(float y){carY = y;}
void Car::setZ(float z){carZ = z;}
void Car::setTheta(float theta){carTheta = theta;}
void Car::setDirZ(float z){carDirZ = z;}
void Car::setDirX(float x){carDirX = x;}
void Car::setBackTorusRotation(float btr){backTorusRotation = btr;}
void Car::setWheelRotation(float wr){wheelRotation = wr;}
void Car::setCarHeadingStep(float chs){carHeadingStep = chs;}
void Car::setCarTranslationStepSize(float ctss){carTranslationStepSize = ctss;}
void Car::setForwardCarMotion(bool fcm){forwardCarMotion = fcm;}
void Car::setBackwardsCarMotion(bool bcm){backwardsCarMotion = bcm;}

//getters for all of the instance variables of the car
//dont need for a4, but useful for debugging and may need later
float Car::getX(){return carX;}
float Car::getY(){return carY;}
float Car::getZ(){return carZ;}
Point Car::getPos(){return Point(carX,carY,carZ);}
float Car::getTheta(){return carTheta;}
float Car::getDirZ(){return carDirZ;}
float Car::getDirX(){return carDirX;}
float Car::getBackTorusRotation(){return backTorusRotation;}
float Car::getWheelRotation(){return wheelRotation;}
float Car::getCarHeadingStep(){return carHeadingStep;}
float Car::getCarTranslationStepSize(){return carTranslationStepSize;}
bool Car::getForwardCarMotion(){return forwardCarMotion;}
bool Car::getBackwardsCarMotion(){return backwardsCarMotion;}

//draw a unit right triangular prism out of glut primitives for windshield
void Car::drawUnitRightTrianglePrism(){
	 glDisable( GL_LIGHTING );//disable lighting since using primitives
	 
	 //draw two right triangles a unit apart
	 glBegin(GL_TRIANGLES);
	 //right triangle along z axis
	 glVertex3f(0,0,0);
	 glVertex3f(0,1,0);
	 glVertex3f(0,0,1);
	 
	 //right triangle parallel to z axis one unit out
	 glVertex3f(1,0,0);
	 glVertex3f(1,1,0);
	 glVertex3f(1,0,1);
	 glEnd();
	 
	 //connect triangles on all sides to give prism appearance
	 glBegin(GL_QUADS);
	 //bottom face
	 glVertex3f(0,0,0);
	 glVertex3f(1,0,0);
	 glVertex3f(1,0,1);
	 glVertex3f(0,0,1);
	 
	 //angled face
	 glVertex3f(1,1,0);
	 glVertex3f(0,1,0);
	 glVertex3f(0,0,1);
	 glVertex3f(1,0,1);
	 
	 //back face
	 glVertex3f(1,1,0);
	 glVertex3f(0,1,0);
	 glVertex3f(0,0,0);
	 glVertex3f(1,0,0);
	 glEnd();
	 
	 glEnable(GL_LIGHTING); //reenable lighting after primitives drawn
}

//draw torus with a ball in the middle to give wheel like appearance
void Car::drawBallWheel(){
	//draw ball in middle yellow
	glColor3ub(255,255,0);
	glutSolidSphere(0.5,20,20);
	//tire is colored grayish
	glColor3ub(	49,79,79);
	glutSolidTorus(0.5,1,20,20);
}

//draw axles of car (including wheel)
void Car::drawAxle(){
	glPushMatrix();
	//translate to front/rotate cylinder 90 degrees to align with car body
	glTranslatef(-3,-1,3);
	glRotatef(90,0,1,0);
	
	//notice z and x flipped now due to 90 deg rotation for the cylinder
	
	//check motion booleans to determine which way to rotate the wheels/axles during motion
	if (forwardCarMotion){glRotatef(wheelRotation,0,0,1);}
	else if (backwardsCarMotion){glRotatef(-wheelRotation,0,0,1);}
	
	gluCylinder(gluNewQuadric(), .5, .5, 6, 20, 20); //draw actual cylinder
	
	//right wheel(s)
	drawBallWheel();
	glTranslatef(0,0,6);
	//left wheel(s)
	drawBallWheel();
	glPopMatrix();
}

//silly function for including passive motion
void Car::drawBackMagicalSpinnyThingy(){
	glColor3ub(	127,255,0);
	glPushMatrix();
	glRotatef(180,0,1,0);
	glTranslatef(0,0,-2);
	//draw a cone
	glutSolidCone(1,2,20,20);
	glPopMatrix();
	glColor3ub(	0,250,154);
	//encircle cone tip with a torus
	glutSolidTorus(.5,1,20,20);
}

//function for drawing body of car
void Car::drawCarBody(){
	glColor3ub(255,0,0);
	glPushMatrix();
	//draw a red 4*8 sheet
	glScalef(4,2,8);
	glutSolidCube( 1 );
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0,1.5,-2);
	//with a 4*4 sheet on top to give a car esque look
	glScalef(4,1,4);
	glutSolidCube(1);
	glPopMatrix();
	
	//draw windshield out of right trianglular prism
	//connecting top sheet to bottom sheet
	glColor3ub(0,191,255);//blueish color
	glPushMatrix();
	glTranslatef(-2,1,0);
	glScalef(4,1,2);
	drawUnitRightTrianglePrism();
	glPopMatrix();
}

//function for drawing entire car out of individual pieces
void Car::drawCar(){
	glPushMatrix();
	
	glRotatef(90,0,1,0);
	drawCarBody();
	
	glColor3ub(190,190,190);
	//draw front axle/wheels
	drawAxle();
	
	//draw back axle/wheels
	glPushMatrix();
	glTranslatef(0,0,-6);
	glColor3ub(190,190,190);
	drawAxle();
	glPopMatrix();
	
	//draw torus on back for idle anims
	glPushMatrix();
	glTranslatef(0,1,-5);
	glRotatef(backTorusRotation,0,0,1);
	drawBackMagicalSpinnyThingy();
	glPopMatrix();
	
	glPopMatrix();
}

//user interaction anims for going forward
//update x and z translation based on heading (dirZ and dirX) by the specified step size
//tell the camera that we did this, tell the car we are going forward because w key was pressed
void Car::handleWKey(){
	carZ -= carDirZ*carTranslationStepSize;
	carX -= carDirX*carTranslationStepSize;
	
	forwardCarMotion = true;
}

//user interaction anims for going backwads
//update x and z translation based on heading (dirZ and dirX) by the specified step size
//tell the camera that we did this, tell the car we are going backwards because s key was pressed
void Car::handleSKey(){
	carZ += carDirZ*carTranslationStepSize;
	carX += carDirX*carTranslationStepSize;
	
	backwardsCarMotion = true;
}

//keep car in bounds by checking position and limiting car position
//updating camera lookat position to reflect constraint so camera knows object limitations
void Car::checkCarBounds(){
	if(carX >= 46){carX = 46;}
	if(carZ >= 46){carZ = 46; }
	if(carX <= -46){carX = -46;}
	if(carZ <= -46){carZ = -46;}
}

void Car::checkMotion(){
	//make for intuitive backing up and steering by reversing step size sign if going backwards
	carHeadingStep = fabs(carHeadingStep);
	if (backwardsCarMotion){carHeadingStep = -fabs(carHeadingStep);}
}

void Car::updateHeading(){
	//get x and z components of heading vector to be applied to movement (based on car theta which is updated by a and d keys in timer)
	carDirX = -cos(carTheta*M_PI/180.0);
	carDirZ = sin(carTheta*M_PI/180.0);
}

void Car::handleAKey(){
	//a turns the car left
	carTheta += carHeadingStep;
}

void Car::handleDKey(){
	//d turns the car right
	carTheta -= carHeadingStep;
}

void Car::updateBackTorus(){
	//this gets called during the timer, simply is passive motion for torus on the back of car
	backTorusRotation += 2;
	//loop back to 0 once 360 is reached
	if (backTorusRotation == 360){
		backTorusRotation = 0;
	}
}

void Car::updateWheelMotion(){
	//this gets called in the timer function
	//if moving, rotate the wheels
	//direction of rotation handled in wheel drawing function
	if (forwardCarMotion || backwardsCarMotion){
		wheelRotation += 2;
		//loop back to 0 once 360 degrees reached
		if (wheelRotation == 360){
			wheelRotation = 0;
		}
	}
}

Vector3f Car::getCarNormal(){
	return carNormal;
}

void Car::setCarNormal(Vector3f vector){
	carNormal = vector;
}



























