
#ifndef HERO_H
#define HERO_H
class Hero{
	protected:
	//position
	float carX;
	float carY;
	float carZ;

	public:
	//setters for instance variables, not necessarily needed in A4 but useful for debugging
	void setX(float x);
	void setY(float y);
	void setZ(float z);

	//getters for instance variables, not necessarily needed in A4 but useful for debugging
	float getX();
	float getY();
	float getZ();

	virtual void draw(){};
	virtual void updateAnimation(){};
};

#endif