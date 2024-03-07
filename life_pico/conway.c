#include "conway.h"


static void clearBoard(const int size, int* board) {

    for (int i = 0; i < size; ++i) {

        board[i] = 0;
    }

}

static void seedBoard(const int size, int* out_board) {

    clearBoard(size, out_board);

    time_t t;
    srand((unsigned)time(&t));

    int a;
    for (int i = 0; i < SEEDNUM; ++i) {

        a = rand() % size;
        out_board[a] = 1;
    }
}

static int inBounds(int px, int py) {

    if (px > -1 && px < W && py > -1 && py < H) {
        return 1;
    }
    else {
        return 0;
    }
}

static int neighbors(const int size, int* board, int pos) {

    int count = 0;
    int cy = pos / W;
    int cx = pos % W;
    int ccx = 0;
    int ccy = 0;
    int pos1;

    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {

            ccx = cx + i;
            ccy = cy + j;

            if (inBounds(ccx, ccy) && !(ccx == cx && ccy == cy)) {

                pos1 = (ccy * (W)) + (ccx);

                if (board[pos1] == 1) {
                    ++count;
                }
            }
        }
    }

    return count;
}

static void switchBoards(const int size, int* out_board, int* board) {

    for (int i = 0; i < size; ++i) {

        board[i] = out_board[i];
    }
}

void runBoard(struct conway* data) {

    //seedBoard(SIZE, data->comp_board);
   
        
    // Apply Survival and birth rules
    for (int i = 0; i < SIZE; ++i) {

        data->current = data->comp_board[i];
        data->around = neighbors(SIZE, data->comp_board, i);
        
        if (data->current == 0 && data->around == 3){

            data->out_board[i] = 1;
            data->live++;
        }

        else if (data->current == 1 &&  data->around > 1 && data->around < 4){

            data->out_board[i] = 1;
            data->live++;
        }
        else {
            data->out_board[i] = 0;
        }
    }
    // Track similar boards to trigger re-seed
    if (data->live == data->same1 || data->live == data->same2 || data->live == data->same3|| data->live == data->same4) {
        data->count++;
    }
    
    if (data->count > 50) {
        seedBoard(SIZE, data->out_board);
        data->count = 0;
    }
    data->same4 = data->same3;
    data->same3 = data->same2;
    data->same2 = data->same1;
    data->same1 = data->live;
      
}