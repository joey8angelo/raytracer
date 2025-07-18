#pragma once

#include "shader.h"

class Flat_Shader : public Shader {
public:
  vec3 color;

  Flat_Shader(World &world, const vec3 &color) : Shader(world), color(color) {}

  vec3 shade(const Ray &ray, const vec3 &point, const vec3 &normal,
             int recursion_depth) const override;
};
