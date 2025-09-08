# CLI raytracer
This is a simple raytracer written in C++ that renders scenes directly to the terminal as ASCII art. The program calculates lighting and shadows to create 3D-looking images using only text.

## Features
Real-time ASCII rendering in terminal

Spheres, planes, and obj files for meshes

Bounding Volume Hierarchies

Basic phong and flat shaders

Custom scene building

## Usage
-i <input_scene> Input scene file

-s <scene_number> Test scene file number

-d <width> <height> <debug_x> <debug_y> Debug mode, outputs debug info for pixel (debug_x, debug_y) at resolution width x height, writes to debug.png

-o <output_file> <scale> Output the first frame to output_file at resolution width*scale x height*scale

-O <output_path> <i> <scale> <max_frames> Output every ith frame to output_path at resolution width*scale x height*scale

-t <output_dir> <i> <max_frames> Output every ith frame to output_dir until max_frames is reached

Use the print_ascii.py script to print the ascii frames to the terminal in a loop
![Donut](ndonut.gif)
![Monkey](nmonkey.gif)
