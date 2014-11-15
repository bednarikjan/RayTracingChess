#include "Vector.h"

using namespace std;

template<class T>
T Vector<T>::length()
{
	;
}

template<class T>
void Vector<T>::normalize()
{
	
}

template<class T>
T Vector<T>::dot(Vector other)
{
	return x_ * other.x_ + y_ * other.y_ + z_ * other.z_;
}