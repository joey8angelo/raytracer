#include <iostream>
#include <string>
#include <unistd.h>
#include <ncurses.h>
#include "world.h"

void dump_png(unsigned int*, int, int, const char*);
void image_to_screen(unsigned int*, WINDOW* scr, int, int);
void move_camera(World& world, int key);
int keyboard(World& world);
void ncurses_init();
void set_colors();
void parse(World& world, int width, int height, double ar, const char* fn);

bool debug = false;

int best_color(const ivec3& color);

const double PIXEL_AR = 1/2.35;

int main(int argc, char** argv) {
	int width, height, debugx, debugy;

	World world;

	// enter debug mode
	if (argc == 3) {
		width = 300;
		height = 300;
		debugx = std::stoi(argv[1]);
        debugy = std::stoi(argv[2]);

		parse(world, width, height, 1, "scene1.txt");
		world.camera.set_resolution(ivec2(width, height));
		world.render();

		debug = true;

		world.render_pixel({debugx, debugy});
		world.camera.set_pixel({debugx, debugy}, {1,0,0});
		dump_png(world.camera.image, width, height, "image.png");
		return 0;
	}

    ncurses_init();
	getmaxyx(stdscr, height, width);

	parse(world, width, height, PIXEL_AR, "scene1.txt");
	
	// render the first frame to a png
	// taking into account the aspect ratio
	// of a terminal character
	world.camera.set_resolution(ivec2(width*10, height*(1/PIXEL_AR)*10));
	world.render();

	dump_png(world.camera.image, width*10, height*(1/PIXEL_AR)*10, "image.png");	

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
