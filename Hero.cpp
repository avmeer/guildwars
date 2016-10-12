#include "Hero.h"

void Hero::setX(float x){heroX = x;}
void Hero::setY(float y){heroY = y;}
void Hero::setZ(float z){heroZ = z;}

float Hero::getX(){return heroX;}
float Hero::getY(){return heroY;}
float Hero::getZ(){return heroZ;}

Point Hero::getPos(){return Point(heroX,heroY,heroZ);}