#include "mma8451.h"
#include "stm32f7xx_hal_i2c.h"

#define MMA8451_ADDRESS (0x1D << 1) // 8-bit I2C Slave Address (0x3A)
#define MMA8451_WHO_AM_I_REG 0x0D // WHO_AM_I Register: expected value 0x1A
#define MMA8451_CTRL_REG1 0x2A // Control Register 1
#define MMA8451_XYZ_DATA_CFG 0x0E // Data Configuration Register
#define MMA8451_OUT_X_MSB 0x01 // First data register for X-axis MSB

uint8_t RxData[6]; // Buffer to store X, Y, Z data (2 bytes per axis)


void MMA8451_Init(I2C_HandleTypeDef *device)
{
    uint8_t Data;
    // 1. Standby Mode: Clear Active bit (Bit 0) in CTRL_REG1
    Data = 0x00;
    HAL_I2C_Mem_Write(device, MMA8451_ADDRESS, MMA8451_CTRL_REG1, 1, &Data, 1, HAL_MAX_DELAY);

    // 2. Configure: Set Scale (2g range) in XYZ_DATA_CFG
    Data = 0x00; // Full Scale 2g (bits Fs1:Fs0 = 00)
    HAL_I2C_Mem_Write(device, MMA8451_ADDRESS, MMA8451_XYZ_DATA_CFG, 1, &Data, 1, HAL_MAX_DELAY);

    // 3. Active Mode: Set Active bit (Bit 0) in CTRL_REG1
    Data = 0x01; // ODR=800Hz (default), Active Mode (Bit 0 = 1)
    HAL_I2C_Mem_Write(device, MMA8451_ADDRESS, MMA8451_CTRL_REG1, 1, &Data, 1, HAL_MAX_DELAY);
 }

 AccelerometerReading MMA8451_read(I2C_HandleTypeDef *device) {
    // 1. Read 6 bytes of acceleration data (X MSB, X LSB, Y MSB, Y LSB, Z MSB, Z LSB)
    // Start reading from OUT_X_MSB (0x01)
    HAL_I2C_Mem_Read(device, MMA8451_ADDRESS, MMA8451_OUT_X_MSB, 1, RxData, 6, HAL_MAX_DELAY);

    // 2. Process the 14-bit data. The data is Left-Justified.
    // We need to shift the MSB left by 8 and combine with the LSB, then right-shift by 2 to align.
    return (AccelerometerReading) {
        .x = (int16_t)((RxData[0] << 8) | RxData[1]) >> 2,
        .y = (int16_t)((RxData[2] << 8) | RxData[3]) >> 2,
        .z = (int16_t)((RxData[4] << 8) | RxData[5]) >> 2
    };
}