#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "Ray.h"
#include "Hitable.h"
#include "Color.h"

struct hit_record;

Vector3 reflect(const Vector3& vec, const Vector3& norm) {
	return vec - 2 * dot(vec, norm) * norm;
}

class Material
{
public:
	virtual bool scatter(const Ray& ray_in, const hit_record& rec, Color& attenuation, Ray& scattered) const = 0;
};

#endif
