#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "Vector3d.h"
#include <Eigen\Dense>

#define PI 3.14159265
#define DG_2_RAD(x) ((x) / 180.0 * PI)

using namespace std;

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
	
	//! Constructor
	Camera(Vector3d position, Vector3d direction, 
			unsigned horizontalPixels, unsigned verticalPixels, 
			double fieldOfView) : 
			position_(position), direction_(direction.normalize()),
			horizontalPixels_(horizontalPixels), verticalPixels_(verticalPixels),
			fieldOfView_(fieldOfView), screenCenter(Vector3d(0.0, 1.0, 0.0)), 
			fixedPosition(Vector3d(0.0, 0.0, 0.0)),
			fixedDirection(Vector3d(0.0, 1.0, 0.0))
	{ 
		updatePxStep();
	}
	
	//! Destructor
	~Camera() { }	

	Point& position() { return position_; }
	void setPosition(Point position) {
		position_ = position;
		updatePxStep();
	}

	Vector3d& direction() { return direction_; }
	void setDirection(Vector3d direction) {
		direction_ = direction.normalize();
		updatePxStep();
	}

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

	Point getTopLeftPX() { return TLPx; }
	Vector3d& getWidthStep() { return xStep; } 
	Vector3d& getHeightStep() { return zStep; } 

private:
	Point position_;
	Vector3d direction_;
	Vector3d fixedPosition;
	Vector3d fixedDirection;	
	
	unsigned horizontalPixels_;
	unsigned verticalPixels_;
	double fieldOfView_;
	
	Point TLPx;
	Vector3d xStep;
	Vector3d zStep;

	double pxStep;

	//! Recounts the real world distance between pixels on virtual projection screen.
	void updatePxStep();
};

inline void Camera::updatePxStep() 
{	
	// debug
	fieldOfView_ = 45.0;
	horizontalPixels_ = 160;
	verticalPixels_ = 120;

	position_  = Point(-6.0, -3.0, 6.0);
	direction_ = Vector3d(4.0, 3.0, -2.0).normalize();
	//direction_ = Vector3d(0.0, -1.0, -0.2).normalize();
	
	// original inter-px distance when projection plane is perpendicular to y-axis
	pxStep = fixedDirection.y_ * tan(DG_2_RAD(fieldOfView_ / 2.0)) / (horizontalPixels_ / 2.0);	

	Eigen::RowVector3d dir; dir << fixedDirection.x_, fixedDirection.y_, fixedDirection.z_;
	Eigen::RowVector3d dirNew; dirNew << direction_.x_, direction_.y_, direction_.z_;
	
	Eigen::RowVector3d dirProjZ = dirNew;
	dirProjZ(2) = 0.0;
	dirProjZ.normalize();

	double angleZ = acos(max(min(dirProjZ.dot(dir), 1.0), -1.0));
	//double angleZ = acos(dirProjZ.dot(dir));

	// Check if we should rotate clockwise or counter-clockwise
	if(dirProjZ.cross(dir)(2) < 0.0) angleZ = -angleZ;

	// Rz - rotation along Z-axis, radians!
	Eigen::Matrix4d Rz(4, 4); Rz <<  cos(angleZ),  -sin(angleZ), 0.0, 0.0,
									 sin(angleZ),  cos(angleZ),  0.0, 0.0,
									 0.0,          0.0,          1.0, 0.0,
									 0.0,          0.0,          0.0, 1.0;

	Eigen::RowVector4d dirNew4; dirNew4 << dirNew(0), dirNew(1), dirNew(2), 1.0;
	Eigen::RowVector4d dirProjX = dirNew4 * Rz.inverse();
	Eigen::RowVector3d dirProjX3; dirProjX3 << dirProjX(0), dirProjX(1), dirProjX(2);

	double angleX = acos(max(min(dirProjX3.dot(dir), 1.0), -1.0));
	//double angleX = acos(dirProjX3.dot(dir));

	/*cout << "dirProjX3: " << dirProjX3 << endl;
	cout << "dir: " << dir << endl;
	cout << "dot(dirProjX3, dir): " << dirProjX3.dot(dir) << endl;
	cout << "acos: " << acos(1.1) << endl;	

	cout << "angleX: " << angleX << endl;*/

	// Check if we should rotate clockwise or counter-clockwise
	if(dirProjX3.cross(dir)(0) < 0.0) angleX = -angleX;

	// Rx - rotation along X-axis, radians!
	Eigen::Matrix4d Rx(4, 4); Rx <<   1.0, 0.0,         0.0,		  0.0,
									  0.0, cos(angleX), -sin(angleX), 0.0,
									  0.0, sin(angleX), cos(angleX),  0.0,
									  0.0, 0.0,         0.0,          1.0;	

	// T - Translation matrix
	Eigen::Matrix4d T(4, 4); T <<	1.0,		  0.0,			0.0,		  0.0,
									0.0,		  1.0,			0.0,		  0.0,
									0.0,		  0.0,			1.0,		  0.0,
									position_.x_, position_.y_, position_.z_, 1.0;		

	// debug
	/*cout << "Rx: " << Rx << endl;
	cout << "Rz: " << Rz << endl;
	cout << "T: " << T << endl;*/

	// Resultion rotation matrix
	Eigen::Matrix4d Mr(4, 4); Mr = Rx * Rz;
	// Resultion translation and rotation matrix
	Eigen::Matrix4d Mrt(4, 4); Mrt = Rx * Rz * T;	

	// Calculate screens's top left pixel and both vertical and horizontal steps
	// Top-left pixel on original projection screen (perpendicular to y-axis)
	Eigen::RowVector4d tlpx; tlpx << -((horizontalPixels_ / 2.0 - 0.5) * pxStep), 
									 screenCenter.y_, 
									 (verticalPixels_ / 2.0 - 0.5) * pxStep, 
									 1.0;

	// debug
	Vector3d tlpxDebug = Vector3d(tlpx(0), tlpx(1), tlpx(2));

	tlpx = tlpx * Mrt;	

	TLPx = Vector3d(tlpx(0), tlpx(1), tlpx(2));

	// x step
	Eigen::RowVector4d dx; dx << pxStep, 0.0, 0.0, 1.0;

	// debug
	Vector3d dxDebug = Vector3d(dx(0), dx(1), dx(2));

	dx = dx * Mr;
	xStep = Vector3d(dx(0), dx(1), dx(2));

	// z step
	Eigen::RowVector4d dz; dz << 0.0, 0.0, -pxStep, 1.0;

	// debug
	Vector3d dzDebug = Vector3d(dz(0), dz(1), dz(2));

	dz = dz * Mr;
	zStep = Vector3d(dz(0), dz(1), dz(2));

	// debug
	/*cout << "dirNew: " << dirNew << endl;
	cout << "TL Px orig: " << tlpxDebug << endl;
	cout << "TL Px new:  " << TLPx << endl;
	cout << "dx orig: " << dxDebug << endl;
	cout << "dx new:  " << xStep << endl;
	cout << "dz orig: " << dzDebug << endl;
	cout << "dz new:  " << zStep << endl;*/
	/*exit(1);*/
}

#endif