#include "Point.h"
#include "Vector3f.h"

#ifndef HERO_H
#define HERO_H
class Hero{
	protected:
	//position
	float heroX;
	float heroY;
	float heroZ;

	Vector3f axisOfRotation;
	float theta;


	public:
	//setters for instance variables, not necessarily needed in A4 but useful for debugging
	void setX(float x){heroX = x;}
	void setY(float y){heroY = y;}
	void setZ(float z){heroZ = z;}
	
	void setAxisOfRotation(Vector3f v){axisOfRotation = v;};
	void setTheta(float t){theta=t;};

	float getX(){return heroX;}
	float getY(){return heroY;}
	float getZ(){return heroZ;}

	Point getPos(){return Point(heroX,heroY,heroZ);}

	Vector3f getAxisOfRotation(){return axisOfRotation;};
	float getTheta(){return theta;};


	virtual void draw(){};
	virtual void updateAnimation(){};
	
};

#endif