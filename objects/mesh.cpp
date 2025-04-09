#include "objects/mesh.h"

inline void make_rotation_matrix(vec<double, 16>& mat, const vec3& p, double rX, double rY) {
    double cX = cos(rX);
    double sX = sin(rX);
    double cY = cos(rY);
    double sY = sin(rY);
    mat[0]=cY;     mat[1]=0;  mat[2]=sY;     mat[3]=p[0];
    mat[4]=sX*sY;  mat[5]=cX; mat[6]=-sX*cY; mat[7]=p[1];
    mat[8]=-cX*sY; mat[9]=sX; mat[10]=cX*cY; mat[11]=p[2];
    mat[12]=0;     mat[13]=0; mat[14]=0;     mat[15]=1;
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

Mesh::Mesh(const char* filename, const vec3& pos, double rX, double rY) {
    parse_obj(filename);
    make_rotation_matrix(mat, pos, rX, rY);
    invert_matrix(mat, inv_mat);
    std::cout << "[Mesh::Mesh] mat: " << mat << std::endl;
    std::cout << "[Mesh::Mesh] inv_mat: " << inv_mat << std::endl;
}


Hit Mesh::triangle_intersection(size_t face_idx, const Ray& ray) const {
    const auto& face = faces[face_idx];
    const auto& p1 = vertices[face[0][0]];
    const auto& p2 = vertices[face[0][1]];
    const auto& p3 = vertices[face[0][2]];

    vec3 edge1 = p2 - p1;
    vec3 edge2 = p3 - p1;
    vec3 h = cross(ray.dir, edge2);
    double a = dot(edge1, h);
    double f = 1 / a;
    vec3 s = ray.origin - p1;
    double u = f * dot(s, h);
    if (u < -small_t || u > 1+small_t) {
        if (debug) {
            std::cout << "[Mesh::triangle_intersection] u: " << u
                      << std::endl;
        }
        return {NULL, 0, 0};
    }
    vec3 q = cross(s, edge1);
    double v = f * dot(ray.dir, q);
    if (v < -small_t || u + v > 1+small_t) {
        if (debug) {
            std::cout << "[Mesh::triangle_intersection] v: " << v
                      << " u: " << u << std::endl;
        }
        return {NULL, 0, 0};
    }
    double t = f * dot(edge2, q);
    if (t >= small_t) {
        return {this, t, face_idx};
    } else {
        if (debug) {
            std::cout << "[Mesh::triangle_intersection] t: " << t
                      << std::endl;
        }
        return {NULL, 0, 0};
    }
}

Hit Mesh::intersection(const Ray& ray) const {
    if (debug) {
        std::cout << "[Mesh::intersection] ray: " << ray.origin << " " 
                  << ray.dir << std::endl;
    }
    // transform ray to object space
    const Ray r = transform_ray(ray, inv_mat);

    if (debug) {
        std::cout << "[Mesh::intersection] obj space ray: " << r.origin << " " 
                  << r.dir << std::endl;
    }
    
    if (bb.intersection(r)) {
        if (debug) {
            std::cout << "[Mesh::intersection] ray intersects bounding box" 
                      << std::endl;
        }
        Hit best_hit = {NULL, INT_MAX, 0};
        for (size_t i = 0; i < faces.size(); i++) {
            Hit hit = triangle_intersection(i, r); 

            if (debug && hit.object) {
                std::cout << "[Mesh::intersection] hit: " << hit.dist << " " 
                          << i << std::endl;
            }

            if (hit.object && hit.dist < best_hit.dist) {
                best_hit = hit;
            }
        }
        return best_hit;
    }
    if (debug) {
        std::cout << "[Mesh::intersection] ray does not intersect bounding box" 
                  << std::endl;
    }
    return {NULL, 0, 0};
}

vec3 Mesh::normal(const vec3& point, size_t face_idx) const {
    vec3 p = transform_point(point, inv_mat);

    if (debug) {
        std::cout << "[Mesh::normal] point: " << p << " face: " << face_idx 
                  << std::endl;
    }
    const auto& face = faces[face_idx];
    const auto& p1 = vertices[face[0][0]];
    const auto& p2 = vertices[face[0][1]];
    const auto& p3 = vertices[face[0][2]];

    // obj file gave no normals for vertices
    // transform normal to world space
    if (face[1][0] == -1) {
        // use cross product of edges to get normal
        return transform_vec(cross(p2 - p1, p3 - p1), mat).normalized();
    } else {
        vec3 v1 = p2 - p1;
        vec3 v2 = p3 - p1;
        vec3 v3 = p - p1;

        double d00 = dot(v1, v1);
        double d01 = dot(v1, v2);
        double d11 = dot(v2, v2);
        double d20 = dot(v3, v1);
        double d21 = dot(v3, v2);
        double denom = d00 * d11 - d01 * d01;

        double y = (d11 * d20 - d01 * d21) / denom;
        double z = (d00 * d21 - d01 * d20) / denom;
        double x = 1.0f - y - z;

        vec3 n1 = normals[face[1][0]].normalized();
        vec3 n2 = normals[face[1][1]].normalized();
        vec3 n3 = normals[face[1][2]].normalized();

        if (debug) {
            std::cout << "[Mesh::normal] barycentric coords: " << x << " " 
                      << y << " " << z << std::endl;
            std::cout << "[Mesh::normal] normals: " << n1 << " " << n2 << " " 
                      << n3 << std::endl;
        }

        // use barycentric weights to interpolate given normals
        vec3 res = transform_vec((n1 * x + n2 * y + n3 * z), mat).normalized();
        
        if (debug) {
            std::cout << "[Mesh::normal] interpolated normal: " << res << std::endl;
        }
        
        return res;   
    }
} 

void Mesh::parse_obj(const char* filename) {
    bool warn = false;

    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "[Mesh::parse_obj] Could not open file " << filename << std::endl;
        exit(1);
    }

    std::cout << "parsing " << filename << std::endl;

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string type;
        ss >> type;
        if (type == "v") {
            vec3 v;
            ss >> v;
            vertices.push_back(v);
            bb.include(v);
        } else if (type == "vn") {
            vec3 n;
            ss >> n;
            normals.push_back(n);
        } else if (type == "vt") {
            if (!warn) {
                warn = true;
                std::cerr << "[Mesh::parse_obj] Texture coordinates not supported" << std::endl;
            }
        } else if (type == "f") {
            vec<ivec3, 2> f = {{-1,-1,-1},{-1,-1,-1}};
            for (int i = 0; i < 3; i++) {
                std::string s;
                ss >> s;
                vec<std::string, 3> v = split(s, '/');
                f[0][i] = std::stoi(v[0]) - 1;
                if(v[2]!="")
                    f[1][i] = std::stoi(v[2]) - 1;
                if (v[1].size() > 0 && !warn) {
                    warn = true;
                    std::cerr << "[Mesh::parse_obj] Texture coordinates not supported" << std::endl;
                }
            }
            faces.push_back(f);
        }
    }
}


vec<std::string, 3> Mesh::split(const std::string& s, char delim) {
    std::stringstream ss(s);
    vec<std::string, 3> v = {"","",""};
    for (int i = 0; i < 3; i++) {
        if(!std::getline(ss, v[i], delim))
            break;
    }
    return v;
}

void Mesh::move_dir(const vec3& vec) {
    //mat[3] += vec[0];
    //mat[7] += vec[1];
    //mat[11] += vec[2];
    //invert_matrix(mat, inv_mat);
    make_rotation_matrix(mat, {mat[3], mat[7], mat[11]}, vec[0], vec[1]);
    invert_matrix(mat, inv_mat);
}

void Mesh::move_to(const vec3& vec) {
    mat[3] = vec[0];
    mat[7] = vec[1];
    mat[11] = vec[2];
    invert_matrix(mat, inv_mat);
}
