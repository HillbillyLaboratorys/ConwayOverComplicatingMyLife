#include <stdio.h>
#include <Windows.h>
#include <time.h>
#include <stdlib.h>

#define W 40
#define H 40
#define SEEDNUM 400
#define SIZE (W * H)
#define oSize (((W * 2) + 1) * H)

void clearBoard(const int size, int* board) {

    for (int i = 0; i < size; ++i) {

        board[i] = 0;
    }

}

void seedBoard(const int size, int* out_board) {

    clearBoard(size, out_board);

    time_t t;
    srand((unsigned)time(&t));

    int a;
    for (int i = 0; i < SEEDNUM; ++i) {

        a = rand() % size;
        out_board[a] = 1;
    }
}

int inBounds(int px, int py, const int mx, const int my) {

    if (px > -1 && px < mx && py > -1 && py < my) {
        return 1;
    }
    else {
        return 0;
    }
}

int neighbors(const int size, int* board, int pos, const int wi, const int hi) {

    int count = 0;
    int cy = pos / wi;
    int cx = pos % hi;
    int ccx = 0;
    int ccy = 0;
    int pos1;

    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {

            ccx = cx + i;
            ccy = cy + j;

            if (inBounds(ccx, ccy, wi, hi) && !(ccx == cx && ccy == cy)) {

                pos1 = (ccy * (wi)) + (ccx);
                //printf("  %d i%d,", pos1, *pos);
                if (board[pos1] == 1) {
                    ++count;
                }
            }
        }
    }

    return count;
}

void switchBoards(const int size, int* out_board, int* board) {

    for (int i = 0; i < size; ++i) {

        board[i] = out_board[i];
    }
}

void plotBoard(const int size, int* board, const int wi, const int hi) {

    //char out[oSize];

    //for (int i = 0; i < size; ++i) {

    //    if (i % wi == 0) {
    //        out[i] = '\n';
    //        ++i;
    //    }
    //    if (board[i] == 1) {

    //        out[i] = ' ';
    //        out[i + 1] = '#';
    //    }
    //    if (board[i] == 0) {

    //        out[i] = ' ';
    //        out[i + 1] = ' ';
    //    }
    //}
    //printf("%s", out);

    for (int i = 0; i < size; ++i) {

        if (i % wi == 0 && i != 0) {
            printf("\n");
        }
        if (board[i] == 1) {
            printf(" #");
        }
        if (board[i] == 0) {
            printf("  ");
        }

    }
}


int main() {

      
    int around = 0;
    int current;

    int out_board[SIZE];
    int comp_board[SIZE];
    int* out_bp = out_board;
    int* comp_bp = comp_board;

    seedBoard(SIZE, comp_bp);
    //clearBoard(SIZE, comp_bp);
    //plotBoard(SIZE, comp_bp, W, H);

    while (1) {

        for (int i = 0; i < SIZE; ++i) {

            current = comp_board[i];
            around = neighbors(SIZE, comp_bp, i, W, H);
            //printf(" around: %d ", around);

            if (current == 1 && (around > 3 || around < 2)) {

                out_board[i] = 0;
            }

            else if (current == 0 && around == 3) {

                out_board[i] = 1;
            }

            else if (current == 1 && around > 1 && around < 4) {

                out_board[i] = 1;
            }
            else {
                out_board[i] = 0;
            }
        }

        system("cls");
        plotBoard(SIZE, out_bp, W, H);

        switchBoards(SIZE, out_bp, comp_bp);

        Sleep(50);

    }
    return 0;
}
