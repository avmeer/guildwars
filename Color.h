#ifdef __APPLE__			// if compiling on Mac OS
	#include <GLUT/glut.h>
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
#else					    // else compiling on Linux OS
	#include <GL/glut.h>
	#include <GL/gl.h>
	#include <GL/glu.h>
#endif

#include <math.h>

#ifndef COLOR_H
#define COLOR_H

class Color
{

	private:

		float red;
		float green;
		float blue;

	public:
		Color();
		Color( float initRed, float initGreen, float initBlue );

		void setColor();

		float get_red();
		float get_green();
		float get_blue();

		// Setters:
		void set_red( float newRed );
		void set_green( float newGreen );
		void set_blue( float newBlue );
		
};

#endif // COLOR_H