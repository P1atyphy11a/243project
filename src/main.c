#include"project.h"

int color[8]={0xF800, 0x07E0, 0x001F, 0xFFE0, 0xF81F, 0x07FF, 0xF4CE, 0x970F};  // colors will be used for the puzzle

short int board[MAX_SIZE][MAX_SIZE];                                                  //array to store the puzzle status, MAX_SIZE=8
short int dest_board[MAX_SIZE][MAX_SIZE];

int BOARD_SIZE;                                                                 //actural puzzle size, no bigger than MAX_SIZE

volatile int pixel_buffer_start;

int main(){


    disable_A9_interrupts(); // disable interrupts in the A9 processor
    set_A9_IRQ_stack(); // initialize the stack pointer for IRQ mode
    config_GIC(); // configure the general interrupt controller
    config_KEYs(); // configure pushbutton KEYs to generate interrupts
    enable_A9_interrupts(); // enable interrupts in the A9 processor

    volatile int * pixel_ctrl_ptr = (int *)0xFF203020;

    pixel_buffer_start = *pixel_ctrl_ptr;

    init(4);
    print_board_debug();
    shuffle();
    print_board_debug();
    draw_screen();
    while(1);
    return 0;
}
