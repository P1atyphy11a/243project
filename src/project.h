#ifndef PROJECT
#define PROJECT


#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 8
volatile int pixel_buffer_start;

void init(int n);


void shuffle();

void shift_left(int n);
void shift_right(int n);
void shift_up(int n);
void shift_down(int n);

void print_board_debug();

void plot_pixel(int x, int y, short int color);

void draw_one_block(int x, int y, short int color);

void draw_picture();


#endif
