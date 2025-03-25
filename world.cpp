#include "world.h"
#include "ray.h"
#include "objects/object.h"
#include "shaders/shader.h"
#include "lights/light.h"
#include "util.h"

World::~World() {
	// delete background shader
    if (background_shader)
        delete background_shader;
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
	vec3 color;
	Ray ray;
	ray.origin = camera.position;
	for (int i = 0; i < samples; i++) {
		vec2 off;
		if (i != 0) {
			off = {xorshift_0_1(index[0], index[1], i)-0.5, 
			       xorshift_0_1(index[1], index[0], i)-0.5};
		}
		ray.set_dir((camera.world_position(index, off)-ray.origin).normalized());
		color += cast_ray(ray, recursion_depth_limit);
	}
	color /= samples;
	camera.set_pixel(index, color);
}

vec3 World::cast_ray(const Ray& ray, int recursion_depth) {
	Hit hit = closest_intersection(ray);
	if (hit.object){
		vec3 intersection = ray.at(hit.dist);
		return hit.object->shader->shade(
                	ray, intersection, hit.object->normal(
				intersection), recursion_depth);
	}
	else
		return background_shader->shade(ray, {0,0,0}, 
                           {0,0,0}, recursion_depth);
}
