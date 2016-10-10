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
}

Camera::Camera(float ox, float oy, float oz){
	
	objX = ox;
	objY = oy;
	objZ = oz;
	
	//pretty starting point
	cameraRadius = 12.0f;
    cameraTheta = 2.80;
    cameraPhi = 2.0;

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

void Camera::setObjPos(Point p){
	objX = p.getX();
	objY = p.getY();
	objZ = p.getZ();
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










