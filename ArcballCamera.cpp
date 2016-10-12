#include "ArcballCamera.h"

//get xyz coordinates to place camera based on our theta and phi values
void ArcballCamera::computeArcballPositionAndSet(){
	cameraX = cameraRadius * sinf(cameraTheta)*sinf(cameraPhi);
    cameraY = cameraRadius * -cosf(cameraPhi);
    cameraZ = cameraRadius * -cosf(cameraTheta)*sinf(cameraPhi);
    recomputeOrientation();
}

//handle mouse dragging for changing orientation
void ArcballCamera::handleCameraDrag(float xOld, float xNew, float yOld, float yNew){
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
void ArcballCamera::handleZoom(float xOld, float xNew, float yOld, float yNew){
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