
#ifdef __APPLE__			// if compiling on Mac OS
	#include <GLUT/glut.h>
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
#else					// else compiling on Linux OS
	#include <GL/glut.h>
	#include <GL/gl.h>
	#include <GL/glu.h>
#endif

//sprite class that constantly revolves around an object based on any bezier curve collection

#ifndef SPRITE_H
#define SPRITE_H
class Sprite{
	private:
	//radius values for passive animation of sphere
	float spriteRadius;
	float spriteRadiusStepSize;
	
	//value to be updated for passive animation of torus
	float torusRot;
	
	//position that gets updated and translated along bezier curve
	float x;
	float y;
	float z;
	
	public:
	Sprite();
	
	//functions for drawing various components of sprite
	void drawSprite();
	void drawSpriteCenter();
	void drawInnerTorus();
	void drawOuterTorus();
	
	//function to update the passive animation of the sprite
	void updateSprite();
};
#endif