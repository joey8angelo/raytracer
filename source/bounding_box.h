#pragma once

#include "ray.h"
#include "vec.h"
#include <cstddef>
#include <limits>

extern bool debug;

// https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-box-intersection.html
class Bounding_Box {
public:
  vec3 bounds[2]; // min, max

  Bounding_Box() {
    bounds[0].fill(std::numeric_limits<float>::max());
    bounds[1].fill(std::numeric_limits<float>::min());
  }

  float intersection(const Ray &ray) const {
    float tmin, tmax, tymin, tymax, tzmin, tzmax;

    tmin = (bounds[ray.sign[0]][0] - ray.origin[0]) * ray.inv_dir[0];
    tmax = (bounds[1 - ray.sign[0]][0] - ray.origin[0]) * ray.inv_dir[0];
    tymin = (bounds[ray.sign[1]][1] - ray.origin[1]) * ray.inv_dir[1];
    tymax = (bounds[1 - ray.sign[1]][1] - ray.origin[1]) * ray.inv_dir[1];

    if ((tmin > tymax) || (tymin > tmax))
      return 0;
    if (tymin > tmin)
      tmin = tymin;
    if (tymax < tmax)
      tmax = tymax;

    tzmin = (bounds[ray.sign[2]][2] - ray.origin[2]) * ray.inv_dir[2];
    tzmax = (bounds[1 - ray.sign[2]][2] - ray.origin[2]) * ray.inv_dir[2];

    if ((tmin > tzmax) || (tzmin > tmax))
      return 0;
    if (tzmin > tmin)
      tmin = tzmin;
    if (tzmax < tmax)
      tmax = tzmax;

    float dist = tmin;
    if (tmin < 0) {
      dist = tmax;
      if (tmax < 0)
        return 0;
    }

    if (tmin < 0 && tmax < 0)
      return 0;

    return dist;
  }

  void include(const vec3 &point) {
    bounds[0] = componentwise_min(bounds[0], point);
    bounds[1] = componentwise_max(bounds[1], point);
  }

  void include(const Bounding_Box &bb) {
    bounds[0] = componentwise_min(bounds[0], bb.bounds[0]);
    bounds[1] = componentwise_max(bounds[1], bb.bounds[1]);
  }
};
