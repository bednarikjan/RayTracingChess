#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "Vector.h"

class Camera
{
public:		
	//! Constructor
	Camera(Vector position, Vector direction, 
			unsigned screenWidth, unsigned screenHeight, 
			double fieldOfView) : 
			position_(position), direction_(direction),
			screenWidth_(screenWidth), screenHeight_(screenHeight),
			fieldOfView_(fieldOfView) 
	{ 
		// TODO Calculate projection plane distance			

	}
	
	//! Destructor
	~Camera() { }
	
	Vector position_;
	Vector direction_;
	unsigned screenWidth_;
	unsigned screenHeight_;
	double fieldOfView_;

private:
	double projectionPlaneDistance;

	static const unsigned width = 4;
	static const unsigned height = 3;
};

#endif