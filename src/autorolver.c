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

int pair[2];

void get_direction(int id1, int id2){
    for(int i=0;i<3;i++){
        for(int j=0;j<4;j++){
            int temp;
            switch(j){
                case 0:
                    temp=id_shift_up(id1,i);     
                    if(temp==id2){
                        pair[0]=i;
                        pair[1]=j;
                        return;
                    }                   
                    break;
                case 1:
                    temp=id_shift_down(id1,i);            
                    if(temp==id2){
                        pair[0]=i;
                        pair[1]=j;
                        return;
                    }                
                    break;
                case 2:
                    temp=id_shift_left(id1,i);           
                    if(temp==id2){
                        pair[0]=i;
                        pair[1]=j;
                        return;
                    }                 
                    break;
                case 3:
                    temp=id_shift_right(id1,i);
                    if(temp==id2){
                        pair[0]=i;
                        pair[1]=j;
                        return;
                    }    
                    break;
            }
        }
    }
}

extern int number_board[MAX_SIZE][MAX_SIZE];

void autosolve_bfs(){
    bfs(board_to_id(number_board),destID);
    bfs_traceback(board_to_id(number_board),destID);
    for(int i=0;i<path_back-1;i++){
        get_direction(path_queue[i],path_queue[i+1]);
        switch (pair[1]){
        case 0:
            shift_up(pair[0]);
            draw_screen();
            break;
        case 1:
            shift_down(pair[0]);
            draw_screen();
            break;
        case 2:
            shift_left(pair[0]);
            draw_screen();
            break;
        case 3:
            shift_right(pair[0]);
            draw_screen();
            break;
        default:
            break;
        }
    }
}