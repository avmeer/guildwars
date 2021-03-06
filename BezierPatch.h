#include "BezierCurve.h"
#include "Point.h"
#include <vector>

class BezierPatch{
	private:
	vector<Point> controlPoints;
	vector<Point> curvePoints;
	BezierCurve c0, c1, c2, c3, c4;
	Point tempPoint;

	public:
	BezierPatch();
	BezierPatch(vector<Point> *points);
	void populateBezierPatch(float u, float stepSize);
	vector<Point>* getCurvePoints();
	
	Point dUBezier(const float &u, const float &v); 
	Point dVBezier(const float &u, const float &v); 
	float getYPosition(const float &u, const float &v);
};