#include <string>
#include "Camera.h"
#include "Vector.h"

using namespace std;

class Scene
{
public:
	//! Constructor
	Scene(void) { }
	
	//! Destructor
	~Scene(void) { }

	//! Loads model from file
	void loadModel(string fileName);

	//! Sets cameraposition and projection screen size
	void setCamera(Vector position, Vector direction, unsigned screenWidth, unsigned screenHeight);

private:
	Camera camera;

};

inline void Scene::loadModel(string fileName) 
{

}

inline void Scene::setCamera(Vector position, Vector direction, unsigned screenWidth, unsigned screenHeight)
{

}