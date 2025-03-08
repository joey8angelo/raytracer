#pragma once

#include "light.h"

class Point_Light : public Light {
	public:
	Point_Light(const vec3& position, const vec3& color,
		    double brightness)
		    : Light(position, color, brightness) {}

	virtual vec3 emitted_light(const vec3& vec) const {
		return color*brightness/(4*pi*vec.magnitude_squared());
	}
};
