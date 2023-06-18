#include "kernel.hpp"
#include "memory.hpp"
#include "debug.hpp"

static void 
fill_colorful_bitmap_vesa(uint8_t *destination, size_t count)
{
    for (size_t i = 0; i < count; i++) {
        size_t offset = i * 3;

        destination[offset+0] = (uint8_t)i;
        destination[offset+1] = (255 - (uint8_t)i);
        destination[offset+2] = (uint8_t)(i/1366);
    }
}

extern "C" void
kmain()
{
    kalloc_init();

    fill_colorful_bitmap_vesa((uint8_t*)VbeModeInfo.framebuffer, VbeModeInfo.width * VbeModeInfo.height);
}

