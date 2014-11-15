#ifndef _SCENE_H_
#define _SCENE_H_

#include <string>
#include "Camera.h"
#include "Vector.h"
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
		Camera camera(Vector(0.0, 0.0, 10.0), Vector(1.0, 1.0, -1.0), 640, 480, 100);
		Light light(Vector(10.0, 10.0, 10.0), 2.0);
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
		//delete model;
		delete rayTracer;		
	}	

	void setCameraLocation(Vector position, Vector direction) 
	{
		rayTracer->camera_->position_ = position;
		rayTracer->camera_->direction_ = direction;			
	}

	void setCameraResolution(unsigned screenWidth, unsigned screenHeight)
	{
		rayTracer->camera_->screenWidth_ = screenWidth;
		rayTracer->camera_->screenHeight_ = screenHeight;
	}

	void setCameraFieldOfView(double horizontalAngle)
	{
		rayTracer->camera_->fieldOfView_ = horizontalAngle;	
	}
	
	void setLightPosition(Vector position)
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

//private:	
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
	// TODO - check if model is properly loaded - not NULL!
	model = NULL;
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