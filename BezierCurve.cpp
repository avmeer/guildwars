#include "BezierCurve.h"
//default constructor not used
BezierCurve::BezierCurve(){
	p0 = Point(0,0,0);
	p1 = Point(0,0,0);
	p2 = Point(0,0,0);
	p3 = Point(0,0,0);
}

//constructor with parameter that creates an instance of a bezier curve based on 4 points
BezierCurve::BezierCurve(Point p0, Point p1, Point p2, Point p3){
	this->p0 = p0;
	this->p1 = p1;
	this->p2 = p2;
	this->p3 = p3;
	
	//start curve with control cage and bez curve displayed
	displayControlCage = true;
	displayBezierCurve = true;

	controlPoints.push_back(p0);
	controlPoints.push_back(p1);
	controlPoints.push_back(p2);
	controlPoints.push_back(p3);
	
	//step size is 1/128 for good resolution of curve
	this->stepSize = 0.0078125f;
}

//evaluate a point along the bezier curve based on interpolation value t (helps with sprite anim and rendering bez curve)
Point BezierCurve::evaluateBezierCurve(float t){
	return Point((-1.0f*p0 + 3.0f*p1 + -3.0f*p2 + p3)*pow(t,3) + (3.0f*p0 + -6.0f*p1 + 3.0f*p2)*pow(t,2) + (-3.0f*p0 + 3.0f*p1)*t + p0);
}

//not used, but may be used in future assignments/labs to orient along tangent of curve
Vector3f BezierCurve::evaluateTangentPoint(float t){
	Point p = Point(3.0f*(-1.0f*p0 + 3.0f*p1 + -3.0f*p2 + p3)*pow(t,2) + 2.0f*(3.0f*p0 + -6.0f*p1 + 3.0f*p2)*t + (-3.0f*p0 + 3.0f*p1));
	Vector3f tempVec = Vector3f(p.getX(), p.getY(), p.getZ());
	tempVec.normalizeVec();
	return tempVec;
}

//function to actually display the bezier curve
void BezierCurve::renderBezierCurve(){
	//check flag which is set by user
	if (displayBezierCurve){
		//disable lighting (working with glut primitives)
		glDisable(GL_LIGHTING);
		//make lines thicker so visible from far
		glLineWidth(3.0f);
		Point point;
		
		//make curve a nice blue color
		glColor3f( 0, 0, 1 );
		
		//start drawing line strips
		//vertices of line strips based on stipping from 0 to 1 and passing in the interpolated value to the evaluatePoint function
		glBegin( GL_LINE_STRIP );                 
		for(float i = 0.0f; i <= 1.0f; i += stepSize){
			point = evaluateBezierCurve(i);
			glVertex3f(point.getX(),point.getY(),point.getZ());
		}
		glEnd();
		//re enable lighting
		glEnable(GL_LIGHTING);
		//reset line width for future
		glLineWidth(1.0f);
	}
}

//function that draws lines between the control points
void BezierCurve::connectControlPoints(){
	//check flag which is set by user
	if(displayControlCage){
		//disable lighting (working with glut primitives)
		glDisable(GL_LIGHTING);
		//nice yellow color
		glColor3ub(255,255,0);
		//make lines thick enough to see
		glLineWidth(3.0f);
		//start drawing line strips, the vertices are simply based on p0-p3 instance variables (control points)
		glBegin(GL_LINE_STRIP);
		for (unsigned int i = 0; i < controlPoints.size(); i++){
			glVertex3f(controlPoints[i].getX(), controlPoints[i].getY(), controlPoints[i].getZ());
		}
		glEnd();
		//reset lighting and line width
		glEnable(GL_LIGHTING);
		glLineWidth(1.0f);
	}
}

//function to display control points as little green spheres
void BezierCurve::drawControlPoints(){
	//check flag (set by user)
	if(displayControlCage){
		//nice green color
		glColor3ub(0,230,0);
		
		//for each control point, translate to that points xyz values and draw a small sphere
		for (unsigned int i = 0; i < controlPoints.size(); i++){
			glPushMatrix();
			glTranslatef(controlPoints[i].getX(), controlPoints[i].getY(), controlPoints[i].getZ());
			glutSolidSphere(0.2,20,20);
			glPopMatrix();
		}
	}
}

//functions for toggling displays of control cage and bezier curves (called based on user interaction)
void BezierCurve::toggleControlCage(){displayControlCage = !displayControlCage;}
void BezierCurve::toggleCurve(){displayBezierCurve = !displayBezierCurve;}









