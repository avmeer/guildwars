//wont compile without these which is annoying
/////////////////////////

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

#include "Point.h"
#include "Vector3f.h"
#include "Hero.h"
////////////////////////

//header file for drawing a car object

#ifndef CAR_H
#define CAR_H
class Car:public Hero{
	private:
	//heading
	float carTheta;
	float carDirZ;
	float carDirX;

	Vector3f carNormal;
	
	//values updated by timer for passive info
	float backTorusRotation;
	float wheelRotation;
	
	//steps for animations
	float carHeadingStep;
	float carTranslationStepSize;
	
	//direction bools
	bool forwardCarMotion;
	bool backwardsCarMotion;
	
	public:
	
	//default constructor and constructor with position, heading, and rotation values to be defined
	Car();
	Car(float x, float y, float z, float theta, float dirZ, float dirX, float backTRot);
	
	//setters for instance variables, not necessarily needed in A4 but useful for debugging
	void setTheta(float theta);
	void setDirZ(float z);
	void setDirX(float x);
	void setBackTorusRotation(float btr);
	void setWheelRotation(float wr);
	void setCarHeadingStep(float chs);
	void setCarTranslationStepSize(float ctss);
	void setForwardCarMotion(bool fcm);
	void setBackwardsCarMotion(bool bcm);

	void setCarNormal(Vector3f);
	Vector3f getCarNormal();
	
	//getters for instance variables, not necessarily needed in A4 but useful for debugging
	Point getPos();
	float getTheta();
	float getDirZ();
	float getDirX();
	float getBackTorusRotation();
	float getWheelRotation();
	float getCarHeadingStep();
	float getCarTranslationStepSize();
	bool getForwardCarMotion();
	bool getBackwardsCarMotion();
	
	//functions for drawing the various parts that make up a car
	void draw();
	void drawUnitRightTrianglePrism();
	void drawBallWheel();
	void drawAxle();
	void drawBackMagicalSpinnyThingy();
	void drawCarBody();

	void updateAnimation();

	void drawHeroName();
	
	//functions for handling interaction during execution (based on timers, user input etc)
	void handleWKey();
	void handleSKey();
	void checkCarBounds();
	void checkMotion();
	void handleAKey();
	void handleDKey();
	void updateBackTorus();
	void updateWheelMotion();
	void updateHeading();
};
#endif











