#include "ray.h"
#include "world.h"
#include <chrono>
#include <iostream>
#include <ncurses.h>
#include <string>
#include <vector>

void image_to_window(const unsigned int *data, const std::vector<bool> &block,
                     WINDOW *win, int width, int height);
void dump_png(unsigned int *data, int width, int height,
              const std::string &filename);
int keyboard(World &world);
void move_camera(World &world, int key);
void rotate_camera(World &world, vec3 point, int key);
void rot(vec3 &p, const vec3 &c, const vec3 &u, float theta);

class Ray;

auto prev_time = std::chrono::system_clock::now();

class WindowManager {
public:
  std::vector<WINDOW *> windows;
  std::vector<void (*)(WINDOW *, const World &)> window_updaters;
  std::vector<bool> blocked;
  const World &world;

  explicit WindowManager(const World &world) : world(world) {
    int width = world.camera.number_pixels[0];
    int height = world.camera.number_pixels[1];
    blocked.resize(width * height);
    for (int i = 0; i < width * height; i++) {
      blocked[i] = false;
    }
  }

  ~WindowManager() {
    for (auto win : windows) {
      delwin(win);
    }
  }
  WINDOW *create_window(int wwidth, int wheight, int x, int y,
                        void (*updater)(WINDOW *, const World &) = nullptr) {
    int width = world.camera.number_pixels[0];
    int height = world.camera.number_pixels[1];
    window_updaters.push_back(updater);
    WINDOW *win = newwin(wheight, wwidth, y, x);
    box(win, 0, 0);
    windows.push_back(win);

    for (int i = 0; i < wwidth; i++) {
      for (int j = 0; j < wheight; j++) {
        int idx = (height - j - y) * width + i - width + x;
        if (idx >= 0 && idx < width * height) {
          blocked[idx] = true;
        }
      }
    }
    return win;
  }

  void refresh_windows() {
    for (size_t i = 0; i < windows.size(); i++) {
      if (window_updaters[i]) {
        window_updaters[i](windows[i], world);
      }
      wrefresh(windows[i]);
    }
  }
};

// render loop for the world
void render(World &world, const float &pixel_ar, const std::string &output,
            const int &frame_out, const float &scale) {
  int width = world.camera.number_pixels[0];
  int height = world.camera.number_pixels[1];
  size_t frame = 0;

  WindowManager wm(world);

  WINDOW *win =
      wm.create_window(20, 3, 0, 0, [](WINDOW *win, const World &world) {
        mvwprintw(win, 1, 1, "%.2f %.2f %.2f", world.camera.position[0],
                  world.camera.position[1], world.camera.position[2]);
      });
  mvwprintw(win, 0, 1, "Camera Position ");

  win = wm.create_window(20, 3, 0, 3, [](WINDOW *win, const World &world) {
    mvwprintw(win, 1, 1, "%.2f %.2f %.2f", world.camera.look[0],
              world.camera.look[1], world.camera.look[2]);
  });
  mvwprintw(win, 0, 1, "Camera Look Dir ");

  win = wm.create_window(6, 3, 0, 6, [](WINDOW *win, const World &world) {
    auto cur_time = std::chrono::system_clock::now();
    static auto prev_time = cur_time;
    static size_t frame = 0;
    static size_t prev_frame = 0;
    if (cur_time - prev_time >= std::chrono::seconds(1)) {
      mvwprintw(win, 1, 1, "%-*zu", 4, frame - prev_frame);
      prev_time = cur_time;
      prev_frame = frame;
    }
    frame++;
  });
  mvwprintw(win, 0, 1, "FPS ");

  while (true) {
    // handle keyboard inputs
    int k = keyboard(world);
    if (k == -1)
      break;

    // if we want to dump a sequence of images to every frame_out frame
    if (frame_out != 0 && frame % frame_out == 0) {
      world.camera.set_resolution(
          ivec2(width * scale, height * scale * (1 / pixel_ar)));
      world.render();
      std::string nm = std::to_string(frame);
      std::string zs = std::string(4 - nm.size(), '0');
      std::string fn = output + "_" + zs + nm + ".png";
      dump_png(world.camera.image, width * scale,
               height * scale * (1 / pixel_ar), fn.c_str());
      world.camera.set_resolution(ivec2(width, height));
    }

    // render the world
    world.render();
    // render the camera image to stdscr
    image_to_window(world.camera.image, wm.blocked, stdscr, width, height);
    // refresh windows
    refresh();
    wm.refresh_windows();

    frame++;
  }
}

int keyboard(World &world) {
  int ch = getch();
  static bool rotate = false;
  static vec3 pos = {0, 0, 0};

  Hit hit;
  Ray ray;

  switch (ch) {
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

void move_camera(World &world, int key) {
  const float speed = 0.1;
  const float slow = 0.05;
  vec3 p;
  switch (key) {
  case 'w':
    world.camera.position += world.camera.look * speed;
    break;
  case 'a':
    world.camera.position -= world.camera.horizontal * speed;
    break;
  case 's':
    world.camera.position -= world.camera.look * speed;
    break;
  case 'd':
    world.camera.position += world.camera.horizontal * speed;
    break;
  case KEY_UP:
    p = world.camera.position;
    p += world.camera.look + world.camera.vertical * slow;
    world.camera.aim(p);
    break;
  case KEY_DOWN:
    p = world.camera.position;
    p += world.camera.look - world.camera.vertical * slow;
    world.camera.aim(p);
    break;
  case KEY_LEFT:
    p = world.camera.position;
    p += world.camera.look - world.camera.horizontal * slow;
    world.camera.aim(p);
    break;
  case KEY_RIGHT:
    p = world.camera.position;
    p += world.camera.look + world.camera.horizontal * slow;
    world.camera.aim(p);
    break;
  }
}

// rotate camera around a point
void rotate_camera(World &world, vec3 point, int key) {
  const float speed = 0.05;
  const vec3 h = world.camera.horizontal;
  const vec3 v = world.camera.vertical;
  switch (key) {
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

void rot(vec3 &p, const vec3 &c, const vec3 &u, float theta) {
  vec3 r = p - c;
  float cos_theta = cos(theta);
  float sin_theta = sin(theta);
  p = c + r * cos_theta + cross(u, r) * sin_theta +
      u * (dot(u, r) * (1 - cos_theta));
}
