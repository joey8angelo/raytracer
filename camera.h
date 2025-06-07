#pragma once

#include "vec.h"

class Camera {
public:
  vec3 position;
  vec3 image_position;
  vec3 vertical;
  vec3 horizontal;
  vec3 look;

  vec2 min, max;
  vec2 image_size;
  vec2 pixel_size;
  ivec2 number_pixels;
  double focal_dist;
  unsigned int *image;

  Camera();
  ~Camera();

  void set_pos_and_aim(const vec3 &pos, const vec3 &look_at, const vec3 &sdup);
  void aim(const vec3 &look_at);
  void aim(const vec3 &look_at, const vec3 &sdup);

  void focus(double focal_dist, double aspect_ratio, double fov);
  void set_resolution(const ivec2 &number_pixels_in);

  vec3 world_position(const ivec2 &index, const vec2 &offsets = {.5, .5});
  vec2 cell_pos(const ivec2 &index, const vec2 &offsets) const;

  unsigned int pixel_color(const vec3 &color);
  void set_pixel(const ivec2 &index, const vec3 &color);
};
