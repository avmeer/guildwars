#include "BezierPatch.h"

BezierPatch::BezierPatch(){}

BezierPatch::BezierPatch(vector<Point> *pointVector){
	points = *pointVector;
	c0 = BezierCurve(points[0],points[1],points[2],points[3]);
	c1 = BezierCurve(points[4],points[5],points[6],points[7]);
	c2 = BezierCurve(points[8],points[9],points[10],points[11]);
	c3 = BezierCurve(points[12],points[13],points[14],points[15]);
	
}

void BezierPatch::renderBezierPatch(float u, float v){
	c4 = BezierCurve(c0.evaluateBezierCurve(u), c1.evaluateBezierCurve(u), c2.evaluateBezierCurve(u), c3.evaluateBezierCurve(u));
	
	//c0.renderBezierCurve();
	//c1.renderBezierCurve();
	//c2.renderBezierCurve();
	//c3.renderBezierCurve();
	
	c4.renderBezierCurve();
}