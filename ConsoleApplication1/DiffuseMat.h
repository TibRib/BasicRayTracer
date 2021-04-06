#ifndef _DIFFUSE_MAT_H_
#define _DIFFUSE_MAT_H_

#include "Material.h"
class DiffuseMat : public Material
{
public:
	Color albedo;

public:
	DiffuseMat(const Color& albedo) : albedo(albedo) {}

	virtual bool scatter(const Ray& ray_in, const hit_record& rec, Color& attenuation, Ray& scattered) const {
		auto scatter_direction = random_in_hemisphere(rec.normal);
		
		// Catch special exact-opposite case
		if (scatter_direction.near_zero())
			scatter_direction = rec.normal;

		scattered = Ray(rec.p, scatter_direction);
		attenuation = albedo;

		return true;
	}
};

#endif

