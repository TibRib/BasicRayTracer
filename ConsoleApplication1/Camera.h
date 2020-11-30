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
	Camera() {
		float aspect_ratio = 16.f / 9.f;
		float viewport_height = 2.f;
		float viewport_width = aspect_ratio * viewport_height;
		float focal_length = 1.0;

		origin = Vector3(0, 0, 0);
		horizontal = Vector3(viewport_width, 0.0, 0.0);
		vertical = Vector3(0.0, viewport_height, 0.0);
		lower_left_corner = origin - horizontal / 2 - vertical / 2 - Vector3(0, 0, focal_length);
	}

	Ray get_ray(double u, double v) const {
		return Ray(origin, lower_left_corner + u * horizontal + v * vertical - origin);
	}
};

#endif
