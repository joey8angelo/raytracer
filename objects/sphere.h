#pragma once

#include "objects/object.h"

/* Sphere, defined by a point in space and a radius
 * */
class Sphere : public Object {
  vec3 pos;
  double radius;

public:
  Sphere(const vec3 &pos, double radius) : pos(pos), radius(radius) {}

  Hit intersection(const Ray &ray) const override;
  vec3 normal(const vec3 &point, size_t face_idx) const override;
  vec3 center() const override;
  Bounding_Box get_bounding_box() const override;
};
