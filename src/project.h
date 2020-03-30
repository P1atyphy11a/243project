#include <math.h>

short int color[7]={0xF000, 0x0FF0, 0x000F, 0xFFF0, 0x0FFF, 0xF00F, 0xFFFF};

short int board[7][7];

void init(int n){
    int size = (n>=7)?7:n;
    for(int i=0; i<size; i++){
        for(int j=0; j<size; j++){
            board[i][j]=color[i];
        }
    }
}