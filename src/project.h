#ifndef PROJECT
#define PROJECT


#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 8

void init(int n);


void shuffle();

void shift_left(int n);
void shift_right(int n);
void shift_up(int n);
void shift_down(int n);

void print_board_debug();

void plot_pixel(int x, int y, short int color);

void draw_one_block(int x, int y, short int color, int width);

void draw_screen();

void draw_puzzle();

void clear_screen();

void draw_dest_board();

#endif
