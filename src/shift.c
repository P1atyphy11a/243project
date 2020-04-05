#include"project.h"

void shift(int sw,int press){
    int rowCol=sw>>9;  //1=col 0=row
    int num=sw&0b00000000111;
    num=(num>=3)?3:num;
    if(rowCol==1){         //col, shift up/down
        if(press==1||press==2)
            shift_down(num);  //key0,1 shift down
        else    
            shift_up(num);    //key 2,3 shift up
    }
    else{
        if(press==1||press==2)
            shift_right(num);  //key0,1 shift right
        else    
            shift_left(num);    //key 2,3 shift left
    }
    draw_screen();
}