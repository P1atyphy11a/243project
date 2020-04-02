#include "project.h"

extern short int board[MAX_SIZE][MAX_SIZE];
extern short int dest_board[MAX_SIZE][MAX_SIZE];
extern int BOARD_SIZE;
extern volatile int pixel_buffer_start;

void plot_pixel(int x, int y, short int color){
    *(short int *)(pixel_buffer_start + (y << 10) + (x << 1)) = color;
}

void draw_one_block(int x, int y, short int color, int width){             //(x,y)is the left top pixel
    for(int i=x; i< x+width; i++)
      for(int j=y; j< y+width; j++)
         plot_pixel(i,j,color);
}

void draw_screen(){
    draw_puzzle();
    draw_dest_board();
}

void draw_puzzle(){
    clear_screen();
    for(int i=0; i<BOARD_SIZE; i++){                            //only draw the actural puzzle board
      for(int j=0; j<BOARD_SIZE; j++){
         short int block_color = board[i][j];
         draw_one_block(20+j*50,20+i*50,block_color, 50);
      }
    }
}

void clear_screen(){
    for(int i=0;i<320;i++)
        for(int j=0;j<240;j++)
            plot_pixel(i,j,0xFFFA);
}

void draw_dest_board(){
    for(int i=0; i<BOARD_SIZE; i++){
        for(int j=0;j<BOARD_SIZE; j++){
            short int block_color = dest_board[i][j];
            draw_one_block(255+j*15, 15+i*15, block_color, 15);
        }
    }
}