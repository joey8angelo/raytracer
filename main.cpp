#include "world.h"
#include <cstring>
#include <iostream>
#include <ncurses.h>
#include <string>

void render(World &world, const double &pixel_ar, const std::string &output,
            const int &frame_out, const double &scale);
void dump_png(unsigned int *data, int width, int height,
              const std::string &filename);
void ncurses_init();
void set_colors();
bool parse_scene(World &world, int width, int height, double ar,
                 const std::string &fn);
bool parse_args(int argc, char **argv, int &width, int &height, int &debugx,
                int &debugy, std::string &scene, std::string &output,
                int &frame_out, double &scale);

void usage() {
  std::cerr << "usage: raytracer [-i input_scene] [-s scene_number] [-d width "
               "height debug_x "
               "debug_y] [-o output_file] [-O dump_file frame_i]\n";
  endwin();
}

bool debug = false;
const double PIXEL_AR = 1 / 2.35; // ad hoc aspect ratio of a terminal character

int main(int argc, char **argv) {
  int width, height, debugx, debugy;
  debugx = debugy = -1;
  std::string scene = "";
  std::string output = "";
  int frame_out = 0;
  double scale = 1.0;

  if (!parse_args(argc, argv, width, height, debugx, debugy, scene, output,
                  frame_out, scale)) {
    usage();
    return 1;
  }

  if (scene == "") {
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
  if (output != "" && frame_out == 0) {
    world.camera.set_resolution(
        ivec2(width * scale, height * scale * (1 / PIXEL_AR)));
    world.render();
    dump_png(world.camera.image, width * scale, height * scale * (1 / PIXEL_AR),
             output);
  }

  // keep rendering the world to the screen until user quits
  world.camera.set_resolution(ivec2(width, height));
  render(world, PIXEL_AR, output, frame_out, scale);

  endwin();
  return 0;
}

bool parse_args(int argc, char **argv, int &width, int &height, int &debugx,
                int &debugy, std::string &scene, std::string &output,
                int &frame_out, double &scale) {
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-i") == 0 && i + 1 < argc) {
      scene = std::string(argv[i + 1]);
      i++;
    } else if (strcmp(argv[i], "-s") == 0 && i + 1 < argc) {
      scene = "scenes/ts_" + std::string(argv[i + 1]) + ".txt";
      i++;
    } else if (strcmp(argv[i], "-d") == 0 && i + 4 < argc) {
      width = std::stoi(argv[i + 1]);
      height = std::stoi(argv[i + 2]);
      debugx = std::stoi(argv[i + 3]);
      debugy = std::stoi(argv[i + 4]);
      i += 4;
    } else if (strcmp(argv[i], "-o") == 0 && i + 2 < argc) {
      output = std::string(argv[i + 1]);
      scale = std::stoi(argv[i + 2]);
      i += 2;
    } else if (strcmp(argv[i], "-O") == 0 && i + 3 < argc) {
      output = std::string(argv[i + 1]);
      frame_out = std::stoi(argv[i + 2]);
      scale = std::stod(argv[i + 3]);
      i += 3;
    } else {
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
