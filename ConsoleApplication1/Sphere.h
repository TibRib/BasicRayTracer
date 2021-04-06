#ifndef _SPHERE_H_
#define _SPHERE_H_

#include "hitable.h"
#include <cmath>

class Sphere : public Hitable {
public:
	Vector3 center;
	double radius;
	shared_ptr<Material> mat_ptr;

	Sphere(){}
	Sphere(Vector3 cen, double r, shared_ptr<Material> mat) 
		: center(cen), radius(r), mat_ptr(mat) {};
	
	virtual bool hit(const Ray&r, double t_min, double t_max, hit_record& rec) const;
	
};

bool Sphere::hit(const Ray&r, double t_min, double t_max, hit_record& rec) const {
	Vector3 oc = r.origin() - center;
	auto a = r.direction().squared_length();
	auto half_b = dot(oc, r.direction());
	auto c = oc.squared_length() - radius * radius;

	auto discriminant = half_b * half_b - a * c;
	if (discriminant < 0) return false;
	auto sqrtd = sqrt(discriminant);

	// Find the nearest root that lies in the acceptable range.
	auto root = (-half_b - sqrtd) / a;
	if (root < t_min || t_max < root) {
		root = (-half_b + sqrtd) / a;
		if (root < t_min || t_max < root)
			return false;
	}

	rec.t = root;
	rec.p = r.point_at_parameter(rec.t);
	rec.normal = (rec.p - center) / radius;
	Vector3 outward_normal = (rec.p - center) / radius;
	rec.set_face_normal(r, outward_normal);
	rec.mat_ptr = mat_ptr;

	return true;
}


#endif