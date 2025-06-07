#include "objects/triangle.h"
#include <iostream>

Hit Triangle::intersection(const Ray &ray) const {
  vec3 edge1 = verts->at(v2) - verts->at(v1);
  vec3 edge2 = verts->at(v3) - verts->at(v1);
  vec3 h = cross(ray.dir, edge2);
  double a = dot(edge1, h);
  double f = 1 / a;
  vec3 s = ray.origin - verts->at(v1);
  double u = f * dot(s, h);
  if (u < 0 || u > 1) {
    return {NULL, 0, 0};
  }
  vec3 q = cross(s, edge1);
  double v = f * dot(ray.dir, q);
  if (v < 0 || u + v > 1) {
    return {NULL, 0, 0};
  }
  double t = f * dot(edge2, q);
  if (t >= small_t) {
    return {this, t, idx};
  } else {
    return {NULL, 0, 0};
  }
}

vec3 Triangle::normal(const vec3 &point, size_t face_idx) const {
  if (debug) {
    std::cout << "[Triangle::normal] point: " << point << " face: " << idx
              << std::endl;
  }

  // obj file gave no normals for vertices
  if (n1 == n2 && n2 == n3 && n1 == 0) {
    // use cross product of edges to get normal
    return cross(verts->at(v2) - verts->at(v1), verts->at(v3) - verts->at(v1))
        .normalized();
  } else {
    vec3 p1 = verts->at(v2) - verts->at(v1);
    vec3 p2 = verts->at(v3) - verts->at(v1);
    vec3 p3 = point - verts->at(v1);

    double d00 = dot(p1, p1);
    double d01 = dot(p1, p2);
    double d11 = dot(p2, p2);
    double d20 = dot(p3, p1);
    double d21 = dot(p3, p2);
    double denom = d00 * d11 - d01 * d01;

    double y = (d11 * d20 - d01 * d21) / denom;
    double z = (d00 * d21 - d01 * d20) / denom;
    double x = 1.0f - y - z;

    if (debug) {
      std::cout << "[Triangle::normal] barycentric coords: " << x << " " << y
                << " " << z << std::endl;
      std::cout << "[Triangle::normal] normals: " << norms->at(n1) << " "
                << norms->at(n2) << " " << norms->at(n3) << std::endl;
    }

    // use barycentric weights to interpolate given normals
    vec3 res = norms->at(n1) * x + norms->at(n2) * y + norms->at(n3) * z;

    if (debug) {
      std::cout << "[Triangle::normal] interpolated normal: " << res
                << std::endl;
    }

    return res.normalized();
  }
}

vec3 Triangle::center() const {
  return (verts->at(v1) + verts->at(v2) + verts->at(v3)) / 3;
}

Bounding_Box Triangle::get_bounding_box() const {
  Bounding_Box bb;
  bb.include(verts->at(v1));
  bb.include(verts->at(v2));
  bb.include(verts->at(v3));
  return bb;
}
