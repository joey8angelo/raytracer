#pragma once

#include "vec.h"

struct Ray {
  Ray() {}
  Ray(const vec3 &origin, const vec3 &dir) : origin(origin), dir(dir) {
    set_inv_dir();
  }

  vec3 origin, dir, inv_dir;
  ivec3 sign;

  vec3 at(double t) const { return origin + t * dir; }

  void set_inv_dir() {
    inv_dir = {1 / dir[0], 1 / dir[1], 1 / dir[2]};
    sign[0] = (inv_dir[0] < 0) ? 1 : 0;
    sign[1] = (inv_dir[1] < 0) ? 1 : 0;
    sign[2] = (inv_dir[2] < 0) ? 1 : 0;
  }

  void set_dir(const vec3 &v) {
    dir = v;
    set_inv_dir();
  }
};
