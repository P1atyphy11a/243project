#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include<stdlib.h>

// extern int BOARD_SIZE;

#define BOARD_SIZE 3

int board_to_id(int (*board)[BOARD_SIZE]){
    int id=0;
    for(int i=0;i<BOARD_SIZE;i++)
        for(int j=0;j<BOARD_SIZE;j++){
            id+=board[i][j]*(pow(BOARD_SIZE,BOARD_SIZE*i+j));
        }
    return id;
}

void id_to_board(int (*board)[BOARD_SIZE], int id){
    int tempID=id;
    // int temp;

    for(int i=8;i>=0;i--){
        int temp=0;
        while(tempID-pow(BOARD_SIZE,i)>=0){
            temp++;
            tempID-=pow(BOARD_SIZE, i);
            // printf("ID after sub:%d, count:%d\n",tempID, temp);
        }

        switch(i){
            case 0:
                board[0][0]=temp;
                break;
            case 1:
                board[0][1]=temp;
                break;
            case 2:
                board[0][2]=temp;
                break;
            case 3:
                board[1][0]=temp;
                break;
            case 4:
                board[1][1]=temp;
                break;
            case 5:
                board[1][2]=temp;
                break;
            case 6:
                board[2][0]=temp;
                break;
            case 7:
                board[2][1]=temp;
                break;
            case 8:
                board[2][2]=temp;
                break;
            default:
                break;
        }

    }

}

void print_board_autosolve_test(int (*board)[BOARD_SIZE]){
    for(int i=0;i<BOARD_SIZE;i++){
        for(int j=0;j<BOARD_SIZE;j++)
            printf("%d ", board[i][j]);
        printf("\n");
    }
}

void shift_left_with_board(int (*board)[BOARD_SIZE],int n){
    short int temp=board[n][0];
    for(int i=0;i<BOARD_SIZE-1;i++){
        board[n][i]=board[n][i+1];
    }
    board[n][BOARD_SIZE-1]=temp;
}

void shift_right_with_board(int (*board)[BOARD_SIZE],int n){
    short int temp=board[n][BOARD_SIZE-1];
    for(int i=BOARD_SIZE-1;i>0;i--){
        board[n][i]=board[n][i-1];
    }
    board[n][0]=temp;
}

void shift_up_with_board(int (*board)[BOARD_SIZE],int n){
    short int temp=board[0][n];
    for(int i=0;i<BOARD_SIZE-1;i++){
        board[i][n]=board[i+1][n];
    }
    board[BOARD_SIZE-1][n]=temp;
}

void shift_down_with_board(int (*board)[BOARD_SIZE],int n){
    short int temp=board[BOARD_SIZE-1][n];
    for(int i=BOARD_SIZE-1;i>0;i--){
        board[i][n]=board[i-1][n];
    }
    board[0][n]=temp;
}

int id_shift_right(int id, int n){
    int temp_board[BOARD_SIZE][BOARD_SIZE];
    id_to_board(temp_board, id);
    shift_right_with_board(temp_board, n);
    return board_to_id(temp_board);
}

int id_shift_left(int id, int n){
    int temp_board[BOARD_SIZE][BOARD_SIZE];
    id_to_board(temp_board, id);
    shift_left_with_board(temp_board, n);
    return board_to_id(temp_board);
}

int id_shift_up(int id, int n){
    int temp_board[BOARD_SIZE][BOARD_SIZE];
    id_to_board(temp_board, id);
    shift_up_with_board(temp_board, n);
    return board_to_id(temp_board);
}

int id_shift_down(int id, int n){
    int temp_board[BOARD_SIZE][BOARD_SIZE];
    id_to_board(temp_board, id);
    shift_down_with_board(temp_board, n);
    return board_to_id(temp_board);
}

int graph[19810][6];

void init_graph(){

}

int visited[19810];
int path[3000];
int cnt;
int start,end;

void dfs_output(){
    printf("%d->\n ", start);
    // for(int i=1;i<cnt-1;i++)
    //     printf("%d->\n", path[i]);
    printf("%d\n-------------------------------------------------------------\n", end);
}

void dfs(int start_id, int dest_id, int step){
    // printf("searching between %d and %d\n", start_id, dest_id);
    if(start_id==dest_id){
        printf("found\n");
        cnt=step;
        printf("cnt:%d\n",cnt);
        dfs_output();
       // goto END;
        return;
    }
        
    
    for(int i=0;i<BOARD_SIZE;i++){
        for(int j=0;j<4;j++){
            int tempid;
            switch (j)
            {
            case 0:
                tempid=id_shift_left(start_id, i);
                break;
            case 1:
                tempid=id_shift_right(start_id, i);
                break;
            case 2:
                tempid=id_shift_up(start_id, i);
                break;
            case 3:
                tempid=id_shift_down(start_id, i);
                break;
            default:
                break;
            }
            if(visited[tempid]!=1){
                visited[tempid]=1;
                path[step]=tempid;
                dfs(tempid,dest_id,step+1);
                visited[tempid]=0;
            }
        }

        // dfs(id_shift_left(start_id, i),dest_id);
        // dfs(id_shift_right(start_id, i),dest_id);
        // dfs(id_shift_up(start_id, i),dest_id);
        // dfs(id_shift_down(start_id, i),dest_id);
    }
    return;
}

int head,tail;
int queue[20000]={0};

// void bfs(){
//     int i=0;
//     int next_id=0;
//     int flag=0;
//     head=0;
//     tail=0;
//     queue[tail]=start;

// }

int main(){
    int board[3][3]={{0,0,2},{1,0,1},{2,1,2}};
    // for(int i=0;i<3;i++)
    //     for(int j=0;j<3;j++)
    //         board[i][j]=j;

        
    // print_board_autosolve_test(board);
    // printf("%d\n",board_to_id(board));
    
    int testboard[3][3];

    id_to_board(testboard,17055);

    print_board_autosolve_test(testboard);
    start = 17055;
    end=19305;
    dfs(start, end, 1);
    return 0;
}