#pragma once
#include <string>

const float PIXEL_AR = 1 / 2.35; // ad hoc aspect ratio of a terminal character

struct RenderConfig {
  std::string frame_output_dir = "";
  int frame_out_interval = 1;
  float frame_out_scale = 1.0;
  int max_out_frames = 1000;

  std::string text_output_dir = "";
  int text_out_interval = 1;
  int max_text_frames = 1000;
};

struct Config {
  std::string scene = "";

  int width = -1;
  int height = -1;
  int debugx = -1;
  int debugy = -1;

  std::string first_frame_output = "";
  float first_frame_scale = 1.0;
};
