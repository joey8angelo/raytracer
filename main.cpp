#include <iostream>
#include <string>
#include <unistd.h>
#include <ncurses.h>
#include "world.h"

#include "phong_shader.h"
#include "sphere.h"
#include "point_light.h"

void dump_png(unsigned int*, int, int, const char*);
void image_to_screen(unsigned int*, WINDOW* scr, int, int);
void move_camera(World& world, int key);
int keyboard(World& world);
void build_world(World& world, int width, int height);
void ncurses_init();

const double PIXEL_AR = 1/2.35;

int main() {
	ncurses_init();

	int width, height;
	getmaxyx(stdscr, height, width);
	std::cout << "w " << width << " h " << height << std::endl;
	
	World world;
	build_world(world, width, height);
	
	// render the first frame to a png
	// taking into account the aspect ratio
	// of a terminal character
	world.camera.set_resolution(ivec2(width*20, height*(1/PIXEL_AR)*20));
	world.render();
	dump_png(world.camera.image, width*20, height*(1/PIXEL_AR)*20, "image.png");
	
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
	noecho();
	nodelay(stdscr, TRUE);
	curs_set(0);
	start_color();

	//init_pair(1, COLOR_RED, COLOR_BLACK);
	//init_pair(2, COLOR_BLUE, COLOR_BLACK);
	//init_pair(3, COLOR_GREEN, COLOR_BLACK);
	//init_pair(4, COLOR_CYAN, COLOR_BLACK);
	//init_pair(5, COLOR_YELLOW, COLOR_BLACK);
	//init_pair(6, COLOR_MAGENTA, COLOR_BLACK);
	//init_pair(7, COLOR_WHITE, COLOR_BLACK);
}

void build_world(World& world, int width, int height) {
	world.camera.focus(1, (double(width)/height) * PIXEL_AR, 70*(pi/180));
	world.camera.set_pos_and_aim({6,0,0}, {0,0,0}, {0,0,1});
	
	Sphere* sphere = new Sphere({0,0,0}, 2.5);
	sphere->shader = new Phong_Shader(world,{1,1,1},{1,1,1},{1,1,1},50);
	world.objects.push_back(sphere);
	
	world.lights.push_back(new Point_Light({7,5,4}, {1,0,0}, 800));
	//world.lights.push_back(new Point_Light({8,-9,4}, {0,0,1}, 800));

	world.ambient_intensity = 0.2;
	world.ambient_color = vec3(1,1,1);
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
