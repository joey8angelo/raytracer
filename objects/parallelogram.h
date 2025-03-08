#include "objects/plane.h"

/* Parallelogram defined by a point and two vectors
 * The point is some corner
 * The two vectors define the two sides
 * */
class Parallelogram : public Plane {
    vec3 u, v;
    double uu, vv, uv, denom;
    public:
    Parallelogram(const vec3& p, const vec3& u, const vec3& v) 
        : Plane(p, {0,0,0}), u(u), v(v) { 
            norm = cross(u, v).normalized();
            uu = dot(u, u);
            vv = dot(v, v);
            uv = dot(u, v);
            denom = uu*vv - uv*uv;
        }

    virtual Hit intersection(const Ray& ray) const override;
    virtual void move_dir(const vec3& vec) override;
    virtual void move_to(const vec3& vec) override;
};
