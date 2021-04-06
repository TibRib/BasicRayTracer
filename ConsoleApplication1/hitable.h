#ifndef _HITABLE_H_
#define _HITABLE_H_

#include "Vector3.h"
#include "Ray.h"

struct hit_record {
	double t;
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
	virtual bool hit(const Ray& r, double t_min, double t_max, hit_record& rec) const = 0;
};

#endif