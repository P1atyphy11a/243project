#include"project.h"

extern int color[8];

extern short int board[MAX_SIZE][MAX_SIZE];
extern short int dest_board[MAX_SIZE][MAX_SIZE];
extern int number_board[MAX_SIZE][MAX_SIZE];


extern int BOARD_SIZE;

int UUDDLLRRBABA[30][2];

void init(int n){                                       //create the DEST_BOARD
    int size = (n>=8)?8:n;
    BOARD_SIZE = size;
    for(int i=0; i<size; i++){
        for(int j=0; j<size; j++){
            board[i][j]=color[i];
            dest_board[i][j]=board[i][j];
            number_board[i][j]=i;
        }
    }
}

void shuffle(){        
    int times=rand()%20+10;
    UUDDLLRRBABA[times][0]=-1;                               //create the puzzle by radomly shuffle the DEST_BOARD
    for(int i=0;i<times;i++){
        int key=rand()%4;
        int num=rand()%(BOARD_SIZE-1);
        UUDDLLRRBABA[i][0]=key;
        UUDDLLRRBABA[i][1]=num;
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
    int temp2 = number_board[n][0];
    for(int i=0;i<BOARD_SIZE-1;i++){
        board[n][i]=board[n][i+1];
        number_board[n][i] = number_board[n][i+1];
    }
    board[n][BOARD_SIZE-1]=temp;
    number_board[n][BOARD_SIZE-1]=temp2;
}

void shift_right(int n){
    short int temp=board[n][BOARD_SIZE-1];
    int temp2 = number_board[n][BOARD_SIZE-1];
    for(int i=BOARD_SIZE-1;i>0;i--){
        board[n][i]=board[n][i-1];
        number_board[n][i]=number_board[n][i-1];
    }
    board[n][0]=temp;
    number_board[n][0]=temp2;
}

void shift_up(int n){
    short int temp=board[0][n];
    int temp2 = number_board[0][n];
    for(int i=0;i<BOARD_SIZE-1;i++){
        board[i][n]=board[i+1][n];
        number_board[i][n]=number_board[i+1][n];
    }
    board[BOARD_SIZE-1][n]=temp;
    number_board[BOARD_SIZE-1][n]=temp2;
}

void shift_down(int n){
    short int temp=board[BOARD_SIZE-1][n];
    int temp2 = number_board[BOARD_SIZE-1][n];
    for(int i=BOARD_SIZE-1;i>0;i--){
        board[i][n]=board[i-1][n];
        number_board[i][n]=number_board[i-1][n];
    }
    board[0][n]=temp;
    number_board[0][n]=temp2;
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
