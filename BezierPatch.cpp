#include "BezierPatch.h"

BezierPatch::BezierPatch(){}

BezierPatch::BezierPatch(vector<Point> *pointVector){
	controlPoints = *pointVector;
	c0 = BezierCurve(controlPoints[0],controlPoints[1],controlPoints[2],controlPoints[3]);
	c1 = BezierCurve(controlPoints[4],controlPoints[5],controlPoints[6],controlPoints[7]);
	c2 = BezierCurve(controlPoints[8],controlPoints[9],controlPoints[10],controlPoints[11]);
	c3 = BezierCurve(controlPoints[12],controlPoints[13],controlPoints[14],controlPoints[15]);
}

void BezierPatch::populateBezierPatch(float u, float stepSize){
	c4 = BezierCurve(c0.evaluateBezierCurve(u), c1.evaluateBezierCurve(u), c2.evaluateBezierCurve(u), c3.evaluateBezierCurve(u));
	
	//add c4 to list of controlPoints
	
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

Point BezierPatch::dUBezier(const float &u, const float &v) 
{ 
   Point P[4]; 
   Point vCurve[4]; 
   for (int i = 0; i < 4; ++i) { 
       P[0] = controlPoints[i]; 
       P[1] = controlPoints[4 + i]; 
       P[2] = controlPoints[8 + i]; 
       P[3] = controlPoints[12 + i]; 
	   
	   BezierCurve bezCurve = BezierCurve(P[0], P[1], P[2], P[3]);
       vCurve[i] = bezCurve.evaluateBezierCurve(v); 
   } 
 
   return -3 * (1 - u) * (1 - u) * vCurve[0] + 
       (3 * (1 - u) * (1 - u) - 6 * u * (1 - u)) * vCurve[1] + 
       (6 * u * (1 - u) - 3 * u * u) * vCurve[2] + 
       3 * u * u * vCurve[3]; 
} 
 
Point BezierPatch::dVBezier(const float &u, const float &v) 
{ 
   Point P[4];
   Point uCurve[4]; 
   for (int i = 0; i < 4; ++i) {
	   P[0] = controlPoints[4*i]; 
	   P[1] = controlPoints[4*i + 1]; 
	   P[2] = controlPoints[4*i + 2]; 
	   P[3] = controlPoints[4*i + 3];
	   
	   BezierCurve bezCurve = BezierCurve(P[0], P[1], P[2], P[3]);
       uCurve[i] = bezCurve.evaluateBezierCurve(u); 
   } 
 
   return -3 * (1 - v) * (1 - v) * uCurve[0] + 
       (3 * (1 - v) * (1 - v) - 6 * v * (1 - v)) * uCurve[1] + 
       (6 * v * (1 - v) - 3 * v * v) * uCurve[2] + 
       3 * v * v * uCurve[3]; 
} 

float BezierPatch::getYPosition(const float &u, const float &v) 
{ 

	Point P[4];
    Point uCurve[4]; 
    for (int i = 0; i < 4; ++i){ 
    	P[0] = controlPoints[4*i]; 
	    P[1] = controlPoints[4*i + 1]; 
	    P[2] = controlPoints[4*i + 2]; 
	    P[3] = controlPoints[4*i + 3];
        BezierCurve bezCurve = BezierCurve(P[0], P[1], P[2], P[3]);
       uCurve[i] = bezCurve.evaluateBezierCurve(u); 
    }


    //fprintf(stdout, "\nx: %f y: %f z: %f\n", p.getX(), p.getY(), p.getZ());
    BezierCurve derpyBezCurve = BezierCurve(P[0], P[1], P[2], P[3]);
    return derpyBezCurve.evaluateBezierCurve(v).getY(); 
}