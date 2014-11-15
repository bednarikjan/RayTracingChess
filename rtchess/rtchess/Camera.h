#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "Vector.h"

class Camera
{
public:	
	Camera() { }
	~Camera() { }
	
	Vector position;
	Vector direction;
	unsigned screenWidth;
	unsigned screenHeight;
	double fieldOfView;

private:
	double projectionPlaneDistance;

	static const unsigned width = 4;
	static const unsigned height = 3;
};

#endif