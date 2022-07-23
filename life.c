#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

void clearBoard(int* sx, int* sy, bool board[*sy][*sx]) {

    for (int i=0; i < *sx; ++i) {
        for (int j=0; j < *sy; ++j) {
            board[j][i] = false;
        }
    }
}

bool inBounds(int* px, int* py, int* psx, int* psy) {

    if (*px >= 0 && *px < *psx && *py >= 0 && *py < *psy){
        return true;
    }
    else {
        return false;
    }
}  

int searchAround(int* sx, int* sy, bool outBoard[*sy][*sx], int* i, int* j) {

    int pac = 0;

    for (int c=0; c < 8; ++c){

        int chky;
        int chkx;

        switch(c){
            case 0:
                chky = *j - 1;
                chkx = *i - 1;
                break;
            case 1:
                chky = *j - 1;
                chkx = *i;
                break;
            case 2:
                chky = *j - 1;
                chkx = *i + 1;
                break;
            case 3:
                chky = *j;
                chkx = *i - 1;
                break;
            case 4:
                chky = *j;
                chkx = *i + 1;
                break;
            case 5:
                chky = *j + 1;
                chkx = *i - 1;
                break;
            case 6:
                chky = *j + 1;
                chkx = *i;
                break;
            case 7:
                chky = *j + 1;
                chkx = *i + 1;
                break;
        }
        /*
            123
            4#5
            678
        */
        if (inBounds(&chkx, &chky, sx, sy) && outBoard[chky][chkx] == true) {
            ++pac;
        }
    }
    return pac;
}

void plotBoard(int* sx, int* sy, bool outBoard[*sy][*sx]) {

   
    system("clear");    
    //printf("1 \n\n");

    for (int i=0; i < *sx; ++i) {

        printf("\n");

        for (int j=0; j < *sy; ++j) {

            if (outBoard[j][i] == true) {
                printf("# ");
            }
            else {
                printf("  ");
            }
        }
    }
}


void seedBoard(int* sx, int* sy, bool outBoard[*sy][*sx]) {

    clearBoard(*&sx, *&sy, outBoard);

    time_t t;
    srand((unsigned) time(&t));

    const int n = 200;
    int a, b;
    for(int i = 0 ; i < n ; i++ ) {
       
        a = rand() % *sy;
        b = rand() % *sx;  
        outBoard[a][b] = true;
    }
}

void doTheThing() {

    int sx = 25;
    int sy = 40;
    int sameCount = 0;
    int same = 0;
    int same1 = 0;
    int same2 = 0;
    int same3 = 0;

    int live;
   
    bool board[sy][sx];
    bool outBoard[sy][sx];

    clearBoard(&sx, &sy, outBoard);
    clearBoard(&sx, &sy, board);

    seedBoard(&sx, &sy, outBoard);

    /*
    board[8][11] = true;
    board[8][10] = true;
    board[9][11] = true;
    board[9][10] = true;
    board[10][9] = true;
    board[10][8] = true;
    board[11][9] = true;
    board[11][8] = true;
    */
   
    while (true) {

        live = 0;

        for (int i=0; i < sx; ++i) {
            for (int j=0; j < sy; ++j) {
               
                bool cell = outBoard[j][i];
                int pac = searchAround(&sx, &sy, outBoard, &i, &j);

                if(outBoard[j][i] == true) {

                    ++live;
                }

                if (cell == true && pac > 3 || cell == true && pac < 2) {
                   
                    board[j][i] = false;
                }

                if (cell == false && pac == 3){

                    board[j][i] = true;
                }

                if (cell == true && pac > 1 && pac < 4){

                    board[j][i] = true;
                }
            }
        }
               
        same = live;

        if (same == same2 || same == same3) {

            ++sameCount;
        }

        if (sameCount > 50) {

            seedBoard(&sx, &sy, board);
            sameCount = 0;
            same = 0;
            same1 = 0;
            same2 = 0;
            same3 = 0;
        }

        same3 = same2;
        same2 = same1;
        same1 = same;
        //printf("%d %d %d %d ", same, same1, same2, same3);
        printf("%d", sameCount);

        for (int i=0; i < sx; ++i) {
            for (int j=0; j < sy; ++j) {

                outBoard[j][i] = board[j][i];

            }
        }

        //clearBoard(&sx, &sy, board);

        plotBoard(&sx, &sy, outBoard);

        printf("\n");

        usleep(75000);
        //sleep(1);
    }
}

int main() {

    doTheThing();
   
    return 0;
}
