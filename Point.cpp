#include "Point.h"
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