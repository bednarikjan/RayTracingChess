#ifndef _SCENE_H_
#define _SCENE_H_

#include <string>
#include <fstream>

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
		Camera camera(Vector3d(0.0, 0.0, 0.0), Vector3d(0.0, 1.0, 0.0), 1920, 1080, 50);
		Light light(Vector3d(5.0, -10.0, 5.0), 0.0, Material(Vector3d(1.0, 1.0, 1.0), 0.0, 0.0, 0.0, 0.0));		
		init(fileName, camera, light);
	}

	Scene(Camera& camera, Light& light, string& fileName)
	{ 
		init(fileName, camera, light);		
	}		
	
	//! Destructor
	~Scene(void) 
	{
		delete model;
		delete rayTracer;
		delete[] image;
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
	RayTracer* rayTracer;	//!< ray tracer
	Model* model;			//!< loaded model (triangle model or spheres)
	Vector3d *image;		//!< output image (matrix of RGB vectors)

	//! Initalizes teh object
	void init(string& fileName, Camera& camera, Light& light)
	{		
		//model = new Model();
		model = new Model(fileName);
		rayTracer = new RayTracer(camera, light, model);
		image = new Vector3d[camera.getScreenHeight() * camera.getScreenWidth()];		
	}	
};

inline void Scene::render()
{
	// DEBUG
	cout << "Rendering scene... " << endl;

	rayTracer->render(image);
}

inline void Scene::saveImage(string &fileName)
{
	// DEBUG
	cout << "Saving rendered image to file " << fileName << endl;

	// Save result to a PPM image
	ofstream ofs(fileName, std::ios::out | std::ios::binary);
	ofs << "P6\n" << rayTracer->camera_->getScreenWidth() << " " << rayTracer->camera_->getScreenHeight() << "\n255\n";
	for (unsigned i = 0; i < rayTracer->camera_->getScreenWidth() * rayTracer->camera_->getScreenHeight(); i++) {
		ofs <<	(unsigned char)(min(image[i].x_, 1.0) * 255) << 
				(unsigned char)(min(image[i].y_, 1.0) * 255) << 
				(unsigned char)(min(image[i].z_, 1.0) * 255);
	}
	ofs.close();	
}

#endif