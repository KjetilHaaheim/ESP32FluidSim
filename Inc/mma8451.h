#ifndef MMA8451_H
#define MMA8451_H

#include <stdint.h>
#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_i2c.h"

typedef struct {
    int16_t x;
    int16_t y;
    int16_t z;
} AccelerometerReading;

void MMA8451_Init(I2C_HandleTypeDef *device);

AccelerometerReading MMA8451_read(I2C_HandleTypeDef *device);

#endif