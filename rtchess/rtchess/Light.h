#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "Vector.h"
#include "Shape.h"

class Light: public Sphere
{
public:	
	Light(Vector& position, double radius): Sphere(position, radius) { }
	~Light() {  }
};

#endif