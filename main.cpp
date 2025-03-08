#include <iostream>
#include <string>
#include <unistd.h>
#include <ncurses.h>
#include "world.h"

#include "shaders/phong_shader.h"
#include "shaders/flat_shader.h"
#include "objects/sphere.h"
#include "lights/point_light.h"
#include "objects/plane.h"
#include "objects/parallelogram.h"

void dump_png(unsigned int*, int, int, const char*);
void image_to_screen(unsigned int*, WINDOW* scr, int, int);
void move_camera(World& world, int key);
int keyboard(World& world);
void build_world(World& world, int width, int height);
void ncurses_init();
void set_colors();

bool debug = false;

int best_color(const ivec3& color);

const double PIXEL_AR = 1/2.35;

int main(int argc, char** argv) {
	ncurses_init();

	int width, height;
	getmaxyx(stdscr, height, width);
	//std::cout << "w " << width << " h " << height << std::endl;
    //std::cout << "col" << best_color({81, 22, 184}) << std::endl;

	World world;
	build_world(world, width, height);
	
	// render the first frame to a png
	// taking into account the aspect ratio
	// of a terminal character
	world.camera.set_resolution(ivec2(width*10, height*(1/PIXEL_AR)*10));
	world.render();

    // cast a debug ray
    int debugx = -1;
    int debugy = -1;
    if (argc == 3) {
        debugx = std::stoi(argv[1]);
        debugy = std::stoi(argv[2]);
        debug = true;
    }
    if(debug) {
        endwin();
        world.render_pixel({debugx, debugy});
        world.camera.set_pixel({debugx, debugy}, {1,1,1});
    }

	dump_png(world.camera.image, width*10, height*(1/PIXEL_AR)*10, "image.png");	

    if(debug)
        return 0;
    // keep rendering the world to the screen
	world.camera.set_resolution(ivec2(width, height));
	while (true) {
		keyboard(world);

		world.render();
		image_to_screen(world.camera.image, stdscr, width, height);

		refresh();	
		usleep(100);
	}
	
	endwin();
	return 0;
}

void ncurses_init() {
	initscr();
    start_color();
    set_colors();
	noecho();
	nodelay(stdscr, TRUE);
	curs_set(0);
}

void build_world(World& world, int width, int height) {
	world.camera.focus(1, (double(width)/height) * PIXEL_AR, 70*(pi/180));
	world.camera.set_pos_and_aim({10,0,0}, {0,0,0}, {0,0,1});
	
	Sphere* sphere = new Sphere({0,0,0}, 3);
	sphere->shader = new Phong_Shader(world,{1,1,1},{1,1,1},{1,1,1},50);
	world.objects.push_back(sphere);

    Plane* plane = new Plane({0,0,-10}, {0,0,1});
    plane->shader = new Phong_Shader(world, {1,1,1},{1,1,1},{1,1,1},50);
    world.objects.push_back(plane);

    Parallelogram* para = new Parallelogram({0,4,4},{0,0,5},{-4,-4,-3});
    para->shader = new Phong_Shader(world, {1,1,1}, {1,1,1}, {1,1,1}, 50);
    world.objects.push_back(para);
	
	world.lights.push_back(new Point_Light({7,5,4}, {1,0,0}, 800));
	world.lights.push_back(new Point_Light({5,-9,4}, {0,0,1}, 800));
    world.lights.push_back(new Point_Light({-4,-5,2}, {0,1,0}, 800));

	world.ambient_intensity = 0;
	world.ambient_color = vec3(1,1,1);
    world.background_shader = new Flat_Shader(world, {0,0,0});
}

int keyboard(World& world) {
	int ch = getch();

	if (ch == ERR)
		return 0;
        
	if (ch == 'w' || ch == 'a' || ch == 's' || ch == 'd')
		move_camera(world, ch);

	return 1;
}

void move_camera(World& world, int key) {
	switch(key){
		case 'w':
			world.camera.position += world.camera.vertical*0.1;
			break;
		case 'a':
			world.camera.position -= world.camera.horizontal*0.1;
			break;
		case 's':
			world.camera.position -= world.camera.vertical*0.1;
			break;
		case 'd':
			world.camera.position += world.camera.horizontal*0.1;
			break;
	};
	world.camera.aim({0,0,0});
}
