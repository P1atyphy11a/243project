#include "project.h"

extern int board[MAX_SIZE][MAX_SIZE];
extern int BOARD_SIZE;
extern volatile int pixel_buffer_start;

void plot_pixel(int x, int y, short int color){
    *(short int *)(pixel_buffer_start + (y << 10) + (x << 1)) = color;
}

void draw_one_block(int x, int y, short int color){             //(x,y)is the left top pixel
    for(int i=x; i< x+50; i++)
      for(int j=y; j< y+50; j++)
         plot_pixel(i,j,color);
}

void draw_picture(){
    clear_screen();
    for(int i=0; i<BOARD_SIZE; i++){
      for(int j=0; j<BOARD_SIZE; j++){
         short int block_color = board[i][j];
         draw_one_block(j*50,i*50,block_color);
      }
    }
}

void clear_screen(){
    for(int i=0;i<320;i++)
        for(int j=0;j<240;j++)
            plot_pixel(i,j,0x0000);
}