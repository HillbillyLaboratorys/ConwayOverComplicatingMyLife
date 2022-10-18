#include <stdio.h>
#include <Windows.h>
#include <time.h>
#include <stdlib.h>

// Width and hight of boards, Window size is (W*2) by H  
#define W 60
#define H 40
#define SIZE (W * H)
#define SEEDNUM (SIZE/4)

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

int inBounds(int px, int py) {

    if (px > -1 && px < W && py > -1 && py < H) {
        return 1;
    }
    else {
        return 0;
    }
}

int neighbors(const int size, int* board, int pos) {

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

    // Set up the positions:
    COORD charBufSize = { WW,WH + 1};
    COORD characterPos = { 0,0 };
    SMALL_RECT writeArea = { 0,0,WW-1,WH };


    int around = 0;
    int current;

    int out_board[SIZE];
    int comp_board[SIZE];
   
    seedBoard(SIZE, comp_board);
   
    int live;
    
    int same1 = 0;
    int same2 = 0;
    int same3 = 0;
    int same4 = 0;
    int count = 0;

    while (1) {

        live = 0;

        // Apply Survival and birth rules
        for (int i = 0; i < SIZE; ++i) {

            current = comp_board[i];
            around = neighbors(SIZE, comp_board, i);
          
            if (current == 0 && around == 3){

                out_board[i] = 1;
                live++;
            }

            else if (current == 1 &&  around > 1 && around < 4){

                out_board[i] = 1;
                live++;
            }
            else {
                out_board[i] = 0;
            }
        }
        // Track similar boards to trigger re-seed
        if (live == same1 || live == same2 || live == same3|| live == same4) {
            count++;
        }
     
        if (count > 50) {
            seedBoard(SIZE, out_board);
            count = 0;
        }
        same4 = same3;
        same3 = same2;
        same2 = same1;
        same1 = live;
      
       // Define attributes of characters to be written to buffer:
        CHAR_INFO hash;
        hash.Char.AsciiChar = '#';
        hash.Attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;;

        CHAR_INFO space;
        space.Char.AsciiChar = ' ';
        space.Attributes = 0;

        // Write data from board to buffer: 
        int j = 0;
        for (int i = 0; i < SIZE; ++i, j+=2) {

            if (out_board[i] == 1) {
                consoleBuffer[j] = hash;
                consoleBuffer[j + 1] = space;
            }
            if (out_board[i] == 0) {
                consoleBuffer[j] = space;
                consoleBuffer[j + 1] = space;
            }
        }      


        // Write the buffer:
        WriteConsoleOutputA(wHnd, consoleBuffer, charBufSize, characterPos, &writeArea);


        switchBoards(SIZE, out_board, comp_board);
        
        Sleep(100);
        
    }
    return 0;
}