#include <stdio.h>
#include <Windows.h>
#include <time.h>
#include <stdlib.h>

void clearBoard(const int* size, int board[*size]){

    for (int i = 0; i < *size; ++i){

        board[i] = 0;
    }

}

void seedBoard(const int* size, int out_board[*size]) {

    clearBoard(*&size, out_board);

    time_t t;
    srand((unsigned) time(&t));

    const int n = 150;
    int a;
    for(int i = 0 ; i < n ; ++i) {
       
        a = rand() % *size;
        out_board[a] = 1;
    }
}

int inBounds(int* px, int* py, const int* mx, const int* my) {

     if (*px > -1 && *px < *mx && *py > -1 && *py < *my) {
        return 1;
    }
    else {
        return 0;
    }
}  

int neighbors(const int* size, int board[*size], int* pos, const int* W, const int* H) {

    int count = 0;
    int cy = (*pos) / (*W);
    int cx = (*pos) % (*W);
    int ccx = 0;
    int ccy = 0;
    int pos1;

    for(int i = -1 ; i < 2 ; i++) {
        for(int j = -1 ; j < 2 ; j++) {
            
            ccx = cx + i;
            ccy = cy + j;

            if (inBounds(&ccx, &ccy, *&W, *&H) && !(ccx == cx && ccy == cy)) {

                pos1 = (ccy * (*W)) + (ccx);
                //printf("  %d i%d,", pos1, *pos);
                if (board[pos1] == 1){
                    ++count;
                }
            }
        }
    }
    
    return count;
}

void switchBoards(const int* size, int out_board[*size], int board[*size]){

    for(int i = 0; i < *size; ++i) {

        board[i] = out_board[i];
    }
}

void plotBoard(const int* size, int board[*size], const int* W, const int* H){

    const int oSize = (((*W) * 2) + 1) * (*H);
    char out[oSize];

    for (int i = 0; i < size; ++i) {

        if (i % *W == 0) {
            out[i] = '\n';
            ++i;
        }
        if (board[i] == 1) {
            
            out[i] = ' ';
            out[i + 1] = '#';
        }
        if (board[i] == 0) {
            
            out[i] = ' ';
            out[i + 1] = ' ';
        }
    }
    printf("%s", out);
}

int main(){

    const int W = 20;
    const int H = 20;
    const int SIZE = W * H;
    int around = 0;
    int current;

    int out_board[SIZE];
    int comp_board[SIZE];

    seedBoard(&SIZE, comp_board);

    while (1) {
 
        for (int i = 0; i < SIZE; ++i){

            current = comp_board[i];
            around = neighbors(&SIZE, comp_board, &i, &W, &H);
            //printf(" around: %d ", around);

            if (current == 1 && around > 3 || current == 1 && around < 2) {
                    
                out_board[i] = 0;
            }

            if (current == 0 && around == 3){

                out_board[i] = 1;
            }

            if (current == 1 && around > 1 && around < 4){

                out_board[i] = 1;
            }
            else {
                out_board[i] = 0;
            }
        }

        system("cls");
        plotBoard(&SIZE, out_board, &W, &H); 

        switchBoards(&SIZE, out_board, comp_board);

        Sleep(1000);   

        }
    return 0;
}