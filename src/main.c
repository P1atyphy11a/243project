#include"project.h"

int color[8]={1, 2, 3, 4, 5, 6, 7, 8};
 
int board[MAX_SIZE][MAX_SIZE];

int BOARD_SIZE;

int main(){
    init(4);
    print_board_debug();
    shuffle();
    print_board_debug();
    return 0;
}