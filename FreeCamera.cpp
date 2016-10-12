#include "FreeCamera.h"

FreeCamera::FreeCamera(){
    cameraX = 60;
    cameraY = 40;
    cameraZ = 30;
    cameraTheta = -M_PI / 3.0f;
    cameraPhi = M_PI / 2.8f;
}
	
//always set an arcball camera with the xyz of the object being looked at, handle the camera positioning later
FreeCamera::FreeCamera(float cx, float cy, float cz){
	cameraX = cx;
	cameraY = cy;
	cameraZ = cz;
	cameraTheta = -M_PI / 3.0f;
    cameraPhi = M_PI / 2.8f;
}

//handle mouse dragging for changing orientation (based on user input)
void FreeCamera::handleCameraDrag(float xOld, float xNew, float yOld, float yNew){
	cameraTheta += (xNew - xOld)*0.005f;
	cameraPhi += (yOld - yNew)*0.005f;
	
	// we don't care if theta goes out of bounds; it'll just loop around.
	// phi, though.. it'll flip out if it flips over top. keep it in (0, M_PI)
	if(cameraPhi <= 0)
		cameraPhi = 0+0.001;
	if(cameraPhi >= M_PI)
		cameraPhi = M_PI-0.001;
	
	recomputeOrientation();
}

void FreeCamera::recomputeOrientation() {
    // TODO #5: Convert spherical coordinates into a cartesian vector
    // see Wednesday's slides for equations.  Extra Hint: Slide #34
	cameraDirX = sin(cameraTheta)*sin(cameraPhi);
	cameraDirY = -cos(cameraPhi);
	cameraDirZ = -cos(cameraTheta)*sin(cameraPhi);
	// and NORMALIZE this directional vector!!!
	float magnitude = sqrt(cameraDirX*cameraDirX + cameraDirY*cameraDirY + cameraDirZ*cameraDirZ);
	cameraDirX = cameraDirX / magnitude;
	cameraDirY = cameraDirY / magnitude;
	cameraDirZ = cameraDirZ / magnitude;
}

float* FreeCamera::getCameraInfo(){
	//array to store info
	float cameraInfo[9];
	
	//top row of our glulookatarray (camera position + lookat point)
	cameraInfo[0] = cameraX;
	cameraInfo[1] = cameraY;
	cameraInfo[2] = cameraZ;
	//middle row (lookat point)
	cameraInfo[3] = cameraX + cameraDirX;
	cameraInfo[4] = cameraY + cameraDirY;
	cameraInfo[5] = cameraZ + cameraDirZ;
	//bottom row (up vector)
	cameraInfo[6] = 0.0f;
	cameraInfo[7] = 1.0f;
	cameraInfo[8] = 0.0f;
	
	return cameraInfo;
}