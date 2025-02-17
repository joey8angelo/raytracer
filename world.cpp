#include "world.h"
#include "ray.h"

World::~World() {
	// delete background shader
	// delete objects
	for(size_t i = 0; i < objects.size(); i++)
		delete objects[i];
	// delete lights
	for(size_t i = 0; i < lights.size(); i++)
		delete lights[i];
}

Hit World::closest_intersection(const Ray& ray) {
	Hit closest_hit = {NULL, std::numeric_limits<double>::max()};
	for (auto ob : objects) {
		Hit hit = ob->intersection(ray);
		if (hit.object && closest_hit.dist > hit.dist)
			closest_hit = hit;
	}
	return closest_hit;
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
	(void)recursion_depth; // TEMP
	Hit hit = closest_intersection(ray);
	if (hit.object)
		return vec3(1,1,1); // call objects shader at ray.at(dist)
	else
		return vec3(0,0,0); // call background shader
}
