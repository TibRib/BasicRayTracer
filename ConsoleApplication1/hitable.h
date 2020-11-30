#pragma once

#ifndef HITABLE_H
#define HITABLE_H

#include "Vector3.h"
#include "Ray.h"

struct hit_record {
	float t;
	Vector3 p;
	Vector3 normal;
	bool front_face;

	inline void set_face_normal(const Ray& r, const Vector3& outward_normal) {
		front_face = dot(r.direction(), outward_normal) < 0;
		normal = front_face ? outward_normal : -outward_normal;
	}
};

class Hitable {
public:
	virtual bool hit(const Ray& r, float t_min, float t_max, hit_record& rec) const = 0;
};

#endif