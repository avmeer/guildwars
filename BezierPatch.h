#include "BezierCurve.h"
#include "Point.h"
#include <vector>

class BezierPatch{
	private:
	vector<Point> points;
	BezierCurve c0, c1, c2, c3, c4;

	public:
	BezierPatch();
	BezierPatch(vector<Point> *points);
	void renderBezierPatch(float u, float v);
};