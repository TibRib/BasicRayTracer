#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "utils.h"
#include "Vector3.h"
#include "Ray.h"

class Camera
{
private:
	Vector3 _origin;
	Vector3 _lower_left_corner;
	Vector3 _horizontal;
	Vector3 _vertical;

public:
	Camera(double vfov,  double aspect_ratio ) {
		auto theta = degrees_to_radians(vfov);
		auto h = tan(theta / 2);
		auto viewport_height = 2.0 * h;
		auto viewport_width = aspect_ratio * viewport_height;
		auto focal_length = 1.0;
		
		_origin = Vector3(0,0,0);
		_horizontal = Vector3(viewport_width, 0.0, 0.0);
		_vertical = Vector3(0.0, viewport_height, 0.0);
		_lower_left_corner = _origin - _horizontal / 2 - _vertical / 2 - Vector3(0, 0, focal_length);
	}

	Ray get_ray(double u, double v) const {
		return Ray(_origin, _lower_left_corner + u * _horizontal + v * _vertical - _origin);
	}
	
	Vector3 getOrigin() const {
		return _origin;
	}


};

#endif
