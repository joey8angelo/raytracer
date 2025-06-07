#pragma once

#include "shader.h"

class Normal_Shader : public Shader {
public:
  Normal_Shader(World &world) : Shader(world) {}

  vec3 shade(const Ray &ray, const vec3 &point, const vec3 &normal,
             int recursion_depth) const override {
    // Return the normal as color
    return normal;
  }
};
