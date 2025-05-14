#include "objects/plane.h"
#include "ray.h"

Hit Plane::intersection(const Ray& ray) const {
    if (dot(norm, ray.dir) >= 0)
        return {NULL, 0, 0};
    auto t = dot(norm, p - ray.origin) / dot(norm, ray.dir);
    if (t > small_t)
        return {this, t, 0};
    else
        return {NULL, 0, 0};
}

vec3 Plane::normal(const vec3& point, size_t face_idx) const {
    return norm;
}

vec3 Plane::center() const {
    return p;
}

Bounding_Box Plane::get_bounding_box() const {
    throw std::runtime_error("Plane does not have a bounding box");
}