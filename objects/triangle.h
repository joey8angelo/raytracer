#include "objects/object.h"
#include <vector>

class Triangle : public Object {
  const std::vector<vec3> *verts;
  const std::vector<vec3> *norms;
  size_t v1, v2, v3;
  size_t n1, n2, n3;
  size_t idx;

public:
  Triangle() {}
  Triangle(const std::vector<vec3> *v, const std::vector<vec3> *n, size_t v1,
           size_t v2, size_t v3, size_t n1, size_t n2, size_t n3, size_t idx)
      : verts(v), norms(n), v1(v1), v2(v2), v3(v3), n1(n1), n2(n2), n3(n3),
        idx(idx) {}

  Hit intersection(const Ray &ray) const override;
  vec3 normal(const vec3 &point, size_t face_idx) const override;
  vec3 center() const override;
  Bounding_Box get_bounding_box() const override;
};
