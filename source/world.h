#pragma once

#include "bvh.h"
#include "camera.h"
#include "objects/object.h"
#include "vec.h"
#include <vector>

class Ray;
class Light;
class Shader;

class World {
public:
  Camera camera;

  Shader *background_shader;
  std::vector<Object *> finite_objects;
  std::vector<Object *> infinite_objects;
  std::vector<Light *> lights;
  std::vector<Shader *> shaders;
  vec3 ambient_color;
  float ambient_intensity;
  int recursion_depth_limit;
  int samples;
  BVH bvh;

  World()
      : background_shader(0), ambient_intensity(0), recursion_depth_limit(3),
        samples(1) {}
  ~World();

  void render_pixel(const ivec2 &index);
  void render();

  vec3 cast_ray(const Ray &ray, int recursion_depth);
  Hit closest_intersection(const Ray &ray);
};
