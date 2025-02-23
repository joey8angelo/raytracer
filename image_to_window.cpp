#include <iostream>
#include <string>
#include <ncurses.h>
#include "vec.h"

int colors[] = {COLOR_BLACK, COLOR_RED, COLOR_GREEN, COLOR_YELLOW, COLOR_BLUE,
                    COLOR_MAGENTA, COLOR_CYAN, COLOR_WHITE};

//std::string chars = " `.-':_,^=;><+!rc*/z?sLTv)J7(|Fi{C}fI31tlu[neoZ5Yxjya]2ESwqkP6h9d4VpOGbUAKXHm8RD#$Bg0MNWQ%&@";

//std::string chars = " .:-=+*#%@";

//std::string chars = " .'`^\",:;Il!i~+-?]}{1)(|\\/tfjrxnuvczXYUJCLQ0OZmwqpdbkhao*#MW&8%B@$";

std::string chars = " .,\"~!+:vcIow0XPR#*RB@";

//std::string chars = "@&%QWNM0gB$#DR8mHXKAUbGOpV4d9h6PkqwSE2]ayjxY5Zoen[ult13If}C{iD|(7J)vTLs?z/*cr!+<>;=^,_:'-.` ";

inline vec3 get_rgb(unsigned int color) {
	return vec3(color>>24,(color>>16)&0xff,(color>>8)&0xff);
}

char best_char(const vec3& color) {
    int brightness = (color[0] + color[1] + color[2]) / 3;
    int index = (brightness * (chars.size() - 2)) / 255;
    return chars[index];
}

int best_color(const vec3& color) {
	int best_match = 0;
	double best_distance = 1e9;

	for (int i = 0; i < 8; i++) {
        	short nr, ng, nb;
        	color_content(colors[i], &nr, &ng, &nb);

        	// Convert ncurses color (0-1000) to 0-255 range
        	nr = (nr * 255) / 1000;
        	ng = (ng * 255) / 1000;
        	nb = (nb * 255) / 1000;

        	double dist = sqrt(pow(color[0] - nr, 2) + 
                              pow(color[1] - ng, 2) + 
                              pow(color[2] - nb, 2));
        	
		if (dist < best_distance) {
            		best_distance = dist;
            		best_match = colors[i];
        	}
    	}
	return best_match;
}

void get_color_and_char(unsigned int in_color, int& out_color, char& ch) {
	vec3 rgb = get_rgb(in_color);
	out_color = best_color(rgb);
	ch = best_char(rgb);
}

void image_to_screen(unsigned int* data, WINDOW* scr, int width, int height) {
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			//if (i == 15 && j == 30)
			//	std::cout << "debug" << std::endl;
			int color;
			char ch;
			get_color_and_char(data[i*width + j], color, ch);
			//wattron(scr, COLOR_PAIR(1));
			mvwaddch(scr,height-i-1,j,ch);
		}
	}
}
