#include <iostream>
#include <ncurses.h>

void image_to_screen(unsigned int* data, WINDOW* scr, int width, int height) {
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (data[i*width + j] == 255)
				mvwaddch(scr,i,j,' ');
			else
				mvwaddch(scr,i,j, '@');
		}
	}
}
