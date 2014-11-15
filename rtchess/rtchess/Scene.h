#ifndef _SCENE_H_
#define _SCENE_H_

#include <string>
#include "Camera.h"
#include "Vector.h"

using namespace std;

class Scene
{
public:
	//! Constructor
	Scene() { }
	
	//! Destructor
	~Scene(void) { }

	//! Loads model from file
	void loadModel(string fileName);

	//! Sets cameraposition and projection screen size
	void setCameraLocation(Vector position, Vector direction);
	void setCameraResolution(unsigned screenWidth, unsigned screenHeight);
	void setCameraFieldOfView(double horizontalAngle);

private:
	Camera camera;
};

inline void Scene::loadModel(string fileName) 
{

}

inline void Scene::setCameraLocation(Vector position, Vector direction)
{
	camera.position = position;
	//camera.direction = direction.normalize();	
}

inline void Scene::setCameraResolution(unsigned screenWidth, unsigned screenHeight)
{
	camera.screenWidth = screenWidth;
	camera.screenHeight = screenHeight;
}

inline void Scene::setCameraFieldOfView(double horizontalAngle)
{
	camera.fieldOfView = horizontalAngle;
}

#endif