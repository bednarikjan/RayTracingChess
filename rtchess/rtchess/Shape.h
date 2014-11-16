#ifndef _SHAPE_H_
#define _SHAPE_H_

#include "Vector3d.h"

class Shape
{

public:
	Shape() { }
	~Shape() { }

	//! Calculates coordinates of intersection with given ray.
	virtual bool intersects(Vector3d& ray, Point& intersection) = 0;
};

class Sphere: public Shape
{
public:
	Sphere(Point& center, double radius): center_(center), radius_(radius) { }
	~Sphere() { }

	virtual bool intersects(Vector3d& ray, Point& intersection);

	Point center_;
	double radius_;
};

inline bool Sphere::intersects(Vector3d& ray, Point& intersection)
{
	// TODO
	return false;	
}

#endif