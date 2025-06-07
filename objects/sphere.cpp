#include "objects/sphere.h"
#include "ray.h"

Hit Sphere::intersection(const Ray &ray) const {
  vec3 oc = ray.origin - pos;
  auto a = dot(ray.dir, ray.dir);
  auto b = 2 * dot(ray.dir, oc);
  auto c = dot(oc, oc) - radius * radius;
  auto discriminant = b * b - 4 * a * c;

  if (discriminant < 0)
    return {NULL, 0, 0};

  double t = (-b - sqrt(discriminant)) / (2 * a);
  if (t > small_t)
    return {this, t, 0};

  return {NULL, 0, 0};
}

vec3 Sphere::normal(const vec3 &point, size_t face) const {
  return (point - pos).normalized();
}

vec3 Sphere::center() const { return pos; }

Bounding_Box Sphere::get_bounding_box() const {
  Bounding_Box bb;
  bb.include(pos - vec3(radius, radius, radius));
  bb.include(pos + vec3(radius, radius, radius));
  return bb;
}
