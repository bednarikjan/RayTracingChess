#ifndef _MODEL_H_
#define _MODEL_H_

#include <vector>
#include "Shape.h"
#include "Vector3d.h"

class Model 
{
public:	
	//! Constructor
	Model()
	{ 
		// DEBUG - generate a few spheres
		shapes.push_back(new Sphere(Vector3d(0.0, 0.0, -10003.0), 10000.0, Material(Vector3d(0.2, 0.2, 0.2), 0.9, 0.0, 0.0, 0.0)));  // ground
		shapes.push_back(new Sphere(Vector3d(5.0, 50.0, 3.0), 5.0, Material(Vector3d(0.8, 0.15, 0.15), 0.5, 0.0, 0.0, 10.0))); // red
		shapes.push_back(new Sphere(Vector3d(1.0, 40.0, 5.0), 3.0, Material(Vector3d(0.15, 0.8, 0.15), 0.8, 0.0, 1.5, 50.0))); // green
	}

	//! Destructor
	~Model() { 
		for(vector<Shape *>::iterator it = shapes.begin(); it != shapes.end(); ++it) 
		{
			delete *it;	
		}
	}

	vector<Shape *> shapes;
};

#endif