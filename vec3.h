//==================================================================================================
// Written in 2016 by Peter Shirley <ptrshrl@gmail.com>
//
// To the extent possible under law, the author(s) have dedicated all copyright and related and
// neighboring rights to this software to the public domain worldwide. This software is distributed
// without any warranty.
//
// You should have received a copy (see file COPYING.txt) of the CC0 Public Domain Dedication along
// with this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
//==================================================================================================

#ifndef VEC3H
#define VEC3H

#include <math.h>
#include <stdlib.h>
#include <iostream>

class vec3  {


	public:
		vec3() {}
		vec3(float e0, float e1, float e2) { x = e0; y = e1; z = e2; }

		inline const vec3& operator+() const { return *this; }
		inline vec3 operator-() const { return vec3(-x, -y, -z); }
		inline vec3& operator+=(const vec3 &v2);
		inline vec3& operator-=(const vec3 &v2);
		inline vec3& operator*=(const vec3 &v2);
		inline vec3& operator/=(const vec3 &v2);
		inline vec3& operator*=(const float t);
		inline vec3& operator/=(const float t);

		inline float size() const { return sqrt(x*x + y*y + z*z); }
		inline float squared_size() const { return x*x + y*y + z*z; }
		inline void normalize();


		float x,y,z;
};



inline std::istream& operator>>(std::istream &is, vec3 &t) {
	is >> t.x >> t.y >> t.z;
	return is;
}

inline std::ostream& operator<<(std::ostream &os, const vec3 &t) {
	os << t.x << " " << t.y << " " << t.z;
	return os;
}

inline void vec3::normalize() {
	float k = 1.0 / sqrt(x*x + y*y + z*z);
	x *= k; y *= k; z *= k;
}

inline vec3 operator+(const vec3 &v1, const vec3 &v2) {
	return vec3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

inline vec3 operator-(const vec3 &v1, const vec3 &v2) {
	return vec3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

inline vec3 operator*(const vec3 &v1, const vec3 &v2) {
	return vec3(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
}

inline bool operator==(const vec3 &v1, const vec3 &v2) {
	return (v1.x == v2.x) && (v1.y == v2.y) && (v1.z == v2.z);
}

inline vec3 operator/(const vec3 &v1, const vec3 &v2) {
	return vec3(v1.x / v2.x, v1.y / v2.y, v1.z / v2.z);
}

inline vec3 operator*(float t, const vec3 &v) {
	return vec3(t*v.x, t*v.y, t*v.z);
}

inline vec3 operator/(vec3 v, float t) {
	return vec3(v.x/t, v.y/t, v.z/t);
}

inline vec3 operator*(const vec3 &v, float t) {
	return vec3(t*v.x, t*v.y, t*v.z);
}

inline float dot(const vec3 &v1, const vec3 &v2) {
	return v1.x *v2.x + v1.y *v2.y  + v1.z *v2.z;
}

inline vec3 cross(const vec3 &v1, const vec3 &v2) {
	return vec3( (v1.y*v2.z - v1.z*v2.y),
			(-(v1.x*v2.z - v1.z*v2.x)),
			(v1.x*v2.y - v1.y*v2.x));
}


inline vec3& vec3::operator+=(const vec3 &v){
	x  += v.x;
	y  += v.y;
	z  += v.z;
	return *this;
}

inline vec3& vec3::operator*=(const vec3 &v){
	x  *= v.x;
	y  *= v.y;
	z  *= v.z;
	return *this;
}

inline vec3& vec3::operator/=(const vec3 &v){
	x  /= v.x;
	y  /= v.y;
	z  /= v.z;
	return *this;
}

inline vec3& vec3::operator-=(const vec3& v) {
	x  -= v.x;
	y  -= v.y;
	z  -= v.z;
	return *this;
}

inline vec3& vec3::operator*=(const float t) {
	x  *= t;
	y  *= t;
	z  *= t;
	return *this;
}

inline vec3& vec3::operator/=(const float t) {
	float k = 1.0/t;

	x  *= k;
	y  *= k;
	z  *= k;
	return *this;
}

inline vec3 unit_vector(vec3 v) {
	return v / v.size();
}

#endif
