#include "Vector3f.h"

Vector3f::Vector3f(){
	x = 0;
	y = 0;
	z = 0;
}

Vector3f::Vector3f(float _x, float _y, float _z){
	x = _x;
	y = _y;
	z = _z;
}

float Vector3f::getX(){
	return x;
}
float Vector3f::getY(){
	return y;
}
float Vector3f::getZ(){
	return z;
}

void Vector3f::normalizeVec(){
	float magnitude = sqrt(x*x + y*y + z*z);
	x = x/magnitude;
	y = y/magnitude;
	z = z/magnitude;
}

float Vector3f::dotProduct(Vector3f otherVec){
	return x*otherVec.getX() + y*otherVec.getY() + z*otherVec.getZ();
}
Vector3f Vector3f::crossProduct(Vector3f otherVec){
	float a1 = x;
	float a2 = y;
	float a3 = z;
	float b1 = otherVec.getX();
	float b2 = otherVec.getY();
	float b3 = otherVec.getZ();
	float xNew = a2*b3 - a3*b2;
	float yNew = a3 * b1 - a1 * b3;
	float zNew = a1*b2 - a2*b1;
	if (yNew < 0) {
		xNew *= -1;
		yNew *= -1;
		zNew *= -1;
	}
	Vector3f crossP = Vector3f(xNew, yNew, zNew);
	crossP.normalizeVec();
	return crossP;
}

float Vector3f::getAngleBetween(Vector3f otherVec){
	this->normalizeVec();
	otherVec.normalizeVec();
	float dotP = this->dotProduct(otherVec);
	float angle = acos(dotP);
	angle = angle * 180.0 / M_PI;
	//if (angle > 90.0){angle = otherVec.getAngleBetween(*this);}
	return angle;
}