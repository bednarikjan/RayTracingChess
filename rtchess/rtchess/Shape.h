#ifndef _SHAPE_H_
#define _SHAPE_H_

#include "Vector3d.h"
#include "Ray.h"

using namespace std;

#define MIN(a, b) (a) < (b) ? (a) : (b)
#define MAX(a, b) (a) > (b) ? (a) : (b)

class Shape
{

public:
	Shape() { }
	~Shape() { }

	//! Calculates coordinates of intersection with given ray.
	virtual bool intersects(const Ray& ray, Point& intersection, Vector3d& normal) = 0;	
};

class Sphere: public Shape
{
public:
	Sphere(Point& center, double radius): center_(center), radius_(radius) { }
	~Sphere() { }

	virtual bool intersects(const Ray& ray, Point& intersection, Vector3d& normal);	

	Point center_;
	double radius_;
};

inline bool Sphere::intersects(const Ray& ray, Point& intersection, Vector3d& normal)
{
	Point start = ray.getStart();
	Vector3d dir = ray.getDirection();
	Vector3d s2c = start - center_;

	double A = dir.x_ * dir.x_ + dir.y_ * dir.y_ + dir.z_ * dir.z_;
	double B = 2 * (s2c.x_ * dir.x_ + s2c.y_ * dir.y_ + s2c.z_ * dir.z_);
	double C = (s2c.x_ * s2c.x_ + s2c.y_ * s2c.y_ + s2c.z_ * s2c.z_ - radius_ * radius_);

	// discriminant
	double D = B * B - 4 * A * C;

	if(D > 0.0) {
		double t1 = (-B + sqrt(D)) / (2.0 * A);
		double t2 = (-B - sqrt(D)) / (2.0 * A);
		double t;

		// the object is behind the ray
		if(t1 < 0.0 && t2 < 0.0) {	
			return false;
		// inside the speher - one of the intersection points is behind the ray
		} else if (t1 < 0.0 || t2 < 0.0) {
			t = MAX(t1, t2);
		// choose closer intersection
		} else {
			t = MIN(t1, t2);
		}
		
		intersection = Vector3d(start.x_ + dir.x_ * t, start.y_ + dir.y_ * t, start.z_ + dir.z_ * t);
		normal = (intersection - center_).normalize();	 
		return true;
	}
	return false;	
}


#endif