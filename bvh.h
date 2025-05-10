#pragma once

#include "objects/object.h"
#include "bounding_box.h"
#include <vector>

struct BVHNode {
    // if count == 0, left_start is the index of the left child, 
    // else it is the start index of the first primitive
    // right child is left+1
    size_t left_start, count;
    Bounding_Box bb; // bounding box of the node

    BVHNode() : left_start(0), count(0), bb() {}
};

// offline bvh
class BVH {
    public:
    const std::vector<Object*>* objects;
    size_t n;
    size_t used_nodes;
    std::vector<size_t> idxs;
    std::vector<BVHNode> nodes;

    BVH();
    BVH(const std::vector<Object*>* objects);

    void build(const std::vector<Object*>* objects);

    Hit intersection(const Ray& ray, size_t node_idx = 0) const;
    private:
    void update_node_bb(size_t node_idx);
    void divide(size_t node_idx);
    void clear();
};