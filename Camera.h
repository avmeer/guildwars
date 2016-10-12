#include "Point.h"

#ifndef CAMERA_H
#define CAMERA_H
class Camera{
	protected:
	float cameraX, cameraY, cameraZ; // camera position in cartesian coordinates
	float cameraTheta, cameraPhi;    // camera DIRECTION in spherical coordinates
	float objX, objY, objZ;          // lookat point in cartesian coordinates
	float cameraRadius;	 			 //Scale factor for camera distance away from character (used for zoom)
	float cameraDirX, cameraDirY, cameraDirZ;
	public:
	
	//constructors
	
	//arcball camera has no meaning without object attached...defaults to looking at origin
	Camera();
	
	//always set an arcball camera with the xyz of the object being looked at, handle the camera positioning later
	Camera(float ox, float oy, float oz);
	
 	//getters
	float getX();
	float getY();
	float getZ();
	float getDirX(){return cameraDirX;};
	float getDirY(){return cameraDirY;};
	float getDirZ(){return cameraDirZ;};
	
	//setters
	void setObjX(float x);
	void setObjY(float y);
	void setObjZ(float z);
	
	void setObjPos(Point p);
	
	
	
	//update object location if object location changes
	void updateObjectLocation(float x, float y, float z);
	
	//function to return camerainfo to be used in gluLookat function
	float* getCameraInfo();

	void recomputeOrientation();
};
#endif