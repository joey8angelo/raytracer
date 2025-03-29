#pragma once

#include "vec.h"

struct Ray {
	Ray() {}
	Ray(const vec3& origin, const vec3& dir) : origin(origin), dir(dir) {
		set_inv_dir();
	}

	vec3 origin;
	vec3 dir;
	vec3 inv_dir;

	vec3 at(double t) const {
		return origin + t*dir;
	}

	void set_inv_dir() {
		inv_dir = vec3(1.0/dir[0], 1.0/dir[1], 1.0/dir[2]).normalized();
	}

	void set_dir(const vec3& v) {
		dir = v;
		set_inv_dir();
	}
};

