#pragma once

#include "vec.h"

static const double small_t = 1e-4;

class Object;
class Ray;

struct Hit {
	const Object* object;
	double dist;
};

class Object {
	public:
	Object() {}
	virtual ~Object() {}
	
	virtual Hit intersection(const Ray& ray) const = 0;
	virtual vec3 normal(const vec3& point) const = 0;
};
