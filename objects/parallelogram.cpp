#include "objects/parallelogram.h"
#include "ray.h"

Hit Parallelogram::intersection(const Ray& ray) const {
    Hit hit = Plane::intersection(ray);

    if(!hit.object)
        return hit;

    vec3 A = ray.at(hit.dist);
    vec3 AP = A-p;
    auto apu = dot(AP, u);
    auto apv = dot(AP, v);
    auto s = (apu * vv - apv*uv)/denom;
    auto t = (apv * uu - apu*uv)/denom;
    if(0 <= s && s <= 1 && 0 <= t && t <= 1)
        return {this, hit.dist};
    return {NULL, 0};
}

void Parallelogram::move_dir(const vec3& vec) {
    p += vec;
}

void Parallelogram::move_to(const vec3& vec) {
    p = vec;
}
