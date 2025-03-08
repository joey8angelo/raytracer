#include "light.h"
#include "phong_shader.h"
#include "ray.h"
#include "world.h"
#include "object.h"

vec3 Phong_Shader::shade(const Ray& ray, const vec3& point, 
	const vec3& normal, int recursion_depth) const {
	vec3 color = ambient_color * world.ambient_color 
	             * world.ambient_intensity;

	for(size_t i = 0; i < world.lights.size(); i++) {
		vec3 lr = world.lights[i]->position - point;
		
		Ray shadow;
		shadow.origin = point;
		shadow.dir = lr.normalized();

		Hit hit = world.closest_intersection(shadow);
		if (hit.object && hit.dist < lr.magnitude())
			continue;

		double diff_intensity = std::max(dot(
			lr.normalized(), normal.normalized()), 0.0);
		
		vec3 light_color = world.lights[i]->emitted_light(lr);
			
		color += diff_intensity * diffuse_color * light_color;

		vec3 reflection = ((2 * dot(lr, normal) * normal) - lr).normalized();
		double spec_intensity = pow(std::max(
			dot(reflection, (ray.dir * -1)), 0.0), 
			specular_power);

		color += spec_intensity * specular_color * light_color;
	}
	
	return color;
}
