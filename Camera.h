#ifndef CAMERA_H
#define CAMERA_H
class Camera{
	private:
	float cameraX, cameraY, cameraZ; // camera position in cartesian coordinates
	float cameraTheta, cameraPhi;    // camera DIRECTION in spherical coordinates
	float objX, objY, objZ;          // lookat point in cartesian coordinates
	float cameraRadius;	 			 //Scale factor for camera distance away from character (used for zoom)
	
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
	
	//setters
	void setObjX(float x);
	void setObjY(float y);
	void setObjZ(float z);
	
	//computes x y z of camera based on theta phi and radius
	void computeArcballPositionAndSet();
	
	//handle mouse dragging for changing orientation (based on user input)
	void handleCameraDrag(float xOld, float xNew, float yOld, float yNew);
	
	//handle user choosing to zoom
	void handleZoom(float xOld, float xNew, float yOld, float yNew);
	
	//update object location if object location changes
	void updateObjectLocation(float x, float y, float z);
	
	//function to return camerainfo to be used in gluLookat function
	float* getCameraInfo();
};
#endif