#pragma once

#include "objects/object.h"

/* Infinite plane, defined by a point and a normal vector
 * */
class Plane : public Object {
protected:
  vec3 p;
  vec3 norm;

public:
  Plane(const vec3 &p, const vec3 &norm) : p(p), norm(norm) {}

  Hit intersection(const Ray &ray) const override;
  vec3 normal(const vec3 &point, size_t face_idx) const override;
  vec3 center() const override;
  Bounding_Box get_bounding_box() const override;
};
