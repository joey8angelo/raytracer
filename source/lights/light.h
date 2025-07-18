#pragma once

#include "../vec.h"

extern bool debug;

class Light {
public:
  vec3 position;
  vec3 color;
  float brightness;

  Light() : position(), color(1, 1, 1), brightness(1) {}
  Light(const vec3 &pos, const vec3 &col, float bright)
      : position(pos), color(col), brightness(bright) {}

  virtual ~Light() {}

  virtual vec3 emitted_light(const vec3 &vec) const = 0;
};
