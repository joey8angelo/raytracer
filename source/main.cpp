#include "config.h"
#include "world.h"
#include <cstring>
#include <fstream>
#include <iostream>
#include <ncurses.h>
#include <string>

void render(World &world, const RenderConfig &rconfig);
void dump_png(unsigned int *data, int width, int height,
              const std::string &filename);
void ncurses_init();
void set_colors();
bool parse_scene(World &world, int width, int height, float ar,
                 const std::string &fn);
bool parse_args(int argc, char **argv, int &width, int &height, int &debugx,
                int &debugy, std::string &scene, std::string &output,
                int &frame_out, float &scale);

// Turn command line arguments into a RenderConfig object and check validitiy of
// inputs
bool parse_args(int argc, char **argv, Config &config, RenderConfig &rconfig) {
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-i") == 0 && i + 1 < argc) {
      config.scene = std::string(argv[i + 1]);
      std::ifstream file(config.scene);
      if (!file.is_open()) {
        std::cerr << "Error: scene file " << config.scene << " does not exist."
                  << std::endl;
        return false;
      }
      i++;
    } else if (strcmp(argv[i], "-s") == 0 && i + 1 < argc) {
      config.scene = "source/scenes/ts_" + std::string(argv[i + 1]) + ".txt";
      std::ifstream file(config.scene);
      if (!file.is_open()) {
        std::cerr << "Error: test scene number " << std::string(argv[i + 1])
                  << " does not exist" << std::endl;
        return false;
      }
      i++;
    } else if (strcmp(argv[i], "-d") == 0 && i + 4 < argc) {
      config.width = std::stoi(argv[i + 1]);
      config.height = std::stoi(argv[i + 2]);
      config.debugx = std::stoi(argv[i + 3]);
      config.debugy = std::stoi(argv[i + 4]);
      i += 4;
    } else if (strcmp(argv[i], "-o") == 0 && i + 2 < argc) {
      config.first_frame_output = std::string(argv[i + 1]);
      config.first_frame_scale = std::stoi(argv[i + 2]);
      i += 2;
    } else if (strcmp(argv[i], "-O") == 0 && i + 4 < argc) {
      rconfig.frame_output_dir = std::string(argv[i + 1]);
      rconfig.frame_out_interval = std::stoi(argv[i + 2]);
      rconfig.frame_out_scale = std::stod(argv[i + 3]);
      rconfig.max_out_frames = std::stoi(argv[i + 4]);
      i += 4;
    } else if (strcmp(argv[i], "-t") == 0 && i + 3 < argc) {
      rconfig.text_output_dir = std::string(argv[i + 1]);
      rconfig.text_out_interval = std::stoi(argv[i + 2]);
      rconfig.max_text_frames = std::stoi(argv[i + 3]);
      i += 3;
    } else {
      std::cerr << "Usage: Raytracer [options]" << std::endl
                << "Options:" << std::endl
                << "  -i <input_scene> Input scene file" << std::endl
                << "  -s <scene_number> Test scene file number" << std::endl
                << "  -d <width> <height> <debug_x> <debug_y> Debug mode, "
                   "outputs debug info for pixel (debug_x, debug_y) at "
                   "resolution width x height, writes to debug.png"
                << std::endl
                << "  -o <output_file> <scale> Output the first frame to "
                   "output_file at resolution width*scale x height*scale"
                << std::endl
                << "  -O <output_path> <i> <scale> <max_frames> Output every "
                   "ith frame "
                   "to output_path at resolution width*scale x height*scale"
                << std::endl
                << "  -t <output_dir> <i> <max_frames> Output every ith "
                   "frame to output_dir until max_frames is reached"
                << std::endl;
      return false;
    }
  }
  if (config.scene == "") {
    std::cerr << "Error: no scene file provided, use -i or -s to provide a "
                 "scene file"
              << std::endl;
    return false;
  }
  return true;
}

bool debug = false;

int main(int argc, char **argv) {
  Config config;
  RenderConfig rconfig;

  if (!parse_args(argc, argv, config, rconfig)) {
    return 1;
  }

  World world;

  // enter debug mode
  if (config.debugx >= 0 && config.debugy >= 0 && config.width > 0 &&
      config.height > 0) {
    // render the world
    if (!parse_scene(world, config.width, config.height, 1, config.scene))
      return 1;
    world.camera.set_resolution(ivec2(config.width, config.height));
    world.render();

    // render a pixel with debug on
    debug = true;
    world.render_pixel({config.debugx, config.debugy});
    world.camera.set_pixel({config.debugx, config.debugy}, {1, 0, 0});

    dump_png(world.camera.image, config.width, config.height, "debug.png");

    return 0;
  }

  // start ncurses
  ncurses_init();

  // get the terminal size
  getmaxyx(stdscr, config.height, config.width);

  if (!parse_scene(world, config.width, config.height, PIXEL_AR,
                   config.scene)) {
    endwin();
    return 2;
  }

  // if output was set render the first frame to a png
  if (config.first_frame_output != "") {
    world.camera.set_resolution(
        ivec2(config.width * config.first_frame_scale,
              config.height * config.first_frame_scale * (1 / PIXEL_AR)));
    world.render();
    dump_png(world.camera.image, config.width * config.first_frame_scale,
             config.height * config.first_frame_scale * (1 / PIXEL_AR),
             config.first_frame_output);
  }

  // keep rendering the world to the screen until user quits
  world.camera.set_resolution(ivec2(config.width, config.height));
  render(world, rconfig);

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
  keypad(stdscr, TRUE);
}
