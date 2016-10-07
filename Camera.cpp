#include "Camera.h"
#include <math.h>

Camera::Camera(){
	objX = 0;
	objY = 0;
	objZ = 0;
	
	//pretty starting point
	cameraRadius = 12.0f;
    cameraTheta = 2.80;
    cameraPhi = 2.0;
	
	//recompute x y z based on object
	computeArcballPositionAndSet();
}

Camera::Camera(float ox, float oy, float oz){
	
	objX = ox;
	objY = oy;
	objZ = oz;
	
	//pretty starting point
	cameraRadius = 12.0f;
    cameraTheta = 2.80;
    cameraPhi = 2.0;
	
	//recompute x y z based on object
	computeArcballPositionAndSet();
}

//get xyz coordinates to place camera based on our theta and phi values
void Camera::computeArcballPositionAndSet(){
	cameraX = cameraRadius * sinf(cameraTheta)*sinf(cameraPhi);
    cameraY = cameraRadius * -cosf(cameraPhi);
    cameraZ = cameraRadius * -cosf(cameraTheta)*sinf(cameraPhi);
}

//handle mouse dragging for changing orientation
void Camera::handleCameraDrag(float xOld, float xNew, float yOld, float yNew){
	cameraTheta += (xNew - xOld)*0.005f;
	cameraPhi += (yNew - yOld)*0.005f;
	
	// we don't care if theta goes out of bounds; it'll just loop around.
	// phi, though.. it'll flip out if it flips over top. keep it in (0, M_PI)
	if(cameraPhi <= 0)
		cameraPhi = 0+0.001;
	if(cameraPhi >= M_PI)
		cameraPhi = M_PI-0.001;
	
	computeArcballPositionAndSet();
}
	
	//user interaction will call this function if ctrl click is used (pass in mouse coordinates to determine zoom amount)
void Camera::handleZoom(float xOld, float xNew, float yOld, float yNew){
	double totalChangeSq = (xNew - xOld) + (yNew - yOld);
	cameraRadius += totalChangeSq*0.03;
		
	//limit the camera radius to some reasonable values so the user can't get lost
	if(cameraRadius < 2.0) 
		cameraRadius = 2.0;
	if(cameraRadius > 50.0) 
		cameraRadius = 50.0;
	
	//update camera (x,y,z) based on (radius,theta,phi)
	computeArcballPositionAndSet();
}

//setters for xyz, dont need in a4 but useful for debugging
void Camera::setObjX(float x){
	objX = x;
}
void Camera::setObjY(float y){
	objY = y;
}
void Camera::setObjZ(float z){
	objZ = z;
}

//function that returns the required information for gluLookat() in an array
float* Camera::getCameraInfo(){
	//array to store info
	float cameraInfo[9];
	
	//top row of our glulookatarray (camera position + lookat point)
	cameraInfo[0] = cameraX + objX;
	cameraInfo[1] = cameraY + objY;
	cameraInfo[2] = cameraZ + objZ;
	//middle row (lookat point)
	cameraInfo[3] = objX;
	cameraInfo[4] = objY;
	cameraInfo[5] = objZ;
	//bottom row (up vector)
	cameraInfo[6] = 0.0f;
	cameraInfo[7] = 1.0f;
	cameraInfo[8] = 0.0f;
	
	return cameraInfo;
}

//getters (not needed but useful for debugging)
float Camera::getX(){return cameraX;}
float Camera::getY(){return cameraY;}
float Camera::getZ(){return cameraZ;}










