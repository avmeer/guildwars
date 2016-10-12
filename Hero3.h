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
#include "Hero.h"
#include "Vector3f.h"

#ifndef HERO3_H
#define HERO3_H

class Hero3:public Hero
{
	protected:
	bool wingFlap;
	int wingAngle;
	public:
	//constructors
	Hero3();

	void draw();
	void drawTriangle();
	void drawWings();
	void drawHead();
	void drawEye();
	void wingTimer(int value);
};

#endif