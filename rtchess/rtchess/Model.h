#ifndef _MODEL_H_
#define _MODEL_H_

#include <vector>
#include <cctype>
#include "Shape.h"
#include "Vector3d.h"

using namespace std;

class Model 
{
public:	
	//! Constructor
	Model()
	{ 
		// debug
		cout << "Loading model..." << endl;

		// DEBUG - generate a few spheres
		shapes.push_back(new Sphere(Vector3d(0.0, 0.0, -10003.0), 10000.0, Material(Vector3d(0.2, 0.2, 0.2), 0.9, 0.0, 0.0, 0.0)));  // ground
		shapes.push_back(new Sphere(Vector3d(5.0, 50.0, 3.0), 5.0, Material(Vector3d(0.8, 0.15, 0.15), 0.1, 0.0, 0.0, 10.0))); // red
		shapes.push_back(new Sphere(Vector3d(1.0, 40.0, 5.0), 3.0, Material(Vector3d(0.15, 0.8, 0.15), 0.8, 0.0, 1.5, 50.0))); // green

		// DEBUG - generate a few triangles
		/*shapes.push_back(new Triangle(Vector3d(-20.0, 100.0, -20.0), Vector3d(20.0, 100.0, -20.0), Vector3d(-5.0, 100.0, 20.0),
									  Vector3d(0.0, -1.0, 0.0).normalize(), Vector3d(0.0, -1.0, 0.0).normalize(), Vector3d(0.0, -1.0, 0.0).normalize(), 
									  Material(Vector3d(0.15, 0.15, 0.85), 0.0, 0.0, 0.0, 10.0)));		*/
	}	

	Model(string& fileName) { 
		// debug
		cout << "Loading model from " << fileName << "..." << endl;
		
		load(fileName); 
	}

	//! Destructor
	~Model() { 
		for(vector<Shape *>::iterator it = shapes.begin(); it != shapes.end(); ++it) 
		{
			delete *it;	
		}
	}

	/*!
	Loads model from .OBJ file. Expects parameters v, vn, f
	*/
	void load(string fileName);

	vector<Shape *> shapes;	
};

inline void Model::load(string fileName) {
	vector<Vector3d> vertices;
	vector<Vector3d> normals;

	// TODO -> do somehow else! Material should not be stored for each 3angle
	Material m(Vector3d(0.15, 0.15, 0.85), 0.3, 0.0, 0.0, 15.0);

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
			shapes.push_back(new Triangle(vertices.at(iv1 - 1), vertices.at(iv2 - 1), vertices.at(iv3 - 1),
										  normals.at(in1 - 1),  normals.at(in2 - 1),  normals.at(in3 - 1), m));
		}
	}

	/*cout << "vertices:" << endl;
	for(unsigned i = 0; i < vertices.size(); ++i) {
		cout << vertices.at(i) << endl;
	}*/

	// debug
	/*Vector3d shift(-2.0, 5.0, -1.5);*/
	Vector3d shift(0.0, 20.0, 20.0);
	/*for(unsigned i = 0; i < shapes.size(); ++i) {
		static_cast<Triangle*>(shapes.at(i))->v0 = static_cast<Triangle*>(shapes.at(i))->v0 + shift;
		static_cast<Triangle*>(shapes.at(i))->v1 = static_cast<Triangle*>(shapes.at(i))->v1 + shift;
		static_cast<Triangle*>(shapes.at(i))->v2 = static_cast<Triangle*>(shapes.at(i))->v2 + shift;*/

		/*cout << "triangle " << i << ":" << endl;
		cout << static_cast<Triangle*>(shapes.at(i))->v0 << 
			static_cast<Triangle*>(shapes.at(i))->v1 <<
			static_cast<Triangle*>(shapes.at(i))->v2 << endl;
		cout << static_cast<Triangle*>(shapes.at(i))->n0 << 
			static_cast<Triangle*>(shapes.at(i))->n1 <<
			static_cast<Triangle*>(shapes.at(i))->n2 << endl;*/
	//}	
}

#endif