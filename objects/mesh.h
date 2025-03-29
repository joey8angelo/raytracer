#pragma once

#include "objects/object.h"
#include "bounding_box.h"
#include <vector>
#include <sstream>
#include <fstream>
#include <climits>

/* Mesh, from an obj file */
class Mesh : public Object {
    std::vector<vec3> vertices;
    std::vector<vec3> normals;
    std::vector<vec<ivec3, 2>> faces;

    // transformation matrices, in row major order
    vec<double, 16> mat;
    vec<double, 16> inv_mat;
    
    Bounding_Box bb;
    Hit triangle_intersection(size_t face_idx, const Ray& ray) const;
    void parse_obj(const char* filename);
    vec<std::string, 3> split(const std::string& s, char delim);
    public:
    Mesh(const char* filename, const vec3& pos, double rX, double rY);
    
    virtual Hit intersection(const Ray& ray) const override;
    virtual vec3 normal(const vec3& point, size_t face_idx) const override;
    virtual void move_dir(const vec3& vec)  override;
    virtual void move_to(const vec3& vec)  override;
};
