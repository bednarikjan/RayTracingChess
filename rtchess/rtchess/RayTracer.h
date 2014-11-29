#ifndef _RAYTRACER_H_
#define _RAYTRACER_H_

#include "Camera.h"
#include "Light.h"
#include "Model.h"

class RayTracer 
{
public:
	RayTracer(Camera& camera, Light &light, Model* model, unsigned maxDepth = 5): model_(model), maxDepth_(maxDepth)
	{ 
		camera_ = new Camera(camera);
		light_ = new Light(light);
	}

	~RayTracer()
	{
		delete camera_;
		delete light_;
	}

	void setModel(Model *model) { model_ = model; }
	void render(Vector3d* image);	
	
	Camera* camera_;
	Light* light_;
	Model* model_;
	unsigned maxDepth_;

private:
	Vector3d trace(Ray& ray, unsigned depth);

	static const double INFINITY;
};

const double RayTracer::INFINITY = std::numeric_limits<double>::max();

inline void RayTracer::render(Vector3d* image)
{
	// topleft screen pixel position
	double dx = camera_->getPxStep();
	Point px = Point(-((camera_->getScreenWidth() / 2.0 - 0.5) * dx), 
						camera_->screenCenter.y_, 
					   (camera_->getScreenHeight() / 2.0 - 0.5) * dx);
	
	// trace ray through each pixel
	for(int i = 0; i < (int)camera_->getScreenHeight(); i++) 
	{
		for(int j = 0; j < (int)camera_->getScreenWidth(); j++) 
		{
			Ray ray(camera_->position_, (px + Point(j * dx, 0.0, -i * dx)) - camera_->position_);
			image[i * camera_->getScreenWidth() + j] = trace(ray, maxDepth_);
		}
	}
}

inline Vector3d RayTracer::trace(Ray& ray, unsigned depth)
{		
	double tMin = INFINITY, t;	
	Point isect, isectClosest;
	Shape* obj = NULL;
	Vector3d normal;

	// resulting pixel color
	Vector3d color;

	// find closest intersection	
	for(int i = 0; i < (int)model_->shapes.size(); i++) {
		if(model_->shapes.at(i)->intersects(ray, isect, normal, t) && (t < tMin)) {		
			tMin = t;
			isectClosest = isect;
			obj = model_->shapes.at(i);
		}
	}

	// some intersection found
	if(tMin < INFINITY) {		
		Vector3d cop(0.0, 0.0, 0.0);	// color of object at the given pixel.
		Vector3d cr(0.0, 0.0, 0.0);		// color of reflected ray
		Vector3d ct(0.0, 0.0, 0.0);		// color of refracted ray

		// hack
		isectClosest = isectClosest + (normal * 0.0001);

		// cast shadow rays
		bool illuminated = true;
		Vector3d lv((light_->center_ - isectClosest).normalize());	// vector aiming to light
		for(int i = 0; i < (int)model_->shapes.size(); i++) {
			if(/*model_->shapes.at(i) != obj && */model_->shapes.at(i)->intersects(Ray(isectClosest, lv), isect, normal, t)) {
				illuminated = false;
				break;
			}
		}

		// evaluate Phong reflection and shading model
		double Ia = 0.0, Id = 0.0, Is = 0.0;
		
		// ambient
		Ia = 0.4;

		if(illuminated) {						
			// diffuse		
			Id = lv.dot(normal) * 1.5;			

			// specular
			Is = pow((-lv + (normal * (2 * (-lv).dot(normal)))).dot((camera_->position_ - isectClosest).normalize()), obj->mat_.sh) * 0.7;
		}

		cop = light_->mat_.c * obj->mat_.c * (Ia + Id + Is);

		// reflective object
		if(obj->mat_.r > 0.0 && depth > 0) {
			//cr = trace( , depth - 1);
		}

		// transparent object
		if(obj->mat_.t > 0.0 && depth > 0) {
			//ct = trace( , depth - 1);
		}

		color = obj->mat_.t * ct + (1.0 - obj->mat_.t) * (obj->mat_.r * cr + (1.0 - obj->mat_.r) * cop);
	} 
	// no intersection
	else {
		color = Vector3d(0.5, 0.5, 0.5);	// background color 
	}

	return color;
}

#endif