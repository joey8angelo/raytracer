#include <iostream>
#include <cstring>
#include <unistd.h>
#include <ncurses.h>
#include "world.h"
#include "util.h"

void dump_png(unsigned int*, int, int, const char*);
void image_to_window(unsigned int*, WINDOW* win, int, int);
void move_camera(World& world, int key);
int keyboard(World& world);
void ncurses_init();
void set_colors();
void parse_scene(World& world, int width, int height, double ar, 
        const char* fn);
void parse_args(int argc, char** argv, int& width, int& height, 
        int& debugx, int& debugy, char*& scene, char*& output, bool& dump);
int best_color(const ivec3& color);
WINDOW* create_window(int width, int height, int x, int y);


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
    bool dump = false;

	parse_args(argc, argv, width, height, 
            debugx, debugy, scene, output, dump);

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
    WINDOW* cam_win = create_window(20, 3, 0, 0);

	getmaxyx(stdscr, height, width);
	parse_scene(world, width, height, PIXEL_AR, scene);
	
	// if output was set render the first frame to a png
	// taking into account the aspect ratio
	// of a terminal character
	if (output && !dump) {
		world.camera.set_resolution(ivec2(width*10, height*10*(1/PIXEL_AR)));
		world.render();
		dump_png(world.camera.image, width*10, height*10*(1/PIXEL_AR), output);
	}

    int t = 0;

    // keep rendering the world to the screen
	world.camera.set_resolution(ivec2(width, height));
	while (true) {
		// handle keyboard inputs
        keyboard(world);

        // if we want to dump a sequence of images to every 10th frame
        if (dump && t%10==0) {
            int c = 1;
            world.camera.set_resolution(ivec2(width*c, height*c*(1/PIXEL_AR)));
            world.render();
            std::string nm = std::to_string(t/10);
            std::string zs = std::string(4-nm.size(), '0');
            std::string fn = std::string(output) + zs + nm + ".png";
            dump_png(world.camera.image, width*c, height*c*(1/PIXEL_AR), 
                    fn.c_str());
            world.camera.set_resolution(ivec2(width, height));
        }

        // render the world
		world.render();
        // render the camera image to stdscr
		image_to_window(world.camera.image, stdscr, width, height);

        // render info to camera window
        mvwprintw(cam_win,1,1,"%.2f %.2f %.2f", world.camera.position[0], 
                world.camera.position[1], world.camera.position[2]);
		
        // refresh windows
        refresh();
        wrefresh(cam_win);

	    t++;
    }
	
	endwin();
	return 0;
}

void parse_args(int argc, char** argv, int& width, int& height, 
	int& debugx, int& debugy, char*& scene, char*& output, bool& dump) {
	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-i")==0 && i+1 < argc) {
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
        } else if (strcmp(argv[i], "-O")==0 && i+1 < argc) {
            output = argv[i+1];
            dump=true;
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
    keypad(stdscr, TRUE);
}

int keyboard(World& world) {
	int ch = getch();

	if (ch == ERR)
		return 0;
        
	if (ch == 'w' || ch == 'a' || ch == 's' || ch == 'd' ||
        ch == KEY_UP || ch == KEY_DOWN || ch == KEY_LEFT || ch == KEY_RIGHT) {
		move_camera(world, ch);
    }

	return 1;
}

void move_camera(World& world, int key) {
    static double rx = 0;
    static double ry = 0;
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

WINDOW* create_window(int width, int height, int x, int y) {
    WINDOW* win = newwin(height, width, y, x);
    box(win, 0, 0);
    return win;
}
