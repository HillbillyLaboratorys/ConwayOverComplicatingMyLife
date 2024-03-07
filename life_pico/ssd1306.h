
//#include <string>
#include "pico/stdlib.h"
#include "hardware/spi.h"


void sendCommand(uint8_t* buffer, size_t size);

void sendData(uint8_t* buffer, size_t size);

void ssd1309Init ();