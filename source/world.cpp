#include "world.h"
#include "lights/light.h"
#include "objects/object.h"
#include "ray.h"
#include "shaders/shader.h"
#include "util.h"
#include <iostream>
#include <omp.h>

World::~World() {
  // delete objects
  for (size_t i = 0; i < finite_objects.size(); i++)
    delete finite_objects[i];
  for (size_t i = 0; i < infinite_objects.size(); i++)
    delete infinite_objects[i];
  // delete lights
  for (size_t i = 0; i < lights.size(); i++)
    delete lights[i];
  // delete shaders
  for (size_t i = 0; i < shaders.size(); i++)
    delete shaders[i];
  delete background_shader;
}

Hit World::closest_intersection(const Ray &ray) {
  if (debug) {
    std::cout << "[World::closest_intersection] ray: " << ray.origin << " "
              << ray.dir << std::endl;
  }
  Hit curr_hit = bvh.intersection(ray);
  for (size_t i = 0; i < infinite_objects.size(); i++) {
    Hit hit = infinite_objects[i]->intersection(ray);
    if (hit.object && (!curr_hit.object || hit.dist < curr_hit.dist)) {
      curr_hit = hit;
    }
  }
  for (size_t i = 0; i < finite_objects.size(); i++) {
    Hit hit = finite_objects[i]->intersection(ray);
    if (hit.object && (!curr_hit.object || hit.dist < curr_hit.dist)) {
      curr_hit = hit;
    }
  }
  return curr_hit;
}

void World::render() {
#pragma omp parallel for
  for (int i = 0; i < camera.number_pixels[0]; i++) {
    for (int j = 0; j < camera.number_pixels[1]; j++) {
      render_pixel(ivec2(i, j));
    }
  }
}

void World::render_pixel(const ivec2 &index) {
  vec3 color;
  Ray ray;
  ray.origin = camera.position;
  for (int i = 0; i < samples; i++) {
    vec2 off;
    if (i != 0) {
      off = {xorshift_0_1(index[0], index[1], i) - 0.5f,
             xorshift_0_1(index[1], index[0], i) - 0.5f};
    }
    ray.set_dir((camera.world_position(index, off) - ray.origin).normalized());
    color += cast_ray(ray, recursion_depth_limit);
  }
  color /= samples;
  camera.set_pixel(index, color);
}

vec3 World::cast_ray(const Ray &ray, int recursion_depth) {
  Hit hit = closest_intersection(ray);
  if (hit.object) {
    vec3 intersection = ray.at(hit.dist);
    return hit.object->shader->shade(ray, intersection,
                                     hit.object->normal(intersection, hit.face),
                                     recursion_depth);
  } else {
    return background_shader->shade(ray, {0, 0, 0}, {0, 0, 0}, recursion_depth);
  }
}
