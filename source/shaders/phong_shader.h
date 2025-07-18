#pragma once

#include "shader.h"

class Phong_Shader : public Shader {
public:
  vec3 ambient_color, diffuse_color, specular_color;
  float specular_power;

  Phong_Shader(World &world, const vec3 &ambient_color,
               const vec3 &diffuse_color, const vec3 &specular_color,
               float specular_power)
      : Shader(world), ambient_color(ambient_color),
        diffuse_color(diffuse_color), specular_color(specular_color),
        specular_power(specular_power) {}

  vec3 shade(const Ray &ray, const vec3 &point, const vec3 &normal,
             int recursion_depth) const override;
};
