#pragma once

#include "vec.h"

struct Ray {
	Ray() {}
	Ray(const vec3& origin, const vec3& dir) : origin(origin), dir(dir) {}

	vec3 origin;
	vec3 dir;

	vec3 at(double t) const {
		return origin + t*dir;
	}
};
