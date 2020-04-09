extern BOARD_SIZE;

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