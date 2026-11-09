// Shim mellom C og C++

#ifndef FLUID_BRIDGE_H
#define FLUID_BRIDGE_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

bool Fluid_Init(void);
void Fluid_Update(float gx, float gy, float dt);
bool Fluid_GetPixel(uint8_t x, uint8_t y);

#ifdef __cplusplus
}
#endif

#endif