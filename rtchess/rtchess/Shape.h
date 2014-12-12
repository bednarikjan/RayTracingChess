#ifndef _SHAPE_H_
#define _SHAPE_H_

#include "Vector3d.h"
#include "Ray.h"

using namespace std;

#define MIN(a, b) (a) < (b) ? (a) : (b)
#define MAX(a, b) (a) > (b) ? (a) : (b)

//! Material of the shape. 
struct Material 
{
	Material() : color(Vector3d(0.0, 0.0, 0.0)), reflection(0.0), transparency(0.0), refractIdx(0.0), shininess(4.0) { }
	Material(Vector3d& color, double reflection, double transparency, double refractiveIndex, double shininess) : 
		color(color), reflection(reflection), transparency(transparency), refractIdx(refractiveIndex), shininess(shininess) { }
	Vector3d color;			// RGB, <0.0 - 1.0>
	double reflection;		// <0.0 - 1.0>
	double transparency;	// <0.0 - 1.0>
	double refractIdx;
	double shininess;
};

class Shape
{
public:
	Shape(Material* mat) : mat_(mat) { }
	~Shape() { }

	struct Intersection {
		Point isect;
		Vector3d normal;
		double t;		
		Shape* obj;
	};

	//! Calculates coordinates of intersection with given ray.
	virtual bool intersects(const Ray& ray, Intersection& intersection) = 0;

	//! Translates the shape
	virtual void translate(Vector3d& t) = 0;

	virtual Vector3d minCoords() = 0;
	virtual Vector3d maxCoords() = 0;

	Material* mat_;
};

class Sphere: public Shape
{
public:	
	Sphere(Point& center, double radius, Material* material) : 
		Shape(material), center_(center), radius_(radius) { }
	~Sphere() { }

	virtual bool intersects(const Ray& ray, Intersection& intersection);	
	virtual void translate(Vector3d& t);
	virtual Vector3d minCoords();
	virtual Vector3d maxCoords();

	Point center_;
	double radius_;
};

inline bool Sphere::intersects(const Ray& ray, Intersection& info)
{
	Point start = ray.getStart();
	Vector3d dir = ray.getDir();
	Vector3d s2c = start - center_;

	double A = dir.x_ * dir.x_ + dir.y_ * dir.y_ + dir.z_ * dir.z_;
	double B = 2 * (s2c.x_ * dir.x_ + s2c.y_ * dir.y_ + s2c.z_ * dir.z_);
	double C = (s2c.x_ * s2c.x_ + s2c.y_ * s2c.y_ + s2c.z_ * s2c.z_ - radius_ * radius_);

	// discriminant
	double D = B * B - 4 * A * C;

	if(D > 0.0) {
		double t1 = (-B + sqrt(D)) / (2.0 * A);
		double t2 = (-B - sqrt(D)) / (2.0 * A);		

		// the object is behind the ray
		if(t1 < 0.0 && t2 < 0.0) {	
			return false;
		// inside the speher - one of the intersection points is behind the ray
		} else if (t1 < 0.0 || t2 < 0.0) {
			info.t = MAX(t1, t2);
		// choose closer intersection
		} else {
			info.t = MIN(t1, t2);
		}
		
		info.isect = Vector3d(start.x_ + dir.x_ * info.t, start.y_ + dir.y_ * info.t, start.z_ + dir.z_ * info.t);
		info.normal = (info.isect - center_).normalize();
		info.obj = this;
		return true;
	}
	return false;	
}

Vector3d Sphere::minCoords()
{
	std::cerr << "Sphere::minCoords() - not yet implemented." << std::endl;
	exit(1);
}

Vector3d Sphere::maxCoords()
{
	std::cerr << "Sphere::maxCoords() - not yet implemented." << std::endl;
	exit(1);
}

void Sphere::translate(Vector3d& t)
{
	std::cerr << "Sphere::maxCoords() - not yet implemented." << std::endl;
	exit(1);
}

class Triangle: public Shape
{
public:
	Triangle(Vector3d v0, Vector3d v1, Vector3d v2, 
			 Vector3d n0, Vector3d n1, Vector3d n2,
			 Material* material) : Shape(material), 
			 v0(v0), v1(v1), v2(v2), n0(n0), n1(n1), n2(n2) { }
	~Triangle() { }

	virtual bool intersects(const Ray& ray, Intersection& info);	
	virtual void translate(Vector3d& t);
	virtual Vector3d minCoords();
	virtual Vector3d maxCoords();

	Vector3d v0, v1, v2;	
	Vector3d n0, n1, n2;	

	static const double EPSILON;
};

const double Triangle::EPSILON = 1e-6;

inline bool Triangle::intersects(const Ray& ray, Intersection& info) {
	// Find vectors for 2 edges sharing vertex v0
	Vector3d edge1 = v1 - v0;
	Vector3d edge2 = v2 - v0;

	// calculate determinant
	Vector3d pVec = ray.getDir().cross(edge2);
	double det = edge1.dot(pVec); 	

	// determinant close to 0 => ray parallel to triangle plane
	if(det < -Triangle::EPSILON || det > Triangle::EPSILON) {		
		double invDet = 1.0 / det;
		Vector3d tVec = ray.getStart() - v0;

		// u - first barycentric coordinate
		double u = tVec.dot(pVec) * invDet;
		if(u < 0.0 || u > 1.0)
			return false;

		Vector3d qVec = tVec.cross(edge1);

		// v - second barycentric coordinate
		double v = ray.getDir().dot(qVec) * invDet;
		if(v < 0.0 || (u + v) > 1.0)
			return false;		

		// the triangle is on the oposite side of ray
		double t = edge2.dot(qVec) * invDet;
		if(t < 0.0)
			return false;

		info.t = t;					
		info.isect = ray.getStart() + info.t * ray.getDir();
		info.normal = ((1.0 - u - v) * n0 + u * n1 + v * n2).normalize();		
		info.obj = this;
		return true;
	}
	return false;
}

void Triangle::translate(Vector3d& t)
{	
	v0 += t; 
	v1 += t;
	v2 += t;
}

Vector3d Triangle::minCoords()
{
	return Vector3d((v0.x_ < v1.x_) ? ((v0.x_ < v2.x_) ? v0.x_ : v2.x_) : ((v1.x_ < v2.x_) ? v1.x_ : v2.x_),
					(v0.y_ < v1.y_) ? ((v0.y_ < v2.y_) ? v0.y_ : v2.y_) : ((v1.y_ < v2.y_) ? v1.y_ : v2.y_),
					(v0.z_ < v1.z_) ? ((v0.z_ < v2.z_) ? v0.z_ : v2.z_) : ((v1.z_ < v2.z_) ? v1.z_ : v2.z_));
}

Vector3d Triangle::maxCoords()
{
	return Vector3d((v0.x_ > v1.x_) ? ((v0.x_ > v2.x_) ? v0.x_ : v2.x_) : ((v1.x_ > v2.x_) ? v1.x_ : v2.x_),
					(v0.y_ > v1.y_) ? ((v0.y_ > v2.y_) ? v0.y_ : v2.y_) : ((v1.y_ > v2.y_) ? v1.y_ : v2.y_),
					(v0.z_ > v1.z_) ? ((v0.z_ > v2.z_) ? v0.z_ : v2.z_) : ((v1.z_ > v2.z_) ? v1.z_ : v2.z_));
}


#endif