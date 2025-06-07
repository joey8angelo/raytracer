#pragma once

#include "lights/light.h"

class Direction_Light : public Light {
public:
  Direction_Light(const vec3 &direction, const vec3 &color, double brightness)
      : Light(direction.normalized() * 1e10, color, brightness) {}
  virtual vec3 emitted_light(const vec3 &vec) const {
    return color * brightness;
  }
};
