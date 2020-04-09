#include "project.h"
#include "autosolver.h"
#include <stdbool.h>
#include <math.h>
#include <time.h>

extern int BOARD_SIZE;
extern int UUDDLLRRBABA[30][2];
extern bool AUTOS;

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
            printf("ID after sub:%d, count:%d\n",tempID, temp);
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

// int visited[19810];
// int prev_node[19810];
// int waveFront[19810];
// int pathPointer=0;
// int path[100];

int visited[19810];
int path[3000];
int cnt;
int start,end;
int flag=0;

void dfs_output(){
    printf("%d->\n ", start);
    for(int i=1;i<cnt-1;i++)
        printf("%d->\n", path[i]);
    printf("%d\n-------------------------------------------------------------\n", end);
}

void dfs(int start_id, int dest_id, int step){
    // printf("searching between %d and %d\n", start_id, dest_id);
    if(flag==-1)return;
    if(start_id==dest_id){
        printf("found\n");
        cnt=step;
        printf("cnt:%d\n",cnt);
        dfs_output();
        flag=-1;
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






// void bfs(int start_id, int dest_id){
//     prev_node[dest_id]=-1;
    


// }

void autosolve(){
    bool flag=false;
    for(int i=29;i>=0;i--){
        if(UUDDLLRRBABA[i+1][0]==-1)
            flag=true;
        if(!flag)continue;
        switch (UUDDLLRRBABA[i][0])
        {
        case 1:
            shift_left(UUDDLLRRBABA[i][0]);
            printf("shift_left %d\n", UUDDLLRRBABA[i][0]);
            break;

        case 0:
            shift_right(UUDDLLRRBABA[i][0]);
            printf("shift_right %d\n", UUDDLLRRBABA[i][0]);
            break;

        case 3:
            shift_up(UUDDLLRRBABA[i][0]);
            printf("shift_up %d\n", UUDDLLRRBABA[i][0]);
            break;

        case 2:
            shift_down(UUDDLLRRBABA[i][0]);
            printf("shift_down %d\n", UUDDLLRRBABA[i][0]);
            break;

        default:
            break;
        }
        draw_screen();
        for (int c = 1; c <= 32767; c++);
            // for (int d = 1; d <= 32767; d++);
    }
    AUTOS=false;
}