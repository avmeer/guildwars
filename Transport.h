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

#ifndef TRANSPORT_H
#define TRANSPORT_H

class Transport:public Hero
{
	protected:
	float sphereRadius;
	float scaleFactor;
	float engineRotation;
	float dodecDisplacement;
	float transportOffset;
	bool hoverUp;

	public:
	//constructors
	Transport();

	void draw();
	void drawEngine();
	void drawCockpit();
	void drawDisk();
	void wingAnimate();

	void updateAnimation();

	void drawHeroName();
};

#endif