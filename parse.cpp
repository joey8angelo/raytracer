#include "lights/direction_light.h"
#include "lights/point_light.h"
#include "objects/mesh.h"
#include "objects/plane.h"
#include "objects/sphere.h"
#include "shaders/flat_shader.h"
#include "shaders/normal_shader.h"
#include "shaders/phong_shader.h"
#include "world.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>

template <class T>
bool get(std::unordered_map<std::string, T> m, std::string s, T &res) {
  auto r = m.find(s);
  if (r == m.end()) {
    std::cerr << "Error: " << s << " not found\n";
    return false;
  }
  res = m[s];
  return true;
}

bool ssa(const std::stringstream &ss) {
  if (!ss) {
    std::cerr << "Error: Cannot parse input file\n";
    return false;
  }
  return true;
}

bool parse_scene(World &world, int width, int height, double ar,
                 const std::string &fn) {
  std::ifstream f(fn);
  std::string b;

  std::unordered_map<std::string, vec3> colors;
  std::unordered_map<std::string, Shader *> shaders;

  while (std::getline(f, b)) {
    std::stringstream ss(b);
    std::string entry, name, s0, s1, s2;
    vec3 u, v, w;
    double d0, d1, d2;

    if (!(ss >> entry) || !entry.size() || entry[0] == '#')
      continue;

    if (entry == "color") {
      ss >> name >> u;
      if (!ssa(ss))
        return false;
      colors[name] = u;
    } else if (entry == "sphere") {
      ss >> u >> d0 >> s0;
      if (!ssa(ss))
        return false;
      Object *o = new Sphere(u, d0);
      if (!get(shaders, s0, o->shader))
        return false;
      world.finite_objects.push_back(o);
    } else if (entry == "plane") {
      ss >> u >> v >> s0;
      if (!ssa(ss))
        return false;
      Object *o = new Plane(u, v);
      if (!get(shaders, s0, o->shader))
        return false;
      world.infinite_objects.push_back(o);
    } else if (entry == "mesh") {
      ss >> u >> d0 >> d1 >> d2 >> s0 >> s1;
      if (!ssa(ss))
        return false;
      Object *o = new Mesh(s0.c_str(), u, (pi / 180) * d0, (pi / 180) * d1,
                           (pi / 180) * d2);
      if (!get(shaders, s1, o->shader))
        return false;
      world.finite_objects.push_back(o);
    } else if (entry == "flat_shader") {
      ss >> name >> s0;
      if (!ssa(ss))
        return false;
      vec3 t;
      if (!get(colors, s0, t))
        return false;
      shaders[name] = new Flat_Shader(world, t);
      world.shaders.push_back(shaders[name]);
    } else if (entry == "phong_shader") {
      ss >> name >> s0 >> s1 >> s2 >> d0;
      if (!ssa(ss))
        return false;
      vec3 c0, c1, c2;
      if (!get(colors, s0, c0))
        return false;
      if (!get(colors, s1, c1))
        return false;
      if (!get(colors, s2, c2))
        return false;
      shaders[name] = new Phong_Shader(world, c0, c1, c2, d0);
      world.shaders.push_back(shaders[name]);
    } else if (entry == "normal_shader") {
      ss >> name;
      if (!ssa(ss))
        return false;
      shaders[name] = new Normal_Shader(world);
      world.shaders.push_back(shaders[name]);
    } else if (entry == "point_light") {
      ss >> u >> s0 >> d0;
      if (!ssa(ss))
        return false;
      vec3 t;
      if (!get(colors, s0, t))
        return false;
      Light *l = new Point_Light(u, t, d0);
      world.lights.push_back(l);
    } else if (entry == "direction_light") {
      ss >> u >> s0 >> d0;
      if (!ssa(ss))
        return false;
      vec3 t;
      if (!get(colors, s0, t))
        return 3;
      Light *l = new Direction_Light(u, t, d0);
      world.lights.push_back(l);
    } else if (entry == "ambient_light") {
      ss >> s0 >> d0;
      if (!ssa(ss))
        return false;
      if (!get(colors, s0, world.ambient_color))
        return 3;
      world.ambient_intensity = d0;
    } else if (entry == "camera") {
      ss >> u >> v >> w >> d0;
      if (!ssa(ss))
        return false;
      world.camera.set_pos_and_aim(u, v, w);
      world.camera.focus(1, static_cast<double>(width) / height * ar,
                         d0 * (pi / 180));
    } else if (entry == "background") {
      ss >> s0;
      if (!ssa(ss))
        return false;
      if (!get(shaders, s0, world.background_shader))
        return 3;
    } else if (entry == "recursion_depth") {
      ss >> world.recursion_depth_limit;
      if (!ssa(ss))
        return false;
    } else if (entry == "antialiasing") {
      ss >> world.samples;
      if (!ssa(ss))
        return false;
    } else if (entry == "#") {
      continue;
    } else {
      std::cerr << "Error: Unknown entry " << entry << "\n";
      return false;
    }
  }
  if (!world.background_shader)
    world.background_shader = new Flat_Shader(world, {0, 0, 0});

  world.bvh.build(&world.finite_objects);

  f.close();
  return true;
}
