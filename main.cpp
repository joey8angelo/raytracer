#include <iostream>
#include <string>
#include <unistd.h>
#include <ncurses.h>
#include "world.h"

void dump_png(unsigned int*, int, int, const char*);
void image_to_screen(unsigned int*, WINDOW* scr, int, int);
void move_camera(World& world, int key);
int keyboard(World& world);

const double PIXEL_AR = 1/2.35;

int main() {
	initscr();
	noecho();
	nodelay(stdscr, TRUE);

	int width, height;
	getmaxyx(stdscr, height, width);
	std::cout << "w " << width << " h " << height << std::endl;
	World world;
	
	// render the first frame of the world to a png
	world.camera.set_pos_and_aim(vec3(2,0,0), vec3(0,0,0), vec3(0,0,1));
	world.camera.focus(1, (double(width)/height) * PIXEL_AR, 50*(3.14159/180));
	world.camera.set_resolution(ivec2(width, height*(1/PIXEL_AR)));
	world.render();
	dump_png(world.camera.image, width, height*(1/PIXEL_AR), "image.png");
	
	// keep rendering the world to the screen
	world.camera.focus(1, (double(width)/height) * PIXEL_AR, 50*(3.14159/180));
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
			world.camera.position += vec3(0,0,-0.01);
			break;
		case 'a':
			world.camera.position += vec3(0,-0.01,0);
			break;
		case 's':
			world.camera.position += vec3(0,0,0.01);
			break;
		case 'd':
			world.camera.position += vec3(0,0.01,0);
			break;
	};
}
