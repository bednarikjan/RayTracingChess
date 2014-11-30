#ifndef _RAYTRACER_H_
#define _RAYTRACER_H_

#include "Camera.h"
#include "Light.h"
#include "Model.h"

class RayTracer 
{
public:
	RayTracer(Camera& camera, Light &light, Model* model, unsigned maxDepth = 3): model_(model), maxDepth_(maxDepth)
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
	Vector3d trace(Ray& ray, unsigned depth, bool inside);

	static const double INFINITY;
};

const double RayTracer::INFINITY = std::numeric_limits<double>::max();

inline void RayTracer::render(Vector3d* image)
{
	int w = camera_->getScreenWidth();
	int h = camera_->getScreenHeight();
	double pxw = camera_->getPxStep();

	// TopLeft screen pixel position	
	Point pxTL = Point(-((w / 2.0 - 0.5) * pxw), camera_->screenCenter.y_, (h / 2.0 - 0.5) * pxw);
	
	// trace ray through each pixel
	for(int i = 0; i < h; i++) {
		for(int j = 0; j < w; j++) {
			Ray ray(camera_->position_, (pxTL + Point(j * pxw, 0.0, -i * pxw)) - camera_->position_);
			image[i * w + j] = trace(ray, maxDepth_, false);
		}
	}
}

inline Vector3d RayTracer::trace(Ray& ray, unsigned depth, bool inside)
{		
	
	Shape::Intersection is, isC;	// intersection info
	isC.t = INFINITY;						
	Vector3d color;					// resulting pixel color

	// find closest intersection	
	for(int i = 0; i < (int)model_->shapes.size(); i++) {
		if(model_->shapes.at(i)->intersects(ray, is) && (is.t < isC.t))
			isC = is;					
	}

	// some intersection found
	if(isC.t < INFINITY) {						
		Vector3d cop(0.0, 0.0, 0.0);	// color of object at the given pixel.
		Vector3d cr(0.0, 0.0, 0.0);		// color of reflected ray
		Vector3d ct(0.0, 0.0, 0.0);		// color of refracted ray

		// hack - move interscetion point along a normal vector a bit (double imprecision workaround)
		Point isectOut(isC.isect + (isC.normal * 0.00001));
		Point isectIn(isC.isect - (isC.normal * 0.00001));		
		
		// cast shadow rays
		Vector3d lv((light_->center_ - isectOut).normalize());	// vector aiming to light
		bool illuminated = true;
		if(inside || lv.dot(isC.normal) < 0.0) {		// inside object or face turned away from light
			illuminated = false;
		} else {
			for(int i = 0; i < (int)model_->shapes.size(); i++) {
				if(model_->shapes.at(i)->intersects(Ray(isectOut, lv), is)) {
					illuminated = false;
					break;
				}
			}
		}

		// evaluate Phong reflection and shading model
		Vector3d R, V;
		// TODO - should not be here! move somewhere else - class member perhaps, material?
		double Ia = 0.0, Id = 0.0, Is = 0.0;
		double ka = 0.2, kd = 3.5, ks = 5.0;
		//double ka = 0.0, kd = 3.5, ks = 0.0;		
		
		// ambient
		Ia = ka;

		if(illuminated) {					
			// diffuse		
			Id = lv.dot(isC.normal) * kd;			

			// specular
			R = -lv + isC.normal * (2 * lv.dot(isC.normal));	// reflected light ray
			V = (camera_->position_ - isectOut).normalize();	// viewer-intersection ray
			Is = pow(max(0.0, R.dot(V)), isC.obj->mat_.shininess) * ks;			
		}

		cop = light_->mat_.color * isC.obj->mat_.color * (Ia + Id + Is);

		// reflective object
		if(!inside && isC.obj->mat_.reflection > 0.0 && depth > 0) {
			cr = trace(Ray(isectOut, ray.getDir() + isC.normal * (2 * (-(ray.getDir())).dot(isC.normal))) , depth - 1, false);
		}

		// transparent object
		if(isC.obj->mat_.transparency > 0.0 && depth > 0) {
			double ref = inside ? (1.0 / isC.obj->mat_.refractIdx) : (isC.obj->mat_.refractIdx); // n1 / n2 - ratio of refr. idxs
			Vector3d normal = inside ? isC.normal : -isC.normal;
			double cosI = normal.dot(ray.getDir()); // cosine of incident ray
			Vector3d refrDir(ref * ray.getDir() + (ref * cosI - sqrt(1.0 - ref * ref * (1.0 - cosI * cosI))) * normal);
			ct = trace(Ray(inside ? isectOut : isectIn, refrDir.normalize()), depth - 1, inside ? false : true);
		}

		if(inside) {
			color = ct;
		} else {
			color = isC.obj->mat_.transparency * ct + 
				(1.0 - isC.obj->mat_.transparency) * (isC.obj->mat_.reflection * cr + (1.0 - isC.obj->mat_.reflection) * cop);
		}
	} 
	// no intersection
	else {
		if(depth == maxDepth_)
			color = Vector3d(0.25, 0.25, 0.25);	// background color - GRAY
		else
			color = Vector3d(0.0, 0.0, 0.0);	// background color - BLACK
	}

	return color;
}

#endif