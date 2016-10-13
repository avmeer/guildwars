//header file for simple point class with xyz data and some point operations defined

#ifndef POINT_H
#define POINT_H
class Point{
	public:
	float getX();
	float getY();
	float getZ();
	Point();
	Point(float _x, float _y, float _z);
	void setXYZ(float _x, float _y, float _z);

	float distance(Point);
	
	private:
	float x, y, z;
};

Point operator*( Point p, float f ); // multiplies a Point by a float
Point operator*( float f, Point p ); // multiplies a float by a Point
Point operator+( Point a, Point b ); // adds two Points together
Point operator-( Point a, Point b ); // adds two Points together
#endif