#include "conway.h"
#include "ssd1306.h"
#include <stdio.h>

#define inc 2
#define enter 3

int main () {
    stdio_init_all();

    ssd1309Init();

    // gpio_init(inc);
    // gpio_init(enter);
    // gpio_set_dir(inc, GPIO_IN);
    // gpio_set_dir(enter, GPIO_IN);

    uint8_t buff [SIZE/8];
    
    struct conway b;

    uint32_t base;

    while (1) {
        uint32_t ind = 0;
        runBoard(&b);
        for (uint32_t i = 0; i < H/8; i++) {
            for (uint32_t j = 0; j < W; j++)
            {
                base = 8 * i * (W - 1) + j;
                buff[ind] = (b.out_board[base] << 7) + (b.out_board[base + W] << 6) + (b.out_board[base + W * 2] << 5) + (b.out_board[base + W * 3] << 4) + (b.out_board[base + W * 4] << 3) + (b.out_board[base + W * 5] << 2) + (b.out_board[base + W * 6] << 1) + b.out_board[base + W * 7];
                ind ++;
            }
        }
        sendData(buff, sizeof(buff));
        sleep_ms(10);
    }
        

}