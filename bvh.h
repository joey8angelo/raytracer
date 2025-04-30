#pragma once

#include "objects/object.h"
#include "bounding_box.h"

struct BVHNode {
    // if count == 0, left_start is the index of the left child, 
    // else it is the start index of the first primitive
    // right child is left+1
    size_t left_start, count;
    Bounding_Box bb; // bounding box of the node

    BVHNode() : left_start(0), count(0), bb() {}
};

// offline bvh
template<typename T>
class BVH {
    public:
    const Object* objects;
    size_t n;
    size_t used_nodes;
    size_t* idxs;
    BVHNode* nodes;

    BVH() : objects(nullptr), n(0), used_nodes(0), idxs(nullptr), nodes(nullptr) {}

    ~BVH() {
        delete[] idxs;
        delete[] nodes;
    }

    void build(const T* objects, size_t n) {
        idxs = new size_t[n];
        for (size_t i = 0; i < n; i++) {
            idxs[i] = i;
        }
        nodes = new BVHNode[2*n-1];
        this->objects = objects;

        update_node_bb(0);
    }

    void update_node_bb(size_t node_idx) {
        BVHNode& node = nodes[node_idx];
        for(size_t start = node.left_start, i = 0; i < node.count; i++) {
            const Object& ob = objects[idxs[start + i]];
            node.bb.include(ob.get_bounding_box());
        }
        if (node_idx == 0) {
            std::cout << "[BVH::update_node_bb] root node bb: " << node.bb.bounds[0] << " " 
                        << node.bb.bounds[1] << std::endl;
        }
    }

    void divide(size_t node_idx) {
        BVHNode& node = nodes[node_idx];
        if (node.count <= 2) return;

        // find longest axis
        vec3 diag = node.bb.bounds[1] - node.bb.bounds[0];
        int axis = 0;
        if (diag[1] > diag[axis]) axis = 1;
        if (diag[2] > diag[axis]) axis = 2;

        double mid = (node.bb.bounds[0][axis] + node.bb.bounds[1][axis]) * 0.5;

        // partition
        size_t i = node.left_start;
        int j = i + node.count - 1;
        while (i <= j) {
            if (objects[idxs[i]].center()[axis] < mid) i++;
            else std::swap(idxs[i], idxs[j--]);
        }
        size_t left_count = i = node.left_start;
        if (left_count == 0 || left_count == node.count) return;
        size_t left_idx = used_nodes++;
        size_t right_idx = used_nodes++;
        nodes[left_idx].left_start = node.left_start;
        nodes[left_idx].count = left_count;
        nodes[right_idx].left_start = i;
        nodes[right_idx].count = node.count - left_count;
        node.left_start = left_idx;
        node.count = 0;
        update_node_bb(left_idx);
        update_node_bb(right_idx);
        divide(left_idx);
        divide(right_idx);
    }

    Hit intersection(const Ray& ray, const size_t node_idx = 0) const {
        if (debug) {
            std::cout << "[BVH::intersection] ray: " << ray.origin << " " 
                      << ray.dir << std::endl;
        }
        BVHNode& node = nodes[node_idx];
        if (!node.bb.intersection(ray)) {
            if (debug) {
                std::cout << "[BVH::intersection] ray does not intersect bounding box" 
                          << std::endl;
            }
            return {NULL, 0, 0};
        }
        if (node.count == 0) {
            if (debug) {
                std::cout << "[BVH::intersection] node is not a leaf" << std::endl;
            }
            Hit lh = intersection(ray, node.left_start);
            Hit rh = intersection(ray, node.left_start + 1);
            if (lh.object && rh.object) {
                if (lh.dist < rh.dist) return lh;
                else return rh;
            } else if (lh.object) {
                return lh;
            } else if (rh.object) {
                return rh;
            } else {
                return {NULL, 0, 0};
            }
        } else {
            if (debug) {
                std::cout << "[BVH::intersection] node is a leaf" << std::endl;
            }
            Hit best_hit = {NULL, std::numeric_limits<double>::max(), 0};
            for (size_t i = 0; i < node.count; i++) {
                Hit hit = objects[idxs[node.left_start + i]].intersection(ray);
                if (hit.object && hit.dist < best_hit.dist) {
                    best_hit = hit;
                }
            }
            if (debug) {
                std::cout << "[BVH::intersection] best hit: " << best_hit.dist << " " 
                          << best_hit.face << std::endl;
            }
            return best_hit;
        }
    }
};