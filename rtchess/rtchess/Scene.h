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

Material DEFAULT_LIGHT_MATERIAL(Vector3d(1.0, 1.0, 1.0), 0.0, 0.0, 0.0, 0.0);
const Camera DEFALUT_CAMERA(Vector3d(0.0, 0.0, 0.0), Vector3d(0.0, 1.0, 0.0), 160, 120, 45);
const Light DEFAULT_LIGHT(Vector3d(4.0, 1.0, 6.0), 0.0, &DEFAULT_LIGHT_MATERIAL);		

class Scene
{
public:
	//! Constructor
	Scene(string& modelFile) 
	{ 				
		model_ = new ModelGeneral(modelFile);
		init(DEFALUT_CAMERA, DEFAULT_LIGHT);
	}

	Scene(Camera& camera, Light& light, string& fileName)
	{ 		
		model_ = new ModelGeneral(fileName);
		init(camera, light);		
	}		

	Scene(Model *model) 
	{ 
		assert(model != NULL);
		model_ = model;
		init(DEFALUT_CAMERA, DEFAULT_LIGHT);
	}

	Scene(Camera& camera, Light& light, Model *model) 
	{ 
		assert(model != NULL);
		model_ = model;
		init(camera, light);	
	}
	
	//! Destructor
	~Scene(void) 
	{
		delete model_;
		delete rayTracer;
		delete[] image;
	}	

	void setCameraLocation(Vector3d position, Vector3d direction) 
	{
		rayTracer->camera_->setPosition(position);
		rayTracer->camera_->setDirection(direction);
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

	void setRecursionDepth(int depth) { rayTracer->setDepth(depth); }
	void setBackgroundColor(Vector3d color) { rayTracer->setBackgroundColor(color); }

	//! Main rendering function
	void render();

	//! Save rendered image to .PNG file
	void saveImage(string& fileName);

private:	
	RayTracer* rayTracer;	//!< ray tracer
	Model* model_;			//!< loaded model (triangle model or spheres)
	Vector3d *image;		//!< output image (matrix of RGB vectors)

	//! Initalizes teh object
	void init(Camera camera, Light light)
	{				
		rayTracer = new RayTracer(camera, light, model_);
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