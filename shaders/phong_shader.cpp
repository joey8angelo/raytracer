#include "shaders/phong_shader.h"
#include "lights/light.h"
#include "objects/object.h"
#include "ray.h"
#include "world.h"
#include <algorithm>
#include <iostream>

vec3 Phong_Shader::shade(const Ray &ray, const vec3 &point, const vec3 &normal,
                         int recursion_depth) const {
  if (debug) {
    std::cout << "[Phong_Shader::shade]" << std::endl;
    std::cout << "  ray: " << ray.origin << ", " << ray.dir << std::endl;
    std::cout << "  point: " << point << std::endl;
    std::cout << "  normal: " << normal << std::endl;
  }

  vec3 color = ambient_color * world.ambient_color * world.ambient_intensity;

  for (size_t i = 0; i < world.lights.size(); i++) {
    vec3 lr = world.lights[i]->position - point;
    Ray shadow;
    shadow.origin = point + normal * 2 * small_t;
    shadow.set_dir(lr.normalized());

    if (debug) {
      std::cout << "[Phong_Shader] Shadow ray for light " << i;
      std::cout << ": " << shadow.origin << ", " << shadow.dir << std::endl;
    }

    Hit hit = world.closest_intersection(shadow);
    if (hit.object && hit.dist < lr.magnitude()) {
      if (debug)
        std::cout << "[Phong_Shader] Object in the way, shade" << std::endl;
      continue;
    }

    double diff_intensity =
        std::max(dot(lr.normalized(), normal.normalized()), 0.0);

    vec3 light_color = world.lights[i]->emitted_light(lr);

    color += diff_intensity * diffuse_color * light_color;

    vec3 reflection = ((2 * dot(lr, normal) * normal) - lr).normalized();
    double spec_intensity =
        pow(std::max(dot(reflection, (ray.dir * -1)), 0.0), specular_power);

    color += spec_intensity * specular_color * light_color;
  }

  return color;
}
