
#include "max7219.h"
#include "main.h"

#define REG_DECODE_MODE 0x09
#define REG_INTENSITY 0x0A
#define REG_SCAN_LIMIT 0x0B
#define REG_NO_SHUTDOWN 0x0C
#define REG_DISPLAY_TEST 0x0F

#define CS_PORT SPI_CS_GPIO_Port
#define CS_PIN SPI_CS_Pin

static void write_registry(MAX7219 *device, uint8_t address, uint8_t value) {
    uint8_t data[2] = {address, value};

    HAL_GPIO_WritePin(CS_PORT, CS_PIN, false); // CS SET
    HAL_SPI_Transmit(device->spi, data, 2, 100); // 8+8 bit
    HAL_GPIO_WritePin(CS_PORT, CS_PIN, true); // CS UNSET
}

void MAX7219_Init(MAX7219 *device, SPI_HandleTypeDef *spi, uint8_t brightness) {
    device->spi = spi;

    for (uint8_t y = 0; y < 8; y++) {
        for (uint8_t x = 0; x < 8; x++) device->pixels[y][x] = false;
    }

    HAL_GPIO_WritePin(CS_PORT, CS_PIN, true); // CS UNSET

    write_registry(device, REG_DECODE_MODE, false);
    write_registry(device, REG_SCAN_LIMIT, 7);
    write_registry(device, REG_INTENSITY, brightness);
    write_registry(device, REG_DISPLAY_TEST, false);

    MAX7219_Update(device);

    write_registry(device, REG_NO_SHUTDOWN, true);
}

void MAX7219_SetPixel(MAX7219 *device, uint8_t x, uint8_t y, bool on) {
    if (!device || x >= 8 || y >= 8) return;
    device->pixels[y][x] = on;
}

bool MAX7219_GetPixel(const MAX7219 *device, uint8_t x, uint8_t y) {
    if (!device || x >= 8 || y >= 8) return false;
    return device->pixels[y][x];
}

void MAX7219_Clear(MAX7219 *device) {
    if (!device) return;

    for (uint8_t y = 0; y < 8; y++) {
        write_registry(device, y + 1, false);
        for(uint8_t x = 0; x < 8; x++) device->pixels[y][x] = false;
    }
}


void MAX7219_Update(MAX7219 *device)
{
  const uint8_t columnOffset[8] = {128, 64, 32, 16, 8, 4, 2, 1};
    // 8 bit per rad, legger til columnOffset dersom dioden i kolonnen for den aktuelle rad skal aktiveres
    // Eksempel: 128 + 32 + 8 + 2, vil lyse opp annenhver kolonne

    for (uint8_t y = 0; y < 8; y++) {
        uint8_t value = 0;

        for (uint8_t x = 0; x < 8; x++) {
            if (device->pixels[y][x]) {
                value += columnOffset[x];
            }
        }

        write_registry(device, y + 1, value); // første rad har adresse 0x01, array begynner på 0;
    }
}
