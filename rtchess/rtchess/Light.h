#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "Vector3d.h"
#include "Shape.h"

Material DEFAULT_MATERIAL(Vector3d(1.0, 1.0, 1.0), 0.0, 0.0, 0.0, 0.0);
Sphere DEFAULT_SPHERE(Vector3d(0.0, 0.0, 0.0), 1.0, &DEFAULT_MATERIAL);

class Light: public Sphere
{
public:	
	Light() : Sphere(DEFAULT_SPHERE) { }
	
	Light(Vector3d& position, double radius, Material* material): 
		Sphere(position, radius, material) { }
	~Light() {  }
};

#endif