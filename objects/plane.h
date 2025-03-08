#pragma once

#include "objects/object.h"

/* Infinite plane, defined by a point and a normal vector
 * */
class Plane : public Object {
    protected:
    vec3 p;
    vec3 norm;
    public:
    Plane(const vec3& p, const vec3& norm) : p(p), norm(norm) {}

    virtual Hit intersection(const Ray& ray) const override;
    virtual vec3 normal(const vec3& point) const override;
    virtual void move_dir(const vec3& vec) override;
    virtual void move_to(const vec3& vec) override;
};
