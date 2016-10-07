#include "BezierCurve.h"
#include "Point.h"
#include <vector>

BezierPatch{
	private:
	vector<Point> points;
	
	public:
	BezierPatch();
	BezierPatch(vector<Point> *points);
	createBezierPatch(vector<Point> points, u, v);
};