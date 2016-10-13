#include "Point.h"
#include <math.h>

Point::Point(){
	x = 0;
	y = 0;
	z = 0;
}

Point::Point(float _x, float _y, float _z){
	x = _x;
	y = _y;
	z = _z;
}

//get xyz values of the point
float Point::getX(){return x;}
float Point::getY(){return y;}
float Point::getZ(){return z;}

void Point::setXYZ(float _x, float _y, float _z){
	x = _x;
	y= _y;
	z = _z;
}

// multiplies a Point by a float
Point operator*( Point p, float f ){
	return Point( p.getX() * f, p.getY() * f, p.getZ() * f );
} 

// multiplies a float by a Point
Point operator*( float f, Point p ){
	return Point( p.getX() * f, p.getY() * f, p.getZ() * f );
}

// adds two Points together
Point operator+( Point a, Point b ){
	return Point( a.getX() + b.getX(), a.getY() + b.getY(), a.getZ() + b.getZ() );
}

// adds two Points together
Point operator-( Point a, Point b ){
	return Point( a.getX() - b.getX(), a.getY() - b.getY(), a.getZ() - b.getZ() );
}

float Point::distance(Point otherPoint){
	float x2 = otherPoint.getX();
	float y2 = otherPoint.getY();
	float z2 = otherPoint.getZ();
	return sqrt((x - x2)*(x - x2) + (y - y2)*(y - y2) + (z - z2)*(z - z2));
}