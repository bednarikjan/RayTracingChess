#ifndef _RAYTRACER_H_
#define _RAYTRACER_H_

#include "Camera.h"
#include "Light.h"
#include "Model.h"

class RayTracer 
{
public:
	RayTracer(Camera& camera, Light &light, Model* model): model_(model)
	{ 
		camera_ = new Camera(camera);
		light_ = new Light(light);
	}

	~RayTracer()
	{
		delete camera_;
		delete light_;
	}

	void setModel();
	void trace();
	
	Camera* camera_;
	Light* light_;
	Model* model_;
};

inline void RayTracer::setModel()
{

}

inline void RayTracer::trace()
{

}

#endif