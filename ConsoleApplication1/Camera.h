#pragma once

#ifndef CAMERA_H
#define CAMERA_H

#include "utils.h"
#include "Vector3.h"
#include "Ray.h"

class Camera
{
private:
	Vector3 origin;
	Vector3 lower_left_corner;
	Vector3 horizontal;
	Vector3 vertical;

public:
	Camera(float aspect_ratio = 16.f / 9.f, float viewport_height = 2.f, float focal_length =1.0f, Vector3 origin_v=Vector3(0,0,0)) {
		float viewport_width = aspect_ratio * viewport_height;

		origin = origin_v;
		horizontal = Vector3(viewport_width, 0.0, 0.0);
		vertical = Vector3(0.0, viewport_height, 0.0);
		lower_left_corner = origin - horizontal / 2 - vertical / 2 - Vector3(0, 0, focal_length);
	}

	Ray get_ray(double u, double v) const {
		return Ray(origin, lower_left_corner + u * horizontal + v * vertical - origin);
	}
	
	Vector3 getOrigin() const {
		return origin;
	}
};

#endif
