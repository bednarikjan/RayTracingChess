#ifndef _MODEL_H_
#define _MODEL_H_

#include <vector>
#include <cctype>
#include "Shape.h"
#include "Vector3d.h"

using namespace std;

struct Object
{	
	vector<Shape *> shapes;	
	
	// TODO - add bounding box
	// Sphere boundingBox;
};

class Model 
{
public:	
	//! Constructor
	Model() : visible(true) { }	

	//! Destructor
	~Model() { }

	/*!
		Loads model from .OBJ file. Expects parameters v, vn, f
	*/
	virtual void load(string fileName) = 0;	
	  
	vector<Object> objects_;	
	bool visible;
};

class ModelGeneral : public Model
{
public:	
	ModelGeneral()
	{
		// debug
		cout << "Loading model..." << endl;

		// DEBUG - generate a few spheres
		objects_.push_back(Object());

		objects_.at(0).shapes.push_back(new Sphere(Vector3d(0.0, 0.0, -10003.0), 10000.0, new Material(Vector3d(0.2, 0.2, 0.2), 0.9, 0.0, 0.0, 0.0)));  // ground		
		objects_.at(0).shapes.push_back(new Sphere(Vector3d(5.0, 50.0, 3.0), 5.0, new Material(Vector3d(0.8, 0.15, 0.15), 0.1, 0.0, 0.0, 10.0))); // red
		objects_.at(0).shapes.push_back(new Sphere(Vector3d(1.0, 40.0, 5.0), 3.0, new Material(Vector3d(0.15, 0.8, 0.15), 0.8, 0.0, 1.5, 50.0))); // green		
	}

	ModelGeneral(string& fileName)
	{
		// debug
		cout << "Loading model from " << fileName << "..." << endl;		

		m = new Material(Vector3d(0.15, 0.15, 0.85), 0.3, 0.0, 0.0, 15.0);

		load(fileName); 
	}

	~ModelGeneral() {
		for(int i = 0; i < (int)objects_.at(0).shapes.size(); i++) {
			delete objects_.at(0).shapes.at(i);
		}
	}

	virtual void load(string fileName);	

	// TODO -> move somwhere else
	Material* m;
};

inline void ModelGeneral::load(string fileName) {
	vector<Vector3d> vertices;
	vector<Vector3d> normals;

	// Expecting only 1 object
	objects_.push_back(Object());	

	ifstream file(fileName);	
	if(file.fail()) {
		cerr << "ERROR: The file " << fileName << " cannot be opened." << endl;
		exit(1);
	}	
	string line;
	while(getline(file, line)) {		
		//cout << line << endl;
		
		// vertex 'v num1 num2 num3' 
		if(line[0] == 'v' && isspace(line[1])) {
			double x, y, z;					
			sscanf(line.c_str(), "%*s %lf %lf %lf", &x, &y, &z);
			vertices.push_back(Vector3d(x, y, z));

		// normal 'vn num1 num2 num3'
		} else if(line[0] == 'v' && line[1] == 'n' && isspace(line[2])) {
			normals.push_back(Vector3d());
			sscanf(line.c_str(), "%*s %lf %lf %lf", &normals.back().x_, &normals.back().y_, &normals.back().z_);

		// face 'f v1//vn1 v2//vn2 v3//vn3'
		} else if(line[0] == 'f' && isspace(line[1])) {
			unsigned iv1, iv2, iv3;
			unsigned in1, in2, in3;
			sscanf(line.c_str(), "%*s %u//%u %u//%u %u//%u", &iv1, &in1, &iv2, &in2, &iv3, &in3);
			objects_.at(0).shapes.push_back(new Triangle(vertices.at(iv1 - 1), vertices.at(iv2 - 1), vertices.at(iv3 - 1),
										  normals.at(in1 - 1),  normals.at(in2 - 1),  normals.at(in3 - 1), m));
		}
	}	
}

#endif