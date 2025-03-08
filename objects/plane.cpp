#include "objects/plane.h"
#include "ray.h"

Hit Plane::intersection(const Ray& ray) const {
    if (dot(norm, ray.dir) >= 0)
        return {NULL, 0};
    auto t = dot(norm, p - ray.origin) / dot(norm, ray.dir);
    if (t > small_t)
        return {this, t};
    else
        return {NULL, 0};
}

vec3 Plane::normal(const vec3& point) const {
    return norm;
}

void Plane::move_dir(const vec3& vec) {
    p += vec;
}

void Plane::move_to(const vec3& vec) {
    p = vec;
}
