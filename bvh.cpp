#include "bvh.h"

BVH::BVH() : objects(NULL), n(0), used_nodes(0) {}

BVH::BVH(const std::vector<Object*>* objects) : objects(objects), n(objects->size()), used_nodes(1) {
    idxs.resize(n);
    for (size_t i = 0; i < n; i++) {
        idxs[i] = i;
    }
    nodes.resize(2*n-1);
    update_node_bb(0);
    divide(0);
}

void BVH::build(const std::vector<Object*>* objects) {
    this->objects = objects;
    this->n = objects->size();
    used_nodes = 1;
    idxs.resize(n);
    for (size_t i = 0; i < n; i++) {
        idxs[i] = i;
    }
    nodes.resize(2*n-1);
    nodes[0].left_start = 0;
    nodes[0].count = n;
    std::cout << "[BVH::build] n: " << n << std::endl;
    update_node_bb(0);
    divide(0);
}

void BVH::update_node_bb(size_t node_idx) {
    BVHNode& node = nodes[node_idx];
    for(size_t start = node.left_start, i = 0; i < node.count; i++) {
        const Object* ob = objects->at(idxs[start + i]);
        node.bb.include(ob->get_bounding_box());
    }
}

void BVH::divide(size_t node_idx) {
    std::cout << "[BVH::divide] node: " << node_idx << std::endl;
    BVHNode& node = nodes[node_idx];
    std::cout << "[BVH::divide] node count: " << node.count << " left_start: " << node.left_start << std::endl;
    if (node.count <= 2) return;

    std::cout << "[BVH::divide] node count: " << node.count << std::endl;

    // find longest axis
    vec3 diag = node.bb.bounds[1] - node.bb.bounds[0];
    int axis = 0;
    if (diag[1] > diag[axis]) axis = 1;
    if (diag[2] > diag[axis]) axis = 2;

    std::cout << "[BVH::divide] axis: " << axis << std::endl;
    std::cout << "[BVH::divide] bounds: " << node.bb.bounds[0][axis] << " " 
              << node.bb.bounds[1][axis] << std::endl;

    double part = (node.bb.bounds[0][axis] + node.bb.bounds[1][axis]) * 0.5;

    std::cout << "[BVH::divide] part: " << part << std::endl;

    // partition
    size_t i = node.left_start;
    size_t j = i + node.count - 1;
    while (i <= j) {
        if (objects->at(idxs[i])->center()[axis] < part) i++;
        else std::swap(idxs[i], idxs[j--]);
    }
    size_t left_count = i - node.left_start;
    std::cout << "[BVH::divide] left count: " << left_count << std::endl;
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

Hit BVH::intersection(const Ray& ray, size_t node_idx) const {
    if (debug) {
        std::cout << "[BVH::intersection] ray: " << ray.origin << " " 
                  << ray.dir << std::endl;
    }
    const BVHNode& node = nodes[node_idx];
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
            Hit hit = objects->at(idxs[node.left_start + i])->intersection(ray);
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