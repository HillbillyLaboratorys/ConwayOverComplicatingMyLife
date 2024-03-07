#include "ssd1306.h"

#define CD_PIN 20
#define RST_PIN 21

#define PWR_ON 0xAF
#define PWR_OFF 0xAE

#define BAUD 1000000

static inline void cs_select() {
    asm volatile("nop \n nop \n nop");
    gpio_put(PICO_DEFAULT_SPI_CSN_PIN, 0);  // Active low
    asm volatile("nop \n nop \n nop");
}

static inline void cs_deselect() {
    asm volatile("nop \n nop \n nop");
    gpio_put(PICO_DEFAULT_SPI_CSN_PIN, 1);
    asm volatile("nop \n nop \n nop");
}

// command low
void sendCommand(uint8_t* buffer, size_t size) {
    cs_select();
    gpio_put(CD_PIN, 0);
    spi_write_blocking(spi0, buffer, size);
    cs_deselect();
}

void sendData(uint8_t* buffer, size_t size) {
    cs_select();
    gpio_put(CD_PIN, 1);
    spi_write_blocking(spi0, buffer, size);
    cs_deselect();
}

void ssd1309Init () {
    spi_init(spi0, BAUD);

    gpio_init(CD_PIN);
    gpio_init(RST_PIN);
    gpio_set_dir(CD_PIN, GPIO_OUT);
    gpio_set_dir(RST_PIN, GPIO_OUT);

    gpio_put(RST_PIN, 0);
    sleep_ms(10);
    gpio_put(RST_PIN, 1);

    gpio_set_function(PICO_DEFAULT_SPI_SCK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PICO_DEFAULT_SPI_TX_PIN, GPIO_FUNC_SPI);

    // Make the SPI pins available to picotool
    //bi_decl(bi_2pins_with_func(PICO_DEFAULT_SPI_TX_PIN, PICO_DEFAULT_SPI_SCK_PIN, GPIO_FUNC_SPI));

    // Chip select is active-low, so we'll initialise it to a driven-high state
    gpio_init(PICO_DEFAULT_SPI_CSN_PIN);
    gpio_set_dir(PICO_DEFAULT_SPI_CSN_PIN, GPIO_OUT);
    gpio_put(PICO_DEFAULT_SPI_CSN_PIN, 1);


    // Defaults
    // 0xA8 Multiplex ratio 64 0X3F
    // 0xD5 Clock divider 0x80
    // Page address mode high low column 0x00 0X20

    // display off, set mem addressing mode, mode: horizontal, set contrast, max contrast, inversion off, offset, 0 offset, precharge period, 34, comHW config, config 7, vcomH level, 40, enable ram output, display on
    uint8_t initCommands [] = {
        0xAE, // diplay off
        0x20,0x00, //address mode horizontal
        0x21,0x00,0x7F, // set coloumn address 0 - 127
        0x22,0x00,0x07, // set page address 0 - 7
        0x81,0xFF, // set contrast max
        0xA6, // nonInverted
        0xD3,0x00, // Display Offset 
        0xD9,0x22, // PreCharge Period 34
        0XDA,0x12, // HWconfig 7
        0xDB,0x40, // vcomH level 
        0xA5, // display entire on
        0xA4, // disable entire on mode off
        0xAF // display on
        };

    sendCommand(initCommands, sizeof(initCommands));
    
}


