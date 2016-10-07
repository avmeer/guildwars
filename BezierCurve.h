// HEADERS /////////////////////////////////////////////////////////////////////
// OpenGL / GLUT Libraries we need
#ifdef __APPLE__			// if compiling on Mac OS
	#include <GLUT/glut.h>
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
#else					// else compiling on Linux OS
	#include <GL/glut.h>
	#include <GL/gl.h>
	#include <GL/glu.h>
#endif

#include <GL/glui.h>			// include our GLUI header


// C Libraries we need
#include <time.h>			// for time() to seed our RNG
#include <stdio.h>			// allow to print to terminal
#include <stdlib.h>			// access to rand() and exit()
#include <math.h>			// for cosf(), sinf(), etc.


// C++ Libraries we'll use
#include <fstream>			// we'll use ifstream	
#include <string>			// for, well strings!
#include <vector>			// and vectors (the storage container, not directional)
//////////////////////////////////////////

#include "Point.h"
using namespace std;

#ifndef BEZIERCURVE_H
#define BEZIERCURVE_H
class BezierCurve{
	private:
	//control points
	Point p0;
	Point p1;
	Point p2;
	Point p3;
	
	//vector for easily iterating through control points
	vector<Point> controlPoints;
	
	//resolution of curves
	int resolution;
	
	//stepSize based on resolution, but simply setting to a nice size for A4
	float stepSize;
	
	//booleans for displaying attributes of curve
	bool displayControlCage;
	bool displayBezierCurve;
	
	public:
	//default constructor or constructor based on 4 points (latter more useful)
	BezierCurve();
	BezierCurve(Point p0, Point p1, Point p2, Point p3);
	
	//function for evaluating a point on the bezier curve based on interpolated value passed in (and internal control points)
	Point evaluateBezierCurve(float t);
	
	//function for evaluating the tangent to the bezier curve, not used in A4 but will be used later
	Point evaluateTangentPoint(float t);
	
	//functions for drawing the curve, control points, cage, and toggling them.
	void renderBezierCurve();
	void connectControlPoints();
	void drawControlPoints();
	void toggleControlCage();
	void toggleCurve();
};
#endif