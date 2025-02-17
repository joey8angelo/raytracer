#pragma once

#include "object.h"

class Sphere : public Object {
	vec3 center;
	double radius;
	public:
	Sphere(const vec3& center_in, double radius_in)
		: center(center_in), radius(radius_in) {}
	
	virtual Hit intersection(const Ray& ray) const override;
	virtual vec3 normal(const vec3& point) const override;
};
