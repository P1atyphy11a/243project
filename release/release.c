#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_SIZE 4

int BOARD_SIZE;  

void init(int n);


void shuffle();

void shift_left(int n);
void shift_right(int n);
void shift_up(int n);
void shift_down(int n);

void print_board_debug();

void plot_pixel(int x, int y, short int color);

void draw_one_block(int x, int y, short int color, int width);

void draw_screen();

void draw_puzzle();

void clear_screen();

void draw_dest_board();

void get_input();

void shift(int sw,int press);
void disable_A9_interrupts(void);

void set_A9_IRQ_stack(void);

void config_GIC(void);

void config_KEYs(void);

void enable_A9_interrupts(void);

void pushbutton_ISR(void);

void config_interrupt(int, int);

#define destID 19305

int board_to_id(int (*board)[BOARD_SIZE]);

void id_to_board(int (*board)[BOARD_SIZE], int id);

void print_board_autosolve_test(int (*board)[BOARD_SIZE]);

void shift_left_with_board(int (*board)[BOARD_SIZE],int n);

void shift_right_with_board(int (*board)[BOARD_SIZE],int n);

void shift_up_with_board(int (*board)[BOARD_SIZE],int n);

void shift_down_with_board(int (*board)[BOARD_SIZE],int n);

int id_shift_right(int id, int n);

int id_shift_left(int id, int n);

int id_shift_up(int id, int n);
int id_shift_down(int id, int n);
void dfs_output();

void dfs(int start_id, int dest_id, int step);

void bfs(int start_id, int dest_id);

void autosolve();

int color[8]={0xF800, 0x07E0, 0x001F, 0xFFE0, 0xF81F, 0x07FF, 0xF4CE, 0x970F};  // colors will be used for the puzzle

short int board[MAX_SIZE][MAX_SIZE];                                                  //array to store the puzzle status, MAX_SIZE=8
short int dest_board[MAX_SIZE][MAX_SIZE];
int number_board[MAX_SIZE][MAX_SIZE];
                                                               //actural puzzle size, no bigger than MAX_SIZE

volatile int pixel_buffer_start;

bool AUTOS=true;

int main(){


    disable_A9_interrupts(); // disable interrupts in the A9 processor
    set_A9_IRQ_stack(); // initialize the stack pointer for IRQ mode
    config_GIC(); // configure the general interrupt controller
    config_KEYs(); // configure pushbutton KEYs to generate interrupts
    enable_A9_interrupts(); // enable interrupts in the A9 processor

    volatile int * pixel_ctrl_ptr = (int *)0xFF203020;

    pixel_buffer_start = *pixel_ctrl_ptr;



    init(3);
    print_board_debug();
    shuffle();
    print_board_debug();
    draw_screen();
    while(1);
    return 0;
}


int sw;
int press;
/* ********************************************************************************
* This program demonstrates use of interrupts with C code. The program
*responds
* to interrupts from the pushbutton KEY port in the FPGA.
*
* The interrupt service routine for the KEYs indicates which KEY has been
*pressed
* on the LED display.
********************************************************************************/
// int main(void) {
//     disable_A9_interrupts(); // disable interrupts in the A9 processor
//     set_A9_IRQ_stack(); // initialize the stack pointer for IRQ mode
//     config_GIC(); // configure the general interrupt controller
//     config_KEYs(); // configure pushbutton KEYs to generate interrupts
//     enable_A9_interrupts(); // enable interrupts in the A9 processor
//     while (1){
//       volatile int * sw_ptr = (int *) 0xFF200040; // SW base address
// 	    volatile int * led_ptr = (int *) 0xFF200000;
// 	    sw = *(sw_ptr);
// 	    *led_ptr = sw;
//     };
// }
/* setup the KEY interrupts in the FPGA */
void config_KEYs() {
    volatile int * KEY_ptr = (int *) 0xFF200050; // pushbutton KEY base address
    *(KEY_ptr + 2) = 0xF; // enable interrupts for the two KEYs
}
/* This file:
* 1. defines exception vectors for the A9 processor
* 2. provides code that sets the IRQ mode stack, and that dis/enables
* interrupts
* 3. provides code that initializes the generic interrupt controller
*/

// Define the IRQ exception handler
void __attribute__((interrupt)) __cs3_isr_irq(void) {
// Read the ICCIAR from the CPU Interface in the GIC
    volatile int * sw_ptr = (int *) 0xFF200040;

    int interrupt_ID = *((int *)0xFFFEC10C);
    if (interrupt_ID == 73)
        pushbutton_ISR();
    else
        while (1); // if unexpected, then stay here
    // Write to the End of Interrupt Register (ICCEOIR)
    *((int *)0xFFFEC110) = interrupt_ID;
}



// Define the remaining exception handlers
void __attribute__((interrupt)) __cs3_reset(void) {
    while (1);
}
void __attribute__((interrupt)) __cs3_isr_undef(void) {
    while (1);
}
void __attribute__((interrupt)) __cs3_isr_swi(void) {
    while (1);
}
void __attribute__((interrupt)) __cs3_isr_pabort(void) {
    while (1);
}
void __attribute__((interrupt)) __cs3_isr_dabort(void) {
    while (1);
}
void __attribute__((interrupt)) __cs3_isr_fiq(void) {
    while (1);
}
/*
* Turn off interrupts in the ARM processor
*/
void disable_A9_interrupts(void) {
    int status = 0b11010011;
    asm("msr cpsr, %[ps]" : : [ps] "r"(status));
}
/*
* Initialize the banked stack pointer register for IRQ mode
*/
void set_A9_IRQ_stack(void) {
    int stack, mode;
    stack = 0xFFFFFFFF - 7; // top of A9 onchip memory, aligned to 8 bytes
    /* change processor to IRQ mode with interrupts disabled */
    mode = 0b11010010;
    asm("msr cpsr, %[ps]" : : [ps] "r"(mode));
    /* set banked stack pointer */
    asm("mov sp, %[ps]" : : [ps] "r"(stack));
    /* go back to SVC mode before executing subroutine return! */
    mode = 0b11010011;
    asm("msr cpsr, %[ps]" : : [ps] "r"(mode));
}
/*
* Turn on interrupts in the ARM processor
*/
void enable_A9_interrupts(void) {
    int status = 0b01010011;
    asm("msr cpsr, %[ps]" : : [ps] "r"(status));
}
/*
* Configure the Generic Interrupt Controller (GIC)
*/
void config_GIC(void) {
    config_interrupt (73, 1); // configure the FPGA KEYs interrupt (73)
    // Set Interrupt Priority Mask Register (ICCPMR). Enable interrupts of all
    // priorities
    *((int *) 0xFFFEC104) = 0xFFFF;
    // Set CPU Interface Control Register (ICCICR). Enable signaling of
    // interrupts
    *((int *) 0xFFFEC100) = 1;
    // Configure the Distributor Control Register (ICDDCR) to send pending
    // interrupts to CPUs
    *((int *) 0xFFFED000) = 1;
}
/*
* Configure Set Enable Registers (ICDISERn) and Interrupt Processor Target
* Registers (ICDIPTRn). The default (reset) values are used for other registers
* in the GIC.
*/
void config_interrupt(int N, int CPU_target) {
    int reg_offset, index, value, address;
    /* Configure the Interrupt Set-Enable Registers (ICDISERn).
    * reg_offset = (integer_div(N / 32) * 4
    * value = 1 << (N mod 32) */
    reg_offset = (N >> 3) & 0xFFFFFFFC;
    index = N & 0x1F;
    value = 0x1 << index;
    address = 0xFFFED100 + reg_offset;
    /* Now that we know the register address and value, set the appropriate bit */
    *(int *)address |= value;
    /* Configure the Interrupt Processor Targets Register (ICDIPTRn)
    * reg_offset = integer_div(N / 4) * 4
    * index = N mod 4 */
    reg_offset = (N & 0xFFFFFFFC);
    index = N & 0x3;
    address = 0xFFFED800 + reg_offset + index;
    /* Now that we know the register address and value, write to (only) the
    * appropriate byte */
    *(char *)address = (char)CPU_target;
}
/********************************************************************
* Pushbutton - Interrupt Service Routine
*
* This routine checks which KEY has been pressed. It writes to HEX0
*******************************************************************/
void pushbutton_ISR(void) {
    /* KEY base address */
    volatile int * KEY_ptr = (int *) 0xFF200050;


    volatile int * sw_ptr = (int *) 0xFF200040;
    /* HEX display base address */
    volatile int * HEX3_HEX0_ptr = (int *) 0xFF200020;
    int HEX_bits;
    press = *(KEY_ptr + 3); // read the pushbutton interrupt register
    *(KEY_ptr + 3) = press; // Clear the interrupt
    if (press & 0x1) // KEY0
    HEX_bits = 0b00111111;
    else if (press & 0x2) // KEY1
    HEX_bits = 0b00000110;
    else if (press & 0x4) // KEY2
    HEX_bits = 0b01011011;
    else // press & 0x8, which is KEY3
    HEX_bits = 0b01001111;
    *HEX3_HEX0_ptr = HEX_bits;

    volatile int * led_ptr = (int *) 0xFF200000;
    sw = *(sw_ptr);
    *led_ptr = sw;

    shift(sw,press);

    return;
}

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

void plot_pixel(int x, int y, short int color){
    *(short int *)(pixel_buffer_start + (y << 10) + (x << 1)) = color;
}

void draw_one_block(int x, int y, short int color, int width){             //(x,y)is the left top pixel
    for(int i=x; i< x+width; i++)
      for(int j=y; j< y+width; j++)
         plot_pixel(i,j,color);
}

void draw_screen(){
    draw_puzzle();
    draw_dest_board();
}

void draw_puzzle(){
    clear_screen();
    for(int i=0; i<BOARD_SIZE; i++){                            //only draw the actural puzzle board
      for(int j=0; j<BOARD_SIZE; j++){
         short int block_color = board[i][j];
         draw_one_block(20+j*50*4/3,20+i*50*4/3,block_color, 50*4/3+1);
      }
    }
}

void clear_screen(){
    for(int i=0;i<320;i++)
        for(int j=0;j<240;j++)
            plot_pixel(i,j,0xFFFA);
}

void draw_dest_board(){
    for(int i=0; i<BOARD_SIZE; i++){
        for(int j=0;j<BOARD_SIZE; j++){
            short int block_color = dest_board[i][j];
            draw_one_block(255+j*15*4/3, 15+i*15*4/3, block_color, 15*4/3+1);
        }
    }
}

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

void shift(int sw,int press){
    int rowCol=sw>>9;  //1=col 0=row
    int num=sw&0b0000000111;
    bool cheat=((sw&0b0001010000)==(0b0001010000));
    if(cheat&&AUTOS){
        autosolve();
        cheat=false;
        return;
    }
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