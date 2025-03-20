#include "camera.h"

Camera::Camera() : image(0) {}

Camera::~Camera() {
	delete[] image;
}

void Camera::set_pos_and_aim(const vec3& pos, const vec3& look_at,
                             const vec3& sdup) {
	position = pos;
	aim(look_at, sdup);
}

void Camera::aim(const vec3& look_at) {
	aim(look_at, vertical);
}
void Camera::aim(const vec3& look_at, const vec3& sdup) {
	look = (look_at - position).normalized();
	horizontal = cross(look, sdup).normalized();
	vertical = cross(horizontal, look).normalized();
}

void Camera::focus(double focal_dist, double aspect_ratio, double fov) {
	this->focal_dist = focal_dist;
    	double width = 2.0 * focal_dist * tan(0.5 * fov);
    	double height = width / aspect_ratio;
    	image_size = vec2(width, height);
}

void Camera::set_resolution(const ivec2& number_pixels_in) {
	number_pixels = number_pixels_in;
	if (image)
		delete[] image;
	image = new unsigned int[number_pixels[0] * number_pixels[1]];
	min = -0.5 * image_size;
	max = 0.5 * image_size;
	pixel_size = image_size / vec2(number_pixels);
}
vec3 Camera::world_position(const ivec2& index, const vec2& offsets) {
    vec2 p = cell_pos(index, offsets);	
    return position + look * focal_dist + // position of the image window
               p[0] * horizontal +        // horizontal component
               p[1] * vertical;           // vertical component
}

vec2 Camera::cell_pos(const ivec2& index, const vec2& offsets) const {
	return min+(vec2(index)+offsets)*pixel_size;
}

unsigned int Camera::pixel_color(const vec3& color) {
        unsigned int r = std::min(color[0], 1.0)*255;
        unsigned int g = std::min(color[1], 1.0)*255;
        unsigned int b = std::min(color[2], 1.0)*255;
        return (r<<24)|(g<<16)|(b<<8)|0xff;
}

void Camera::set_pixel(const ivec2& index, const vec3& color) {
	int i=index[0];
        int j=index[1];
        image[j*number_pixels[0]+i]=pixel_color(color);
}
