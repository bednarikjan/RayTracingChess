//! Class implementing 3D Vector3d.
/*!
Standard operators +, - are overloaded to implement basic
operations like Vector3d addition, subtraction
*/

#ifndef _Vector3d_H_
#define _Vector3d_H_

#include <string>
#include <cmath>

using namespace std;

class Vector3d;
typedef Vector3d Point;

class Vector3d
{
public:	
	Vector3d(void) { }
	Vector3d(double x, double y, double z): x_(x), y_(y), z_(z) { }			
	Vector3d(double x): x_(x), y_(x), z_(x) { }	
	~Vector3d(void) { }

	double length();		

	Vector3d& normalize();
	double dot(Vector3d& other);
	Vector3d cross(Vector3d& other);

	Vector3d operator+(Vector3d& other) {
		return Vector3d(x_ + other.x_, y_ + other.y_, z_ + other.z_);
	}
	
	Vector3d operator-() {
		return Vector3d(-x_, -y_, -z_);
	}
	
	Vector3d operator-(Vector3d& other) {
		return *this + -other;	
	}

	//! element-wise multiplication
	Vector3d operator*(Vector3d& other) {
		return Vector3d(x_ * other.x_, y_ * other.y_, z_ * other.z_);
	}

	friend Vector3d operator*(Vector3d& ours, double other) {
		return Vector3d(ours.x_ * other, ours.y_ * other, ours.z_ * other);
	}

	friend Vector3d operator*(double other, Vector3d& ours) {
		return Vector3d(ours.x_ * other, ours.y_ * other, ours.z_ * other);
	}

	friend ostream& operator<<(ostream& os, const Vector3d& v) {		
		os << "(" << v.x_ << ", "
				  << v.y_ << ", "
				  << v.z_ << ")";
		return os;
	}	

	bool operator==(Vector3d& other) {
		return(almostEqual(x_, other.x_) && 
			   almostEqual(y_, other.y_) &&
			   almostEqual(z_, other.z_));
	}

	double x_;
	double y_;
	double z_;	

private:
	bool almostEqual(double a, double b, double EPSILON = 1e-6) {
		return fabs(a - b) < EPSILON;
	}
};

inline double Vector3d::length() {
	return sqrt(x_*x_ + y_*y_ + z_*z_);
}

inline Vector3d& Vector3d::normalize() {
	double len = length();
	x_ /= len;
	y_ /= len;
	z_ /= len;

	return *this;
}

inline double Vector3d::dot(Vector3d& other) {
	return x_ * other.x_ + y_ * other.y_ + z_ * other.z_;
}

inline Vector3d Vector3d::cross(Vector3d& other) {
	return Vector3d(y_ * other.z_ - z_ * other.y_, 
					z_ * other.x_ - x_ * other.z_, 
					x_ * other.y_ - y_ * other.x_);
}

#endif