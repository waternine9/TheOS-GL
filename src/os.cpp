#include "kernel.hpp"
#include "memory.hpp"
#include "debug.hpp"

static void 
fill_colorful_bitmap_u32(uint32_t *destination, size_t count)
{
    uint8_t *destination_rgba = (uint8_t*)destination;

    for (size_t i = 0; i < count*4; i++) {
        destination_rgba[i] = (uint8_t)i;
    }
}

extern "C" void
kmain()
{
    PROBE_VALUE(0xABCDEF);
    fill_colorful_bitmap_u32((uint32_t*)VbeModeInfo.framebuffer, 10000);
}
