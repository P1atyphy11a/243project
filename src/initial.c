#include"project.h"

extern int color[8];

extern short int board[MAX_SIZE][MAX_SIZE];

extern int BOARD_SIZE;

void init(int n){
    int size = (n>=8)?8:n;
    BOARD_SIZE = size;
    for(int i=0; i<size; i++){
        for(int j=0; j<size; j++){
            board[i][j]=color[i];
        }
    }
}

void shuffle(){
    for(int i=0;i<rand()%20+10;i++){
        int key=rand()%4;
        int num=rand()%(BOARD_SIZE-1);
        switch (key)
        {
        case 0:
            shift_left(num);
            printf("shift_left %d\n", num);
            break;

        case 1:
            shift_right(num);
            printf("shift_right %d\n", num);
            break;

        case 2:
            shift_up(num);
            printf("shift_up %d\n", num);
            break;

        case 3:
            shift_down(num);
            printf("shift_down %d\n", num);
            break;

        default:
            break;
        }
        print_board_debug();
    }
}

void shift_left(int n){
    short int temp=board[n][0];
    for(int i=0;i<BOARD_SIZE-1;i++){
        board[n][i]=board[n][i+1];
    }
    board[n][BOARD_SIZE-1]=temp;
}

void shift_right(int n){
    short int temp=board[n][BOARD_SIZE-1];
    for(int i=BOARD_SIZE-1;i>0;i--){
        board[n][i]=board[n][i-1];
    }
    board[n][0]=temp;
}

void shift_up(int n){
    short int temp=board[0][n];
    for(int i=0;i<BOARD_SIZE-1;i++){
        board[i][n]=board[i+1][n];
    }
    board[BOARD_SIZE-1][n]=temp;
}

void shift_down(int n){
    short int temp=board[BOARD_SIZE-1][n];
    for(int i=BOARD_SIZE-1;i>0;i--){
        board[i][n]=board[i-1][n];
    }
    board[0][n]=temp;
}

void print_board_debug(){
    for(int i=0;i<MAX_SIZE;i++){
        for(int j=0;j<MAX_SIZE;j++){
            printf("%d\t",board[i][j]);
        }
        printf("\n");
    }
    printf("------------------------------------------------------------\n");
}

void plot_pixel(int x, int y, short int color)
{
    *(short int *)(pixel_buffer_start + (y << 10) + (x << 1)) = color;
}

void draw_one_block(int x, int y, short int color){             //(x,y)is the left top pixel
    for(int i=x; i< x+50; i++){
      for(int j=y; j< y+50; j++){
         plot_pixel(i,j,color);
      }
    }

}


void draw_picture(){
    for(int i=0; i<MAX_SIZE; i++){
      for(int j=0; j<MAX_SIZE; j++){
         short int block_color = board[i][j];
         draw_one_block(j*50,i*50,block_color);
      }
    }
}
