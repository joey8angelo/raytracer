#include "mesh.h"
#include "../vec.h"
#include <iostream>
#include <string>

inline void make_rotation_matrix(vec<float, 16> &mat, const vec3 &p, float rX,
                                 float rY, float rZ) {
  float cx = cos(rX), sx = sin(rX);
  float cy = cos(rY), sy = sin(rY);
  float cz = cos(rZ), sz = sin(rZ);

  // Rotation part (3x3 upper-left)
  mat[0] = cy * cz;
  mat[1] = sx * sy * cz + cx * sz;
  mat[2] = -cx * sy * cz + sx * sz;
  mat[3] = 0;
  mat[4] = -cy * sz;
  mat[5] = -sx * sy * sz + cx * cz;
  mat[6] = cx * sy * sz + sx * cz;
  mat[7] = 0;
  mat[8] = sy;
  mat[9] = -sx * cy;
  mat[10] = cx * cy;
  mat[11] = 0;
  mat[12] = p[0];
  mat[13] = p[1];
  mat[14] = p[2];
  mat[15] = 1;
}

inline void invert_matrix(const vec<float, 16> &m, vec<float, 16> &inv) {
  float det;

  /* Compute adjoint: */
  inv[0] = +m[5] * m[10] * m[15] - m[5] * m[11] * m[14] - m[9] * m[6] * m[15] +
           m[9] * m[7] * m[14] + m[13] * m[6] * m[11] - m[13] * m[7] * m[10];

  inv[1] = -m[1] * m[10] * m[15] + m[1] * m[11] * m[14] + m[9] * m[2] * m[15] -
           m[9] * m[3] * m[14] - m[13] * m[2] * m[11] + m[13] * m[3] * m[10];

  inv[2] = +m[1] * m[6] * m[15] - m[1] * m[7] * m[14] - m[5] * m[2] * m[15] +
           m[5] * m[3] * m[14] + m[13] * m[2] * m[7] - m[13] * m[3] * m[6];

  inv[3] = -m[1] * m[6] * m[11] + m[1] * m[7] * m[10] + m[5] * m[2] * m[11] -
           m[5] * m[3] * m[10] - m[9] * m[2] * m[7] + m[9] * m[3] * m[6];

  inv[4] = -m[4] * m[10] * m[15] + m[4] * m[11] * m[14] + m[8] * m[6] * m[15] -
           m[8] * m[7] * m[14] - m[12] * m[6] * m[11] + m[12] * m[7] * m[10];

  inv[5] = +m[0] * m[10] * m[15] - m[0] * m[11] * m[14] - m[8] * m[2] * m[15] +
           m[8] * m[3] * m[14] + m[12] * m[2] * m[11] - m[12] * m[3] * m[10];

  inv[6] = -m[0] * m[6] * m[15] + m[0] * m[7] * m[14] + m[4] * m[2] * m[15] -
           m[4] * m[3] * m[14] - m[12] * m[2] * m[7] + m[12] * m[3] * m[6];

  inv[7] = +m[0] * m[6] * m[11] - m[0] * m[7] * m[10] - m[4] * m[2] * m[11] +
           m[4] * m[3] * m[10] + m[8] * m[2] * m[7] - m[8] * m[3] * m[6];

  inv[8] = +m[4] * m[9] * m[15] - m[4] * m[11] * m[13] - m[8] * m[5] * m[15] +
           m[8] * m[7] * m[13] + m[12] * m[5] * m[11] - m[12] * m[7] * m[9];

  inv[9] = -m[0] * m[9] * m[15] + m[0] * m[11] * m[13] + m[8] * m[1] * m[15] -
           m[8] * m[3] * m[13] - m[12] * m[1] * m[11] + m[12] * m[3] * m[9];

  inv[10] = +m[0] * m[5] * m[15] - m[0] * m[7] * m[13] - m[4] * m[1] * m[15] +
            m[4] * m[3] * m[13] + m[12] * m[1] * m[7] - m[12] * m[3] * m[5];

  inv[11] = -m[0] * m[5] * m[11] + m[0] * m[7] * m[9] + m[4] * m[1] * m[11] -
            m[4] * m[3] * m[9] - m[8] * m[1] * m[7] + m[8] * m[3] * m[5];

  inv[12] = -m[4] * m[9] * m[14] + m[4] * m[10] * m[13] + m[8] * m[5] * m[14] -
            m[8] * m[6] * m[13] - m[12] * m[5] * m[10] + m[12] * m[6] * m[9];

  inv[13] = +m[0] * m[9] * m[14] - m[0] * m[10] * m[13] - m[8] * m[1] * m[14] +
            m[8] * m[2] * m[13] + m[12] * m[1] * m[10] - m[12] * m[2] * m[9];

  inv[14] = -m[0] * m[5] * m[14] + m[0] * m[6] * m[13] + m[4] * m[1] * m[14] -
            m[4] * m[2] * m[13] - m[12] * m[1] * m[6] + m[12] * m[2] * m[5];

  inv[15] = +m[0] * m[5] * m[10] - m[0] * m[6] * m[9] - m[4] * m[1] * m[10] +
            m[4] * m[2] * m[9] + m[8] * m[1] * m[6] - m[8] * m[2] * m[5];

  /* Compute determinant: */
  det = +m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

  /* Multiply adjoint with reciprocal of determinant: */
  det = 1.0f / det;

  inv[0] *= det;
  inv[1] *= det;
  inv[2] *= det;
  inv[3] *= det;
  inv[4] *= det;
  inv[5] *= det;
  inv[6] *= det;
  inv[7] *= det;
  inv[8] *= det;
  inv[9] *= det;
  inv[10] *= det;
  inv[11] *= det;
  inv[12] *= det;
  inv[13] *= det;
  inv[14] *= det;
  inv[15] *= det;
}

// transform a given vector by a transformation matrix
inline vec3 transform_vec(const vec3 &v, const vec<float, 16> &mat) {
  return {v[0] * mat[0] + v[1] * mat[1] + v[2] * mat[2],
          v[0] * mat[4] + v[1] * mat[5] + v[2] * mat[6],
          v[0] * mat[8] + v[1] * mat[9] + v[2] * mat[10]};
}
// transform a given point by a transformation matrix
inline vec3 transform_point(const vec3 &p, const vec<float, 16> &mat) {
  return transform_vec(p, mat) + vec3{mat[12], mat[13], mat[14]};
}
// transform a given ray by a transformation matrix
inline Ray transform_ray(const Ray &ray, const vec<float, 16> &mat) {
  vec3 origin = transform_point(ray.origin, mat);
  vec3 dir = transform_vec(ray.dir, mat);
  return {origin, dir};
}

Mesh::Mesh(const char *filename, const vec3 &pos, float rX, float rY,
           float rZ) {
  parse_obj(filename);
  make_rotation_matrix(mat, pos, rX, rY, rZ);
  invert_matrix(mat, inv_mat);
  if (debug) {
    std::cout << "[Mesh::Mesh] mat: " << mat << std::endl;
    std::cout << "[Mesh::Mesh] inv_mat: " << inv_mat << std::endl;
  }
  bvh.build(&tris);
}

vec3 Mesh::center() const { return {mat[3], mat[7], mat[11]}; }

Hit Mesh::intersection(const Ray &ray) const {
  if (debug) {
    std::cout << "[Mesh::intersection] ray: " << ray.origin << " " << ray.dir
              << std::endl;
  }
  // transform ray to object space
  const Ray r = transform_ray(ray, inv_mat);

  Hit hit = bvh.intersection(r);
  if (hit.object) {
    if (debug) {
      std::cout << "[Mesh::intersection] bvh hit: " << hit.dist << " "
                << hit.face << std::endl;
    }
    return {this, hit.dist, hit.face};
  }
  return {NULL, 0, 0};
}

vec3 Mesh::normal(const vec3 &point, size_t face_idx) const {
  vec3 p = transform_point(point, inv_mat);
  return transform_vec(tris[face_idx]->normal(p, 0), mat).normalized();
}

void Mesh::parse_obj(const char *filename) {
  bool warn = false;

  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cerr << "[Mesh::parse_obj] Could not open file " << filename
              << std::endl;
    return;
  }

  std::string line;
  while (std::getline(file, line)) {
    vec3 v0;
    ivec3 iv0;
    ivec3 iv1;

    std::stringstream ss(line);
    std::string type;
    ss >> type;
    if (type == "v") {
      ss >> v0;
      vertices.push_back(v0);
    } else if (type == "vn") {
      ss >> v0;
      normals.push_back(v0);
    } else if (type == "vt") {
      if (!warn) {
        warn = true;
        std::cerr
            << "[Mesh::parse_obj] Warning: Texture coordinates not supported"
            << std::endl;
      }
    } else if (type == "f") {
      for (auto i = 0; i < 3; i++) {
        std::string s;
        ss >> s;
        vec<std::string, 3> t = split(s, '/');

        // vertex index
        iv0[i] = std::stoi(t[0]) - 1;

        // normal index
        if (t[2] != "")
          iv1[i] = std::stoi(t[2]) - 1;
      }

      tris.push_back(new Triangle(&vertices, &normals, iv0[0], iv0[1], iv0[2],
                                  iv1[0], iv1[1], iv1[2], tris.size()));
    }
  }

  file.close();
}

Bounding_Box Mesh::get_bounding_box() const {
  Bounding_Box bb;
  vec3 min = transform_point(bb.bounds[0], mat);
  vec3 max = transform_point(bb.bounds[1], mat);
  bb.include(min);
  bb.include(max);
  return bb;
}

vec<std::string, 3> Mesh::split(const std::string &s, char delim) {
  std::stringstream ss(s);
  vec<std::string, 3> v = {"", "", ""};
  for (auto i = 0; i < 3; i++) {
    if (!std::getline(ss, v[i], delim))
      break;
  }
  return v;
}
