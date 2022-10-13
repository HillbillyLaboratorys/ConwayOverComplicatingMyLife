#include <stdio.h>
#include <Windows.h>
#include <time.h>
#include <stdlib.h>

#define W 60
#define H 60
#define SIZE (W * H)
#define SEEDNUM 1500
#define oSize (((W * 2) + 1) * H)

#define WW (W * 2)
#define WH H

HANDLE wHnd;    // Handle to write to the console.
HANDLE rHnd;    // Handle to read from the console.

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


int main() {
   
        
    // Set up the handles for reading/writing:
    wHnd = GetStdHandle(STD_OUTPUT_HANDLE);
    rHnd = GetStdHandle(STD_INPUT_HANDLE);

    // Change the window title:
    SetConsoleTitle(TEXT("Let There Be Life"));

    // Set up the required window size:
    SMALL_RECT windowSize = { 0, 0, WW - 1, WH - 1};

    // Change the console window size:
    SetConsoleWindowInfo(wHnd, TRUE, &windowSize);

    // Create a COORD to hold the buffer size:
    COORD bufferSize = { WW, WH + 1};

    // Change the internal buffer size:
    SetConsoleScreenBufferSize(wHnd, bufferSize);

    // Set up the character buffer:
    CHAR_INFO consoleBuffer[WW * WH + WW];

    int around = 0;
    int current;

    int out_board[SIZE];
    int comp_board[SIZE];
    int* out_bp = out_board;
    int* comp_bp = comp_board;

    seedBoard(SIZE, comp_bp);
    //clearBoard(SIZE, comp_bp);
    //plotBoard(SIZE, comp_bp, W, H);

    int live;
    
    int same1 = 0;
    int same2 = 0;
    int same3 = 0;
    int same4 = 0;
    int count = 0;

    while (1) {

        live = 0;

        for (int i = 0; i < SIZE; ++i) {

            current = comp_board[i];
            around = neighbors(SIZE, comp_bp, i, W, H);
            //printf(" around: %d ", around);

            if (current == 1 && (around > 3 || around < 2)) {

                out_board[i] = 0;
            }

            else if (current == 0 && around == 3) {

                out_board[i] = 1;
                live++;
            }

            else if (current == 1 && around > 1 && around < 4) {

                out_board[i] = 1;
                live++;
            }
            else {
                out_board[i] = 0;
            }
        }

        if (live == same1 || live == same2 || live == same3|| live == same4) {
            count++;
        }
        
        if (count > 100) {
            seedBoard(SIZE, out_board);
            count = 0;
        }
        same4 = same3;
        same3 = same2;
        same2 = same1;
        same1 = live;
      
        CHAR_INFO hash;
        hash.Char.AsciiChar = '#';
        hash.Attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;;

        CHAR_INFO space;
        space.Char.AsciiChar = ' ';
        space.Attributes = 0;

        int j = 0;
        for (int i = 0; i < SIZE; ++i, ++j) {

            //if (i % W == 0 && i != 0) {
            //    //printf("\n");
            //    consoleBuffer[i].Char.AsciiChar = '\n';
            //    
            //}
            if (out_board[i] == 1) {
                //printf(" #");
                consoleBuffer[j] = hash;
                consoleBuffer[j + 1] = space;
                j++;
            }
            if (out_board[i] == 0) {
                //printf("  ");
                consoleBuffer[j] = space;
                consoleBuffer[j + 1] = space;
                j++;
            }

        }
        
        // Set up the positions:
        COORD charBufSize = { WW,WH + 1};
        COORD characterPos = { 0,0 };
        SMALL_RECT writeArea = { 0,0,WW-1,WH };

        // Write the characters:
        WriteConsoleOutputA(wHnd, consoleBuffer, charBufSize, characterPos, &writeArea);

        //printf("%d", count);
        /*
        system("cls");
        plotBoard(SIZE, out_bp, W, H);
        */
        switchBoards(SIZE, out_bp, comp_bp);
        
        Sleep(100);
        
    }
    return 0;
}
