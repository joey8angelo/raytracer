#include "world.h"
#include "ray.h"

World::World() : ambient_intensity(0), recursion_depth_limit(3) {}

World::~World() {
	// delete background shader
	// delete objects
	// delete lights
}

void World::render() {
	for (int i = 0; i < camera.number_pixels[0]; i++) {
		for (int j = 0; j < camera.number_pixels[1]; j++) {
			render_pixel(ivec2(i,j));
		}
	}
}

void World::render_pixel(const ivec2& index) {
	Ray ray;
	ray.origin = camera.position;
	ray.dir = (camera.world_position(index) - ray.origin).normalized();
	vec3 color = cast_ray(ray, recursion_depth_limit);
	camera.set_pixel(index, color);
}

vec3 World::cast_ray(const Ray& ray, int recursion_depth) {
	// just test if it hits a sphere at 0,0,0
	// and return 1,1,1 if yes 0,0,0 if no
	(void)recursion_depth; //TEMP
	vec3 oc = vec3(0,0,0) - ray.origin;
	auto a = dot(ray.dir, ray.dir);
	auto b = -2.0 * dot(ray.dir, oc);
	auto c = dot(oc, oc) - 0.5*0.5;
	auto discriminant = b*b - 4*a*c;
	return (discriminant >= 0) ? vec3(1,1,1) : vec3(0,0,0);
}
