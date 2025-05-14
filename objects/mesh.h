#pragma once

#include "objects/object.h"
#include "bounding_box.h"
#include "bvh.h"
#include "objects/triangle.h"
#include <vector>
#include <sstream>
#include <fstream>
#include <climits>

/* Mesh, from an obj file */
class Mesh : public Object {
    std::vector<vec3> vertices;
    std::vector<vec3> normals;
    std::vector<Object*> tris;
    BVH bvh;

    // transformation matrices
    vec<double, 16> mat;
    vec<double, 16> inv_mat;
    
    void parse_obj(const char* filename);
    vec<std::string, 3> split(const std::string& s, char delim);
    public:
    Mesh(const char* filename, const vec3& pos, double rX, double rY, double rZ);
    ~Mesh() {
        for (size_t i = 0; i < tris.size(); i++) {
            delete tris[i];
        }
    }
    
    virtual Hit intersection(const Ray& ray) const override;
    virtual vec3 normal(const vec3& point, size_t face_idx) const override;
    virtual vec3 center() const override;
    virtual Bounding_Box get_bounding_box() const override;
};
