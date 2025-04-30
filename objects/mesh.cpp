#include "objects/mesh.h"

inline void make_rotation_matrix(vec<double, 16>& mat, const vec3& p, double rX, double rY, double rZ) {
    double c1 = cos(rX);
    double s1 = sin(rX);
    double c2 = cos(rY);
    double s2 = sin(rY);
    double c3 = cos(rZ);
    double s3 = sin(rZ);
    mat[0]=c1*c2; mat[1]=s1*s2*c3-c1*s3; mat[2]=c1*s2*c3+s1*s3; mat[3]=p[0];
    mat[4]=c2*s3; mat[5]=s1*s2*s3+c1*c3; mat[6]=c1*s1*s3-s1*c3; mat[7]=p[1];
    mat[8]=-s2;   mat[9]=s1*c2;          mat[10]=c1*c2;         mat[11]=p[2];
    mat[12]=0;    mat[13]=0;             mat[14]=0;             mat[15]=1;
}

inline void invert_matrix(const vec<double, 16>& m, vec<double, 16>& inv) {
    double det;

    /* Compute adjoint: */
    inv[0] =
        + m[ 5] * m[10] * m[15]
        - m[ 5] * m[11] * m[14]
        - m[ 9] * m[ 6] * m[15]
        + m[ 9] * m[ 7] * m[14]
        + m[13] * m[ 6] * m[11]
        - m[13] * m[ 7] * m[10];

    inv[1] =
        - m[ 1] * m[10] * m[15]
        + m[ 1] * m[11] * m[14]
        + m[ 9] * m[ 2] * m[15]
        - m[ 9] * m[ 3] * m[14]
        - m[13] * m[ 2] * m[11]
        + m[13] * m[ 3] * m[10];

    inv[2] =
        + m[ 1] * m[ 6] * m[15]
        - m[ 1] * m[ 7] * m[14]
        - m[ 5] * m[ 2] * m[15]
        + m[ 5] * m[ 3] * m[14]
        + m[13] * m[ 2] * m[ 7]
        - m[13] * m[ 3] * m[ 6];

    inv[3] =
        - m[ 1] * m[ 6] * m[11]
        + m[ 1] * m[ 7] * m[10]
        + m[ 5] * m[ 2] * m[11]
        - m[ 5] * m[ 3] * m[10]
        - m[ 9] * m[ 2] * m[ 7]
        + m[ 9] * m[ 3] * m[ 6];

    inv[4] =
        - m[ 4] * m[10] * m[15]
        + m[ 4] * m[11] * m[14]
        + m[ 8] * m[ 6] * m[15]
        - m[ 8] * m[ 7] * m[14]
        - m[12] * m[ 6] * m[11]
        + m[12] * m[ 7] * m[10];

    inv[5] =
        + m[ 0] * m[10] * m[15]
        - m[ 0] * m[11] * m[14]
        - m[ 8] * m[ 2] * m[15]
        + m[ 8] * m[ 3] * m[14]
        + m[12] * m[ 2] * m[11]
        - m[12] * m[ 3] * m[10];

    inv[6] =
        - m[ 0] * m[ 6] * m[15]
        + m[ 0] * m[ 7] * m[14]
        + m[ 4] * m[ 2] * m[15]
        - m[ 4] * m[ 3] * m[14]
        - m[12] * m[ 2] * m[ 7]
        + m[12] * m[ 3] * m[ 6];

    inv[7] =
        + m[ 0] * m[ 6] * m[11]
        - m[ 0] * m[ 7] * m[10]
        - m[ 4] * m[ 2] * m[11]
        + m[ 4] * m[ 3] * m[10]
        + m[ 8] * m[ 2] * m[ 7]
        - m[ 8] * m[ 3] * m[ 6];

    inv[8] =
        + m[ 4] * m[ 9] * m[15]
        - m[ 4] * m[11] * m[13]
        - m[ 8] * m[ 5] * m[15]
        + m[ 8] * m[ 7] * m[13]
        + m[12] * m[ 5] * m[11]
        - m[12] * m[ 7] * m[ 9];

    inv[9] =
        - m[ 0] * m[ 9] * m[15]
        + m[ 0] * m[11] * m[13]
        + m[ 8] * m[ 1] * m[15]
        - m[ 8] * m[ 3] * m[13]
        - m[12] * m[ 1] * m[11]
        + m[12] * m[ 3] * m[ 9];

    inv[10] =
        + m[ 0] * m[ 5] * m[15]
        - m[ 0] * m[ 7] * m[13]
        - m[ 4] * m[ 1] * m[15]
        + m[ 4] * m[ 3] * m[13]
        + m[12] * m[ 1] * m[ 7]
        - m[12] * m[ 3] * m[ 5];

    inv[11] =
        - m[ 0] * m[ 5] * m[11]
        + m[ 0] * m[ 7] * m[ 9]
        + m[ 4] * m[ 1] * m[11]
        - m[ 4] * m[ 3] * m[ 9]
        - m[ 8] * m[ 1] * m[ 7]
        + m[ 8] * m[ 3] * m[ 5];

    inv[12] =
        - m[ 4] * m[ 9] * m[14]
        + m[ 4] * m[10] * m[13]
        + m[ 8] * m[ 5] * m[14]
        - m[ 8] * m[ 6] * m[13]
        - m[12] * m[ 5] * m[10]
        + m[12] * m[ 6] * m[ 9];

    inv[13] =
        + m[ 0] * m[ 9] * m[14]
        - m[ 0] * m[10] * m[13]
        - m[ 8] * m[ 1] * m[14]
        + m[ 8] * m[ 2] * m[13]
        + m[12] * m[ 1] * m[10]
        - m[12] * m[ 2] * m[ 9];

    inv[14] =
        - m[ 0] * m[ 5] * m[14]
        + m[ 0] * m[ 6] * m[13]
        + m[ 4] * m[ 1] * m[14]
        - m[ 4] * m[ 2] * m[13]
        - m[12] * m[ 1] * m[ 6]
        + m[12] * m[ 2] * m[ 5];

    inv[15] =
        + m[ 0] * m[ 5] * m[10]
        - m[ 0] * m[ 6] * m[ 9]
        - m[ 4] * m[ 1] * m[10]
        + m[ 4] * m[ 2] * m[ 9]
        + m[ 8] * m[ 1] * m[ 6]
        - m[ 8] * m[ 2] * m[ 5];

    /* Compute determinant: */
    det = +m[0]*inv[0]+m[1]*inv[4]+m[2]*inv[8]+m[3]*inv[12];

    /* Multiply adjoint with reciprocal of determinant: */
    det = 1.0f / det;

    inv[ 0]*=det; inv[ 1]*=det; inv[ 2]*=det; inv[ 3]*=det; 
    inv[ 4]*=det; inv[ 5]*=det; inv[ 6]*=det; inv[ 7]*=det; 
    inv[ 8]*=det; inv[ 9]*=det; inv[10]*=det; inv[11]*=det; 
    inv[12]*=det; inv[13]*=det; inv[14]*=det; inv[15]*=det;
}

// transform a given vector by a transformation matrix
inline vec3 transform_vec(const vec3& v, const vec<double, 16>& mat) {
    return {v[0]*mat[0] + v[1]*mat[1] + v[2]*mat[2],
            v[0]*mat[4] + v[1]*mat[5] + v[2]*mat[6],
            v[0]*mat[8] + v[1]*mat[9] + v[2]*mat[10]};
}
// transform a given point by a transformation matrix
inline vec3 transform_point(const vec3& p, const vec<double, 16>& mat) {
    return transform_vec(p, mat) + vec3{mat[3],mat[7],mat[11]};
}
// transform a given ray by a transformation matrix
inline Ray transform_ray(const Ray& ray, const vec<double, 16>& mat) {
    vec3 origin = transform_point(ray.origin, mat);
    vec3 dir = transform_vec(ray.dir, mat);
    return {origin, dir};
}

Mesh::Mesh(const char* filename, const vec3& pos, double rX, double rY, double rZ) {
    parse_obj(filename);
    make_rotation_matrix(mat, pos, rX, rY, rZ);
    invert_matrix(mat, inv_mat);
    if (debug) {
        std::cout << "[Mesh::Mesh] mat: " << mat << std::endl;
        std::cout << "[Mesh::Mesh] inv_mat: " << inv_mat << std::endl;
    }
    bvh.build(tris, nt);
}

vec3 Mesh::center() const {
    return {mat[3], mat[7], mat[11]};
}

Hit Triangle::intersection(const Ray& ray) const {
    vec3 edge1 = *v2 - *v1;
    vec3 edge2 = *v3 - *v1;
    vec3 h = cross(ray.dir, edge2);
    double a = dot(edge1, h);
    double f = 1 / a;
    vec3 s = ray.origin - *v1;
    double u = f * dot(s, h);
    if (u < 0 || u > 1) {
        if (debug) {
            std::cout << "[Mesh::triangle_intersection] u: " << u
                      << std::endl;
        }
        return {NULL, 0, 0};
    }
    vec3 q = cross(s, edge1);
    double v = f * dot(ray.dir, q);
    if (v < 0 || u + v > 1) {
        if (debug) {
            std::cout << "[Mesh::triangle_intersection] v: " << v
                      << " u: " << u << std::endl;
        }
        return {NULL, 0, 0};
    }
    double t = f * dot(edge2, q);
    if (t >= small_t) {
        return {this, t, idx};
    } else {
        if (debug) {
            std::cout << "[Mesh::triangle_intersection] t: " << t
                      << std::endl;
        }
        return {NULL, 0, 0};
    }
}

vec3 Triangle::normal(const vec3& point, size_t face_idx) const {
    if (debug) {
        std::cout << "[Triangle::normal] point: " << point << " face: " << idx 
                  << std::endl;
    }

    // obj file gave no normals for vertices
    if ((*n1)[0]==0 && (*n1)[1]==0 && (*n1)[2]==0) {
        // use cross product of edges to get normal
        return cross(*v2 - *v1, *v3 - *v1).normalized();
    } else {
        vec3 p1 = *v2 - *v1;
        vec3 p2 = *v3 - *v1;
        vec3 p3 = point - *v1;

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
            std::cout << "[Mesh::normal] barycentric coords: " << x << " " 
                      << y << " " << z << std::endl;
            std::cout << "[Mesh::normal] normals: " << n1 << " " << n2 << " " 
                      << n3 << std::endl;
        }

        // use barycentric weights to interpolate given normals
        vec3 res = *n1 * x + *n2 * y + *n3 * z;
        
        if (debug) {
            std::cout << "[Mesh::normal] interpolated normal: " << res << std::endl;
        }
        
        return res.normalized();
    } 
}

vec3 Triangle::center() const {
    return (*v1 + *v2 + *v3) / 3;
}

Bounding_Box Triangle::get_bounding_box() const {
    Bounding_Box bb;
    bb.include(*v1);
    bb.include(*v2);
    bb.include(*v3);
    return bb;
}

Hit Mesh::intersection(const Ray& ray) const {
    if (debug) {
        std::cout << "[Mesh::intersection] ray: " << ray.origin << " " 
                  << ray.dir << std::endl;
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

    // -----------------------------------------------------------

    // if (debug) {
    //     std::cout << "[Mesh::intersection] obj space ray: " << r.origin << " " 
    //               << r.dir << std::endl;
    // }
    
    // if (bb.intersection(r)) {
    //     if (debug) {
    //         std::cout << "[Mesh::intersection] ray intersects bounding box" 
    //                   << std::endl;
    //     }
    //     Hit best_hit = {NULL, INT_MAX, 0};
    //     for (size_t i = 0; i < nt; i++) {
    //         Hit hit = tris[i].intersection(r);

    //         if (debug && hit.object) {
    //             std::cout << "[Mesh::intersection] hit: " << hit.dist << " " 
    //                       << i << std::endl;
    //         }

    //         if (hit.object && hit.dist < best_hit.dist) {
    //             best_hit = hit;
    //         }
    //     }
    //     if (best_hit.object) {
    //         if (debug) {
    //             std::cout << "[Mesh::intersection] best hit: " << best_hit.dist 
    //                       << " " << best_hit.face << std::endl;
    //         }
    //         return {this, best_hit.dist, best_hit.face};
    //     }
    // }
    // if (debug) {
    //     std::cout << "[Mesh::intersection] ray does not intersect bounding box" 
    //               << std::endl;
    // }
    // return {NULL, 0, 0};
}

vec3 Mesh::normal(const vec3& point, size_t face_idx) const {
    vec3 p = transform_point(point, inv_mat);
    return transform_vec(tris[face_idx].normal(p, 0), mat).normalized();
} 

void Mesh::parse_obj(const char* filename) {
    bool warn = false;

    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "[Mesh::parse_obj] Could not open file " << filename << std::endl;
        return;
    }

    // count number of vertices, normals and faces
    size_t vs, ns, ts;
    vs = ns = ts = 0;
    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string type;
        ss >> type;
        if (type == "v") {
            vs++;
        } else if (type == "vn") {
            ns++;
        } else if (type == "vt") {
            if (!warn) {
                warn = true;
                std::cerr << "[Mesh::parse_obj] Texture coordinates not supported" << std::endl;
            }
        } else if (type == "f") {
            ts++;
        }
    }
    file.close();
    vertices = new vec3[vs];
    nv = vs;
    normals = new vec3[ns];
    nn = ns;
    tris = new Triangle[ts];
    nt = ts;

    file.open(filename);
    if (!file.is_open()) {
        std::cerr << "[Mesh::parse_obj] Could not open file " << filename << std::endl;
        return;
    }

    // get the actual data
    vs = ns = ts = 0;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string type;
        ss >> type;
        if (type == "v") {
            vec3 v;
            ss >> v;
            vertices[vs++] = v;
            bb.include(v);
        } else if (type == "vn") {
            vec3 n;
            ss >> n;
            normals[ns++] = n;
        } else if (type == "f") {
            ivec3 v;
            ivec3 n;
            for (auto i = 0; i < 3; i++) {
                std::string s;
                ss >> s;
                vec<std::string, 3> t = split(s, '/');
                
                // vertex index
                v[i] = std::stoi(t[0]) - 1;

                // normal index
                if (t[2]!="")
                    n[i] = std::stoi(t[2]) - 1;
            }

            tris[ts] = Triangle(vertices+v[0], vertices+v[1], vertices+v[2],
                normals+n[0], normals+n[1], normals+n[2], ts);
            ts++;
        }
    }

    std::cout << "[Mesh::parse_obj] bounding_box: " << bb.bounds[0] << " " 
              << bb.bounds[1] << std::endl;

    file.close();
}

Bounding_Box Mesh::get_bounding_box() const {
    return bb;
}


vec<std::string, 3> Mesh::split(const std::string& s, char delim) {
    std::stringstream ss(s);
    vec<std::string, 3> v = {"","",""};
    for (auto i = 0; i < 3; i++) {
        if(!std::getline(ss, v[i], delim))
            break;
    }
    return v;
}
