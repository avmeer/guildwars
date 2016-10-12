#include "Camera.h"
#include <math.h>

#ifndef FREECAM_H
#define FREECAM_H

class FreeCamera:public Camera
{
	private:
	float stepSize;

	public:
	//constructors
	
	//arcball camera has no meaning without object attached...defaults to looking at origin
	FreeCamera();
	
	//always set an arcball camera with the xyz of the object being looked at, handle the camera positioning later
	FreeCamera(float cx, float cy, float cz);
	
	//computes x y z of camera based on theta phi and radius
	void recomputeOrientation();
	
	//handle mouse dragging for changing orientation (based on user input)
	void handleCameraDrag(float xOld, float xNew, float yOld, float yNew);

	float* getCameraInfo();
	void handleForwardKey();
	void handleBackwardsKey();
};
#endif