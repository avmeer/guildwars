#include "Camera.h"
#include <math.h>

#ifndef ARCBALLCAMERA_H
#define ARCBALLCAMERA_H

class ArcballCamera:public Camera
{
	public:
	//constructors
	
	//arcball camera has no meaning without object attached...defaults to looking at origin
	ArcballCamera(){computeArcballPositionAndSet();};
	
	//always set an arcball camera with the xyz of the object being looked at, handle the camera positioning later
	ArcballCamera(float ox, float oy, float oz):Camera(ox,oy,oz){computeArcballPositionAndSet();};
	
	//computes x y z of camera based on theta phi and radius
	void computeArcballPositionAndSet();
	
	//handle mouse dragging for changing orientation (based on user input)
	void handleCameraDrag(float xOld, float xNew, float yOld, float yNew);
	
	//handle user choosing to zoom
	void handleZoom(float xOld, float xNew, float yOld, float yNew);
};
#endif