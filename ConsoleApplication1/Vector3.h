#ifndef _VECTOR3_H_
#define _VECTOR3_H_

#include "utils.h"
#include "pch.h"
using namespace std;

class Vector3
{
public:
	Vector3() {}

	Vector3(double e0, double e1, double e2) {
		e[0] = e0;
		e[1] = e1;
		e[2] = e2;
	}
	inline double x() const { return e[0]; }
	inline double y() const { return e[1]; }
	inline double z() const { return e[2]; }
	inline double r() const { return e[0]; }
	inline double g() const { return e[1]; }
	inline double b() const { return e[2]; }

	inline const void print() {
		cout << e[0]<< ", "<< e[1]<< ", " << e[2] << endl;
	}

	inline static Vector3 random() {
		return Vector3(random_double(), random_double(), random_double());
	}

	inline static Vector3 random(double min, double max) {
		return Vector3(random_double(min, max), random_double(min, max), random_double(min, max));
	}

	inline const Vector3& operator+() const { return*this; }
	inline Vector3 operator-() const { return Vector3(-e[0], -e[1], -e[2]); }
	inline double operator[](int i) const { return e[i]; }
	inline double& operator[](int i) { return e[i]; }

	inline Vector3& operator+=(const Vector3 &v2);
	inline Vector3& operator-=(const Vector3 &v2);
	inline Vector3& operator*=(const Vector3 &v2);
	inline Vector3& operator/=(const Vector3 &v2);
	inline Vector3& operator*=(const double t);
	inline Vector3& operator/=(const double t);

	inline double length() const{
		return sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]);
	}
	inline double squared_length() const {
		return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
	}
	inline void make_unit_vector();

	double e[3];
};


inline std::istream& operator>>(std::istream &is, Vector3 &t) {
	is >> t.e[0] >> t.e[1] >> t.e[2];
	return is;
}

inline std::ostream& operator<<(std::ostream &os, const Vector3 &t) {
	os << t.e[0] << " " << t.e[1] << " " << t.e[2];
	return os;
}

inline void Vector3::make_unit_vector() {
	double k = 1.0 / sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]);
	e[0] *= k; e[1] *= k; e[2] *= k;
}

inline Vector3 operator+(const Vector3 &v1, const Vector3 &v2) {
	return Vector3(v1.e[0] + v2.e[0], v1.e[1] + v2.e[1], v1.e[2] + v2.e[2]);
}
inline Vector3 operator-(const Vector3 &v1, const Vector3 &v2) {
	return Vector3(v1.e[0] - v2.e[0], v1.e[1] - v2.e[1], v1.e[2] - v2.e[2]);
}
inline Vector3 operator*(const Vector3 &v1, const Vector3 &v2) {
	return Vector3(v1.e[0] * v2.e[0], v1.e[1] * v2.e[1], v1.e[2] * v2.e[2]);
}
inline Vector3 operator/(const Vector3 &v1, const Vector3 &v2) {
	return Vector3(v1.e[0] / v2.e[0], v1.e[1] / v2.e[1], v1.e[2] / v2.e[2]);
}
inline Vector3 operator*(double t, const Vector3 &v) {
	return Vector3(t*v.e[0], t*v.e[1], t*v.e[2]);
}

inline Vector3 operator/(Vector3 v, double t) {
	return Vector3(v.e[0] / t, v.e[1] / t, v.e[2] / t);
}

inline Vector3 operator*(const Vector3 &v, double t) {
	return Vector3(t*v.e[0], t*v.e[1], t*v.e[2]);
}

inline double dot(const Vector3 &v1, const Vector3 &v2) {
	return		v1.e[0] * v2.e[0] 
			+	v1.e[1] * v2.e[1] 
			+	v1.e[2] * v2.e[2];
}

inline Vector3 cross(const Vector3 &v1, const Vector3 &v2) {
	return Vector3(	v1.e[1] * v2.e[2] - v1.e[2] * v2.e[1],
					v1.e[2] * v2.e[0] - v1.e[0] * v2.e[2],
					v1.e[0] * v2.e[1] - v1.e[1] * v2.e[0]);
}

inline Vector3& Vector3::operator+=(const Vector3 &v) {
	e[0] += v.e[0];
	e[1] += v.e[1];
	e[2] += v.e[2];
	return *this;
}

inline Vector3& Vector3::operator*=(const Vector3 &v) {
	e[0] *= v.e[0];
	e[1] *= v.e[1];
	e[2] *= v.e[2];
	return *this;
}

inline Vector3& Vector3::operator/=(const Vector3 &v) {
	e[0] /= v.e[0];
	e[1] /= v.e[1];
	e[2] /= v.e[2];
	return *this;
}

inline Vector3& Vector3::operator-=(const Vector3 &v) {
	e[0] -= v.e[0];
	e[1] -= v.e[1];
	e[2] -= v.e[2];
	return *this;
}

inline Vector3& Vector3::operator*=(const double t) {
	e[0] *= t;
	e[1] *= t;
	e[2] *= t;
	return *this;
}

inline Vector3& Vector3::operator/=(const double t) {
	double k = 1.0 / t;

	e[0] *= k;
	e[1] *= k;
	e[2] *= k;
	return *this;
}

inline Vector3 unit_vector(Vector3 v) {
	return v / v.length();
}

inline Vector3 random_in_unit_sphere() {
	while (true) {
		auto p = Vector3::random(-1, 1);
		if (p.squared_length() >= 1) continue;
		return p;
	}
}

inline Vector3 random_unit_vector() {
	return unit_vector(random_in_unit_sphere());
}



#endif