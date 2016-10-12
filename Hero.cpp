#include "Hero.h"

#ifndef HERO_H
#define HERO_H
class Hero{
	protected:
	//position
	float carX;
	float carY;
	float carZ;
	
	//heading
	float carTheta;
	float carDirZ;
	float carDirX;

	virtual void draw();
};

#endif