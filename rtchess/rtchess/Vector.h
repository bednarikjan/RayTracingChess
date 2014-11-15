//! Class implementing 3D vector.
/*!
Standard operators +, - are overloaded to implement basic
operations like vector addition, subtraction
*/

#include <string>

using namespace std;

template <class T>
class Vector
{
public:
	Vector(void) { }
	Vector(T x, T y, T z): x_(x), y_(y), z_(z) { }			
	~Vector(void) { }

	T length();
	void normalize();
	T dot(Vector other);

	Vector operator+(Vector other) {
		return Vector<T>(x_ + other.x_, y_ + other.y_, z_ + other.z_);
	}
	
	Vector<T> operator-() {
		return Vector<T>(-x_, -y_, -z_);
	}
	
	Vector<T> operator-(Vector<T> other) {
		return this + -other;	
	}

	friend ostream& operator<<(ostream& os, const Vector<T>& v) {		
		os << "(" << v.x_ << ", "
				  << v.y_ << ", "
				  << v.z_ << ")";
		return os;
	}	

	T x_;
	T y_;
	T z_;	
};

