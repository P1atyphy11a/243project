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


#endif