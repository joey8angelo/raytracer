#pragma once

#include "vec.h"

static const double small_t = 1e-4;
extern bool debug;

class Object;
class Ray;
class Shader;

struct Hit {
	const Object* object;
	double dist;
	size_t face;
};

/* Abstract object
 *
 * intersection defines how a ray/object intersection works
 * normal is the normal vector at a point on this object
 * 	for meshes give a face index, all other objects ignore it
 * move_dir moves the object in the given direction
 * move_to moves the object to the given point in space
 */
class Object {
	public:
	Shader* shader;
	Object() : shader(0) {}
	virtual ~Object() {}
	
	virtual Hit intersection(const Ray& ray) const = 0;
	virtual vec3 normal(const vec3& point, size_t face_idx) const = 0;
	virtual vec3 center() const = 0;
};
