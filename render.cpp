#include <iostream>
#include <ncurses.h>
#include <chrono>
#include "world.h"
#include "ray.h"

void image_to_window(unsigned int* data, bool* block, WINDOW* win, int width, int height);
void dump_png(unsigned int* data, int width, int height, const char* filename);
int keyboard(World& world);
void move_camera(World& world, int key);
void rotate_camera(World& world, vec3 point, int key);
void rot(vec3& p, const vec3& c, const vec3& u, double theta);
WINDOW* create_window(int wwidth, int wheight, int x, int y, int width, int height);

class Ray;

bool* block;

auto prev_time = std::chrono::system_clock::now();

// render loop for the world
void render(World& world, int width, int height, double pixel_ar, const char* output, bool dump) {
    block = new bool[width*height];
	for (int i = 0; i < width*height; i++) {
		block[i] = false;
	}
	
	WINDOW* cam_win = create_window(20, 3, 0, 0, width, height);
	mvwprintw(cam_win,0,1,"Camera Position ");
	WINDOW* cam_look_win = create_window(20, 3, 0, 3, width, height);
	mvwprintw(cam_look_win,0,1,"Camera Look Dir ");
	WINDOW* fps_win = create_window(6, 3, 0, 6, width, height);
	mvwprintw(fps_win,0,1,"FPS ");

	prev_time = std::chrono::system_clock::now();
    
    size_t frame = 0;
	size_t dcount = 0;

    while (true) {
		// handle keyboard inputs
        int k = keyboard(world);
		if (k == -1)
			break;

        // if we want to dump a sequence of images to every 10th frame
        if (dump && dcount%10==0) {
            int c = 1;
            world.camera.set_resolution(ivec2(width*c, height*c*(1/pixel_ar)));
            world.render();
            std::string nm = std::to_string(frame/10);
            std::string zs = std::string(4-nm.size(), '0');
            std::string fn = std::string(output) + "_" + zs + nm + ".png";
            dump_png(world.camera.image, width*c, height*c*(1/pixel_ar), 
                    fn.c_str());
            world.camera.set_resolution(ivec2(width, height));
			dcount = 0;
        }

        // render the world
		world.render();
        // render the camera image to stdscr
		image_to_window(world.camera.image, block, stdscr, width, height);

        // render info to camera windows
        mvwprintw(cam_win,1,1,"%.2f %.2f %.2f", world.camera.position[0], 
                world.camera.position[1], world.camera.position[2]);
		mvwprintw(cam_look_win,1,1,"%.2f %.2f %.2f", world.camera.look[0],
				world.camera.look[1], world.camera.look[2]);

		// render fps to fps window
		auto cur_time = std::chrono::system_clock::now();
		if (cur_time - prev_time >= std::chrono::seconds(1)) {
			mvwprintw(fps_win,1,1,"%-*d", 4, frame);
			prev_time = cur_time;
			frame = 0;
		}

		
        // refresh windows
        refresh();
        wrefresh(cam_win);
		wrefresh(cam_look_win);
		wrefresh(fps_win);

	    frame++;
		dcount++;
    }

	delete[] block;
	delwin(cam_win);
	delwin(cam_look_win);
	delwin(fps_win);
}

int keyboard(World& world) {
	int ch = getch();
	static bool rotate = false;
	static vec3 pos = {0,0,0};

	Hit hit;
	Ray ray;

	switch(ch) {
		case 'w':
		case 'a':
		case 's':
		case 'd':
		case KEY_UP:
		case KEY_DOWN:
		case KEY_LEFT:
		case KEY_RIGHT:
			if (rotate) 
				rotate_camera(world, pos, ch);
			else
				move_camera(world, ch);
			break;
		case 'r':
			rotate = !rotate;
			if (rotate) {
				ray = {world.camera.position, world.camera.look};
				hit = world.closest_intersection(ray);
				if (hit.object)
					pos = hit.object->center();
				else
					pos = world.camera.position + world.camera.look;
			}
			break;
		case 'q':
			return -1;
		default:
			return 0;
	}
	return 0;
}

void move_camera(World& world, int key) {
    const double speed = 0.1;
    const double slow = 0.05;
    vec3 p;
	switch(key){
		case 'w':
			world.camera.position += world.camera.look*speed;
			break;
		case 'a':
			world.camera.position -= world.camera.horizontal*speed;
			break;
		case 's':
			world.camera.position -= world.camera.look*speed;
			break;
		case 'd':
			world.camera.position += world.camera.horizontal*speed;
			break;
        case KEY_UP:
            p = world.camera.position;
            p += world.camera.look+world.camera.vertical*slow;
            world.camera.aim(p);
            break;
        case KEY_DOWN:
            p = world.camera.position;
            p += world.camera.look-world.camera.vertical*slow;
            world.camera.aim(p);
            break;
        case KEY_LEFT:
            p = world.camera.position;
            p += world.camera.look-world.camera.horizontal*slow;
            world.camera.aim(p);
            break;
        case KEY_RIGHT:
            p = world.camera.position;
            p += world.camera.look+world.camera.horizontal*slow;
            world.camera.aim(p);
            break;
    }
}


// rotate camera around a point
void rotate_camera(World& world, vec3 point, int key) {
	const double speed = 0.05;
	const vec3 h = world.camera.horizontal;
	const vec3 v = world.camera.vertical;
	switch(key){
		case KEY_UP:
			rot(world.camera.position, point, h, speed);
			break;
		case KEY_LEFT:
			rot(world.camera.position, point, v, speed);
			break;
		case KEY_DOWN:
			rot(world.camera.position, point, h, -speed);
			break;
		case KEY_RIGHT:
			rot(world.camera.position, point, v, -speed);
			break;
		default:
			return;
	}
	world.camera.aim(point);
}

void rot(vec3& p, const vec3& c, const vec3& u, double theta) {
	vec3 r = p-c;
	double cos_theta = cos(theta);
	double sin_theta = sin(theta);
	p = c + r*cos_theta + cross(u,r)*sin_theta + u*(dot(u, r)*(1-cos_theta));
}

WINDOW* create_window(int wwidth, int wheight, int x, int y, int width, int height) {
    WINDOW* win = newwin(wheight, wwidth, y, x);
    box(win, 0, 0);

	for (int i = 0; i < wwidth; i++) {
    	for (int j = 0; j < wheight; j++) {
        	int idx = (height-j-y)*width +i-width+x;
        	if (idx >= 0 && idx < width * height) {
            	block[idx] = true;
        	}
    	}
	}


    return win;
}