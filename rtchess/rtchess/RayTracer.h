#ifndef _RAYTRACER_H_
#define _RAYTRACER_H_

#include "Scene.h"

class RayTracer 
{
public:
	RayTracer(Scene& scene): scene_(scene) { }
	~RayTracer() { }

	void trace();

private:
	Scene scene_;
};

#endif