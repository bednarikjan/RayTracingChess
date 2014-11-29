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
		shapes.push_back(new Sphere(Vector3d(0.0, 3.0, 0.5), 1.5, Material(Vector3d(0.8, 0.15, 0.15), 0.7, 0.0, 0.0, 4.0)));
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