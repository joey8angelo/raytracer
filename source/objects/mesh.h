#pragma once

#include "../bounding_box.h"
#include "../bvh.h"
#include "object.h"
#include "triangle.h"
#include <climits>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

/* Mesh, from an obj file */
class Mesh : public Object {
  std::vector<vec3> vertices;
  std::vector<vec3> normals;
  std::vector<Object *> tris;
  BVH bvh;

  // transformation matrices
  vec<float, 16> mat;
  vec<float, 16> inv_mat;

  void parse_obj(const char *filename);
  vec<std::string, 3> split(const std::string &s, char delim);

public:
  Mesh(const char *filename, const vec3 &pos, float rX, float rY, float rZ);
  ~Mesh() {
    for (size_t i = 0; i < tris.size(); i++) {
      delete tris[i];
    }
  }

  Hit intersection(const Ray &ray) const override;
  vec3 normal(const vec3 &point, size_t face_idx) const override;
  vec3 center() const override;
  Bounding_Box get_bounding_box() const override;
};
