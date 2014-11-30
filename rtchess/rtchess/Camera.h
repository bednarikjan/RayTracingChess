#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "Vector3d.h"

#define PI 3.14159265
#define DG_2_RAD(x) ((x) / 180.0 * PI)

//! Class implementing camera for viewing the scene
/*!
	The projection screen is actually virtual and is not anyhow
	implemented in teh scene. It is only used to calculate the direction
	of each ray to be traced. The screen is perpendicular to the y-axis
	and its center S = [0, 1, 0].
	
	So far, the camera is placed in [0,0,0] in caretsian coordinate system and
	it looks at y-axis.

	TODO

	Either leave this as is and rotate all objects in scene, when user
	wants to move the camera, or implement generic projection screen.
*/
class Camera
{
public:		
	const Point screenCenter;		
	Point position_;
	Vector3d direction_;
	
	//! Constructor
	Camera(Vector3d position, Vector3d direction, 
			unsigned horizontalPixels, unsigned verticalPixels, 
			double fieldOfView) : 
			position_(position), direction_(direction),
			horizontalPixels_(horizontalPixels), verticalPixels_(verticalPixels),
			fieldOfView_(fieldOfView), screenCenter(Vector3d(0.0, 1.0, 0.0))
	{ 
		updatePxStep();
	}
	
	//! Destructor
	~Camera() { }	

	void setResolution(unsigned horizontalPixels, unsigned verticalPixels)
	{
		horizontalPixels_ = horizontalPixels;
		verticalPixels_ = verticalPixels;
		updatePxStep();
	}

	unsigned getScreenWidth() { return horizontalPixels_; }
	unsigned getScreenHeight() { return verticalPixels_; }
	 
	void setFieldOfView(double horizontalAngle) 
	{
		fieldOfView_ = horizontalAngle;
		updatePxStep();
	}

	double getFieldOfView() { return fieldOfView_; }

	double getPxStep() 
	{
		return pxStep;
	}

private:
	unsigned horizontalPixels_;
	unsigned verticalPixels_;
	double fieldOfView_;
	double pxStep;

	//! Recounts the real world distance between pixels on virtual projection screen.
	void updatePxStep();
};

inline void Camera::updatePxStep() 
{
	pxStep = screenCenter.y_ * tan(DG_2_RAD(fieldOfView_ / 2.0)) / (horizontalPixels_ / 2.0);	
}

#endif