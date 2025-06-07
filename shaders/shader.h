#pragma once

#include "vec.h"

extern bool debug;

class Ray;
class World;

class Shader {
public:
  World &world;

  explicit Shader(World &world) : world(world) {}

  virtual ~Shader() {}

  virtual vec3 shade(const Ray &ray, const vec3 &point, const vec3 &normal,
                     int recursion_depth) const = 0;
};
