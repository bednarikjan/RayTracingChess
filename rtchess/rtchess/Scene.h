#ifndef _SCENE_H_
#define _SCENE_H_

#include <string>
#include "Camera.h"
#include "Vector3d.h"
#include "RayTracer.h"
#include "Model.h"
#include "Light.h"

using namespace std;

class Scene
{
public:
	//! Constructor
	Scene(string& fileName) 
	{ 
		Camera camera(Vector3d(0.0, 0.0, 0.0), Vector3d(0.0, 1.0, 0.0), 640, 480, 100);
		Light light(Vector3d(10.0, 10.0, 10.0), 2.0);
		loadModel(fileName);
		rayTracer = new RayTracer(camera, light, model);
	}

	Scene(Camera& camera, Light& light, string& fileName)
	{ 
		loadModel(fileName);
		rayTracer = new RayTracer(camera, light, model);
	}		
	
	//! Destructor
	~Scene(void) 
	{
		delete model;
		delete rayTracer;		
	}	

	void setCameraLocation(Vector3d position, Vector3d direction) 
	{
		rayTracer->camera_->position_ = position;
		rayTracer->camera_->direction_ = direction;			
	}

	void setCameraResolution(unsigned screenWidth, unsigned screenHeight)
	{
		rayTracer->camera_->setResolution(screenWidth, screenHeight);		
	}

	void setCameraFieldOfView(double horizontalAngle)
	{
		rayTracer->camera_->setFieldOfView(horizontalAngle);	
	}
	
	void setLightPosition(Vector3d position)
	{
		rayTracer->light_->center_ = position;
	}

	void setLightRadius(double radius)
	{
		rayTracer->light_->radius_ = radius;
	}

	//! Main rendering function
	void render();

	//! Save rendered image to .PNG file
	void saveImage(string& fileName);

private:	
	RayTracer* rayTracer;
	Model* model;

	//! Loads model from file
	void loadModel(string fileName);
};

/*!
Loads model from file
*/
inline void Scene::loadModel(string fileName) 
{
	// DEBUG
	cout << "Loading model " << fileName << endl;
	
	model = new Model();
}

inline void Scene::render()
{
	// DEBUG
	cout << "Rendering scene... " << endl;
}

inline void Scene::saveImage(string &fileName)
{
	// DEBUG
	cout << "Saving rendered image to file " << fileName << endl;
}

#endif