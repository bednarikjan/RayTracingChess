//! Class implementing 3D vector.
/*!
Standard operators +, - are overloaded to implement basic
operations like vector addition, subtraction
*/

#include <string>
#include <cmath>

using namespace std;

class Vector
{
public:
	Vector(void) { }
	Vector(double x, double y, double z): x_(x), y_(y), z_(z) { }			
	~Vector(void) { }

	double length();
	void normalize();
	double dot(Vector& other);

	Vector operator+(Vector& other) {
		return Vector(x_ + other.x_, y_ + other.y_, z_ + other.z_);
	}
	
	Vector operator-() {
		return Vector(-x_, -y_, -z_);
	}
	
	Vector operator-(Vector& other) {
		return *this + -other;	
	}

	friend ostream& operator<<(ostream& os, const Vector& v) {		
		os << "(" << v.x_ << ", "
				  << v.y_ << ", "
				  << v.z_ << ")";
		return os;
	}	

	double x_;
	double y_;
	double z_;	
};

inline double Vector::length()
{
	return sqrt(x_*x_ + y_*y_ + z_*z_);
}

inline void Vector::normalize()
{
	double len = length();
	x_ /= len;
	y_ /= len;
	z_ /= len;
}

inline double Vector::dot(Vector& other)
{
	return x_ * other.x_ + y_ * other.y_ + z_ * other.z_;
}
