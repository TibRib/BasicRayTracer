#ifndef _METAL_MAT_H_
#define _METAL_MAT_H_

#include "Material.h"

class MetalMat : public Material
{
public:
	Color albedo;

public:
	MetalMat(const Color& albedo) : albedo(albedo) {}

	virtual bool scatter(const Ray& ray_in, const hit_record& rec, Color& attenuation, Ray& scattered) const {
		Vector3 reflected = reflect(unit_vector(ray_in.direction()), rec.normal);

		scattered = Ray(rec.p, reflected);
		attenuation = albedo;
		return (dot(scattered.direction(), rec.normal) > 0);
	}
};

#endif