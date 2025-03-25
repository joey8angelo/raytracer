#include <iostream>
#include <cstring>
#include <unistd.h>
#include <ncurses.h>
#include "world.h"
#include "util.h"

void dump_png(unsigned int*, int, int, const char*);
void image_to_screen(unsigned int*, WINDOW* scr, int, int);
void move_camera(World& world, int key);
int keyboard(World& world);
void ncurses_init();
void set_colors();
void parse_scene(World& world, int width, int height, double ar, const char* fn);
void parse_args(int argc, char** argv, int& width, int& height, int& debugx, int& debugy, char*& scene, char*& output);
int best_color(const ivec3& color);

void usage() {
	std::cerr << "usage: raytracer [-f input_scene] [-d width height debug_x debug_y] [-o output_file]\n";
	exit(1);
}

bool debug = false;
const double PIXEL_AR = 1/2.35;

int main(int argc, char** argv) {
	int width, height, debugx, debugy;
	debugx = debugy = -1;
	char* scene = 0;
	char* output = 0;

	parse_args(argc, argv, width, height, debugx, debugy, scene, output);

	if (scene == 0) {
		usage();
	}

	World world;

	// enter debug mode
	if (debugx != -1) {
		parse_scene(world, width, height, 1, scene);
		world.camera.set_resolution(ivec2(width, height));
		world.render();

		debug = true;

		world.render_pixel({debugx, debugy});
		world.camera.set_pixel({debugx, debugy}, {1,0,0});
		dump_png(world.camera.image, width, height, "debug.png");
		return 0;
	}

    ncurses_init();
	getmaxyx(stdscr, height, width);

	parse_scene(world, width, height, PIXEL_AR, scene);
	
	// if output was set render the first frame to a png
	// taking into account the aspect ratio
	// of a terminal character
	if (output) {
		world.camera.set_resolution(ivec2(width*10, height*10*(1/PIXEL_AR)));
		world.render();
		dump_png(world.camera.image, width*10, height*10*(1/PIXEL_AR), output);
	}

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

void parse_args(int argc, char** argv, int& width, int& height, 
	int& debugx, int& debugy, char*& scene, char*& output) {
	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-f")==0 && i+1 < argc) {
			scene = argv[i+1];
			i++;
		} else if (strcmp(argv[i], "-d")==0 && i+4 < argc) {
			width = std::stoi(argv[i+1]);
			height = std::stoi(argv[i+2]);
			debugx = std::stoi(argv[i+3]);
			debugy = std::stoi(argv[i+4]);
			i += 4;
		} else if (strcmp(argv[i], "-o")==0 && i+1 < argc) {
			output = argv[i+1];
			i++;
		} else {
			usage();
		}
	}
}

void ncurses_init() {
	initscr();
    start_color();
    set_colors();
	noecho();
	nodelay(stdscr, TRUE);
	curs_set(0);
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
