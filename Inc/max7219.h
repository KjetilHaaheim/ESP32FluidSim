/*
    Inspired by
    https://github.com/taburyak/STM32_MAX7219_SPI
    https://github.com/dariusur/MAX7219_library
    https://github.com/lamik/MAX7219_matrix_STM32_HAL
*/

#ifndef MAX7219_H
#define MAX7219_H

#include <stdbool.h>
#include "stm32f7xx_hal.h"

typedef struct {
    SPI_HandleTypeDef *spi;
    bool pixels[8][8];
} MAX7219;

void MAX7219_Init(
    MAX7219 *device,
    SPI_HandleTypeDef *spi,
    uint8_t brightness
);

void MAX7219_SetPixel(MAX7219 *device, uint8_t x, uint8_t y, bool on);
bool MAX7219_GetPixel(const MAX7219 *device, uint8_t x, uint8_t y);
void MAX7219_Clear(MAX7219 *device);
void MAX7219_Update(MAX7219 *device);

#endif
