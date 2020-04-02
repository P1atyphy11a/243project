#include"project.h"

int color[8]={0xF800, 0x07E0, 0x001F, 0xFFE0, 0xF81F, 0x07FF, 0xF4CE, 0x970F};  // colors will be used for the puzzle

int board[MAX_SIZE][MAX_SIZE];                                                  //array to store the puzzle status, MAX_SIZE=8

int BOARD_SIZE;                                                                 //actural puzzle size, no bigger than MAX_SIZE

volatile int pixel_buffer_start;

int main(){
    volatile int * pixel_ctrl_ptr = (int *)0xFF203020;

    pixel_buffer_start = *pixel_ctrl_ptr;

    init(4);
    print_board_debug();
    shuffle();
    print_board_debug();
    draw_picture();
    return 0;
}
