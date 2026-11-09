#include "fluid_bridge.h"
#include <fluid2d/fluid_solver.h>
#include <cstddef>

static fluid2d::FluidSolver solver;
static fluid2d::Config config;

// Working memory for the solver
alignas(std::max_align_t)
static uint8_t workspace[128 * 1024];

static bool initialized = false;

extern "C" long map(long x, long in_min, long in_max, long out_min, long out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

extern "C" bool Fluid_Init(void)
{
    const size_t bytes = fluid2d::FluidSolver::requiredMemory(config);

    if (bytes == 0 || bytes > sizeof(workspace)) return false;

    if (solver.initialize(config, workspace, bytes)
        != fluid2d::Status::Ok)
        return false;

    initialized = true;
    return true;
}

extern "C" void Fluid_Update(float gx, float gy, float dt)
{
    if (!initialized) return;

    solver.setGravity(gx, gy);
    solver.step(dt);
}

#define DISPLAY_DENSITY_THRESHOLD 0.50f

// Map simulation grid to pixel grid
extern "C" bool Fluid_GetPixel(uint8_t x, uint8_t y)
{
    if (!initialized || x >= 8 || y >= 8) return false;

    const auto density = solver.densityGrid();

    if (!density.values || density.rest_density <= 0.0f) return false;

    const size_t x0 = x * density.width / 8;
    const size_t x1 = (x + 1) * density.width / 8;
    const size_t y0 = y * density.height / 8;
    const size_t y1 = (y + 1) * density.height / 8;

    float total = 0;
    size_t count = 0;

    for (size_t i = x0; i < x1; i++) {
        for (size_t j = y0; j < y1; j++) {
            total += density.values[i * density.stride + j];
            count++;
        }
    }

    return count > 0 &&
           (total / count / density.rest_density) > DISPLAY_DENSITY_THRESHOLD;
}