#include "world.h"
#include <cstring>
#include <iostream>
#include <ncurses.h>

void render(World &world, int width, int height, double pixel_ar,
            const char *output, bool dump);
void dump_png(unsigned int *, int, int, const char *);
void ncurses_init();
void set_colors();
bool parse_scene(World &world, int width, int height, double ar,
                 const char *fn);
bool parse_args(int argc, char **argv, int &width, int &height, int &debugx,
                int &debugy, char *&scene, char *&output, bool &dump);

void usage() {
  std::cerr << "usage: raytracer [-i input_scene] [-d width height debug_x "
               "debug_y] [-o output_file] [-O dump_file]\n";
  endwin();
}

bool debug = false;
const double PIXEL_AR = 1 / 2.35; // ad hoc aspect ratio of a terminal character

int main(int argc, char **argv) {
  int width, height, debugx, debugy;
  debugx = debugy = -1;
  char *scene = 0;
  char *output = 0;
  bool dump = false;

  if (!parse_args(argc, argv, width, height, debugx, debugy, scene, output,
                  dump)) {
    usage();
    return 1;
  }

  if (scene == 0) {
    usage();
    return 1;
  }

  World world;

  // enter debug mode
  if (debugx != -1) {
    // render the world
    if (!parse_scene(world, width, height, 1, scene))
      return 1;
    world.camera.set_resolution(ivec2(width, height));
    world.render();

    // render a pixel with debug on
    debug = true;
    world.render_pixel({debugx, debugy});
    world.camera.set_pixel({debugx, debugy}, {1, 0, 0});

    dump_png(world.camera.image, width, height, "debug.png");

    return 0;
  }

  // start ncurses
  ncurses_init();

  // get the terminal size
  getmaxyx(stdscr, height, width);

  if (!parse_scene(world, width, height, PIXEL_AR, scene)) {
    endwin();
    return 1;
  }

  // if output was set render the first frame to a png
  if (output && !dump) {
    world.camera.set_resolution(
        ivec2(width * 10, height * 10 * (1 / PIXEL_AR)));
    world.render();
    dump_png(world.camera.image, width * 10, height * 10 * (1 / PIXEL_AR),
             output);
  }

  // keep rendering the world to the screen until user quits
  world.camera.set_resolution(ivec2(width, height));
  render(world, width, height, PIXEL_AR, output, dump);

  endwin();
  return 0;
}

bool parse_args(int argc, char **argv, int &width, int &height, int &debugx,
                int &debugy, char *&scene, char *&output, bool &dump) {
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-i") == 0 && i + 1 < argc) {
      scene = argv[i + 1];
      i++;
    } else if (strcmp(argv[i], "-d") == 0 && i + 4 < argc) {
      width = std::stoi(argv[i + 1]);
      height = std::stoi(argv[i + 2]);
      debugx = std::stoi(argv[i + 3]);
      debugy = std::stoi(argv[i + 4]);
      i += 4;
    } else if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
      output = argv[i + 1];
      i++;
    } else if (strcmp(argv[i], "-O") == 0 && i + 1 < argc) {
      output = argv[i + 1];
      dump = true;
      i++;
    } else {
      usage();
      return false;
    }
  }
  return true;
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
