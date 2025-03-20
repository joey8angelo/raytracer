#pragma once

#include "vec.h"
#include "ray.h"
#include <limits>

extern bool debug;

class Bounding_Box {
    public:
    vec3 min;
    vec3 max;

    Bounding_Box() {
        min.fill(std::numeric_limits<double>::max());
        max.fill(std::numeric_limits<double>::min());
    }

    bool intersection(const Ray& ray) const {
        if (debug) {
            std::cout << ray.dir << " " << ray.inv_dir << std::endl;
        }
        vec3 t_min = (min - ray.origin) * ray.inv_dir;
        vec3 t_max = (max - ray.origin) * ray.inv_dir;
        if (debug)
            std::cout << t_min << " " << t_max << std::endl;
        vec3 t1 = componentwise_min(t_min, t_max);
        vec3 t2 = componentwise_max(t_min, t_max);
        double far_d = std::min(std::min(t2[0], t2[1]), t2[2]);
        double near_d = std::max(std::max(t1[0], t1[1]), t1[2]);
        if (debug)
            std::cout << far_d << ", " << near_d << std::endl;
        return (far_d >= near_d && far_d > 0);
    }

    void include(const vec3& point) {
        min = componentwise_min(min, point);
        max = componentwise_max(max, point);
    }
};
