#include <math.h>

#ifndef VECTOR3F_H
#define VECTOR3F_H

class Vector3f{
	private:
	float x, y, z;
	
	public:
	Vector3f();
	Vector3f(float _x, float _y, float _z);
	
	float getX();
	float getY();
	float getZ();
	float dotProduct(Vector3f otherVec);
	Vector3f crossProduct(Vector3f otherVec);
	void normalizeVec();
	float getAngleBetween(Vector3f otherVec);



	void setX(float);
	void setY(float);
	void setZ(float);
	
};

#endif