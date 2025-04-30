#pragma once

#include "objects/object.h"
#include "bounding_box.h"
#include "bvh.h"
#include <vector>
#include <sstream>
#include <fstream>
#include <climits>

class Triangle : public Object {
    // references to vertices and normals of Mesh
    vec3* v1;
    vec3* v2;
    vec3* v3;
    vec3* n1;
    vec3* n2;
    vec3* n3;
    size_t idx;

    public:
    Triangle() {}
    Triangle(vec3* v1, vec3* v2, vec3* v3, vec3* n1, vec3* n2, vec3* n3, size_t idx) 
        : v1(v1), v2(v2), v3(v3), n1(n1), n2(n2), n3(n3), idx(idx) {}
            
    virtual Hit intersection(const Ray& ray) const override;
    virtual vec3 normal(const vec3& point, size_t face_idx) const override;
    virtual vec3 center() const override;
    virtual Bounding_Box get_bounding_box() const override;
};

/* Mesh, from an obj file */
class Mesh : public Object {
    vec3* vertices;
    size_t nv;
    vec3* normals;
    size_t nn;
    Triangle* tris;
    size_t nt;
    BVH<Triangle> bvh;

    // transformation matrices, in row major order
    vec<double, 16> mat;
    vec<double, 16> inv_mat;
    
    Bounding_Box bb;
    void parse_obj(const char* filename);
    vec<std::string, 3> split(const std::string& s, char delim);
    public:
    Mesh(const char* filename, const vec3& pos, double rX, double rY, double rZ);
    ~Mesh() {
        delete[] vertices;
        delete[] normals;
        delete[] tris;
    }
    
    virtual Hit intersection(const Ray& ray) const override;
    virtual vec3 normal(const vec3& point, size_t face_idx) const override;
    virtual vec3 center() const override;
    virtual Bounding_Box get_bounding_box() const override;
};
