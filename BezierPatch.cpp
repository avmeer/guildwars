#include "BezierPatch.h"

BezierPatch::BezierPatch(){}

BezierPatch::BezierPatch(vector<Point> *pointVector){
	points = *pointVector;
	c0 = BezierCurve(points[0],points[1],points[2],points[3]);
	c1 = BezierCurve(points[4],points[5],points[6],points[7]);
	c2 = BezierCurve(points[8],points[9],points[10],points[11]);
	c3 = BezierCurve(points[12],points[13],points[14],points[15]);
}

void BezierPatch::populateBezierPatch(float u, float stepSize){
	c4 = BezierCurve(c0.evaluateBezierCurve(u), c1.evaluateBezierCurve(u), c2.evaluateBezierCurve(u), c3.evaluateBezierCurve(u));
	
	//add c4 to list of points
	
	//c0.renderBezierCurve();
	//c1.renderBezierCurve();
	//c2.renderBezierCurve();
	//c3.renderBezierCurve();
	 for (float i = 0; i < 1.0; i+= stepSize){
		tempPoint = c4.evaluateBezierCurve(i);
		curvePoints.push_back(tempPoint);
	}
	
	//curvePoints.push_back(tempPoint);
	//fprintf(stdout, "%d", curvePoints.size());
	
	
	c4.renderBezierCurve();
}

vector<Point>* BezierPatch::getCurvePoints(){
	return &curvePoints;
}