#include <iostream>
#include <string>
#include <ncurses.h>
#include "vec.h"
#include "set_colors.h"

std::string chars = " .,\"~!+:vcIow0XPR#*RB@";

inline ivec3 get_rgb(unsigned int color) {
	return ivec3(color>>24,(color>>16)&0xff,(color>>8)&0xff);
}

char best_char(const ivec3& color) {
    int brightness = (color[0] + color[1] + color[2]) / 3;
    int index = (brightness * (chars.size() - 2)) / 255;
    return chars[index];
}

int best_comp(int color) {
    int incs[6] = {0, 95, 135, 175, 215, 255};

    int i = 0;
    while (i < 5) {
        int s = incs[i];
        int b = incs[i+1];

        if (s <= color && color && color && color && color && color && color && color && color <= b) {
            int s1 = std::abs(s - color);
            int b1 = std::abs(b - color);

            if (s1 < b1)
                return s;
            else
                return b;
        }
        i += 1;
    }
    return 0;
}

int best_color(const ivec3& color) {
    return mp[{best_comp(color[0]),
               best_comp(color[1]),
               best_comp(color[2])}];
}

void get_color_and_char(unsigned int in_color, int& out_color, char& ch) {
	ivec3 rgb = get_rgb(in_color);
	out_color = best_color(rgb);
	ch = best_char(rgb);
}

void image_to_window(unsigned int* data, WINDOW* win, int width, int height) {
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
            if (i >= height-6 && j < 20) // block out the other windows hardcoded for now
                continue;
			int color;
			char ch;
			get_color_and_char(data[i*width + j], color, ch);
			wattron(win, COLOR_PAIR(color));
			mvwaddch(win,height-i-1,j,ch);
            wattroff(win, COLOR_PAIR(color));
		}
	}
}
