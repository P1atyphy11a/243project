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

int prev_node[19810];
int visited_bfs[19810];
int queue[20000];
int back=0;
int path_back=0;

int path_queue[200];

int pop_queue(){
    int temp=queue[0];
    for(int i=0;i<20000-1;i++)
        queue[i]=queue[i+1];
    back--;
    return temp;
}

void push_back(int n){
    queue[back]=n;
    back++;
    return;
} 

void push_back_path(int n){
    printf("Push_back_path(%d)\n",n);
    path_queue[path_back]=n;
    path_back++;
    return;
} 

void bfs(int start_id, int dest_id){
    prev_node[dest_id]=dest_id;
    push_back(start_id);
    while(back!=0){
        int curnode=pop_queue();
        printf("current Node:%d\n",curnode);
        if(curnode==dest_id){
            printf("found\n");
            return;
        }
        for(int i=0;i<3;i++){
            for(int j=0;j<4;j++){
                int temp;
                switch(j){
                    case 0:
                        temp=id_shift_up(curnode,i);
                        if(visited_bfs[temp]!=1){
                            push_back(temp);
                            prev_node[temp]=curnode;
                        }
                        
                        break;
                    case 1:
                        temp=id_shift_down(curnode,i);
                        if(visited_bfs[temp]!=1){
                            push_back(temp);
                            prev_node[temp]=curnode;
                        }
                        break;
                    case 2:
                        temp=id_shift_left(curnode,i);
                        if(visited_bfs[temp]!=1){
                            push_back(temp);
                            prev_node[temp]=curnode;
                        }
                        break;
                    case 3:
                        temp=id_shift_right(curnode,i);
                        if(visited_bfs[temp]!=1){
                            push_back(temp);
                            prev_node[temp]=curnode;
                        }
                        break;
                }
            }
            // push_back(id_shift_up(curnode,i));
            // push_back(id_shift_down(curnode,i));
            // push_back(id_shift_left(curnode,i));
            // push_back(id_shift_up(curnode,i));
        }
        visited_bfs[curnode]=1;
    }

    printf("do not found\n");
}

void bfs_traceback(int start_id, int dest_id){
    path_back=0;
    int curr=start_id;
    
    printf("Start_id:%d\nPrev_Node:%d\n",curr,prev_node[curr]);
    
    while(prev_node[curr]!=curr){
        printf("Current_Node:%d\nPrev_Node:%d\nPath_back:%d\n",curr,prev_node[curr],path_back);
        push_back_path(curr);
        curr=prev_node[curr];
    }
    push_back_path(dest_id);
    return;
}

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
    int start = 17055;
    int end=19305;
    bfs(end,start);
    bfs_traceback(start,end);
    printf("\n--------------------------------------------------\nPath_back:%d\n",path_back);
    for(int i=0;i<path_back;i++)
        printf("%d -> ",path_queue[i]);

    return 0;
}