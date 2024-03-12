#include <time.h>
#include <stdlib.h>

// Width and hight of boards, Window size is (W*2) by H  
#define W 128
#define H 64
#define SIZE (W * H)
#define SEEDNUM (SIZE/6)

#define WW (W * 2)
#define WH H

struct conway
{
    int out_board[SIZE];
    int comp_board[SIZE];
    int live;
    
    int same1;
    int same2;
    int same3;
    int same4;
    int count;

    int around;
    int current;
};


static void clearBoard(const int size, int* board);

void seedBoard(const int size, int* out_board);

static int inBounds(int px, int py);

static int neighbors(const int size, int* board, int pos);

static void switchBoards(const int size, int* out_board, int* board);

void runBoard(struct conway* data);