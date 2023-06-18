#include "kernel.hpp"
#include "memory.hpp"
#include "debug.hpp"

static void 
fill_colorful_bitmap_u32(uint32_t *destination, size_t count)
{
    const size_t destination_byte_count = count*3;
    uint8_t *destination_rgba = (uint8_t*)destination;

    for (size_t i = 0; i < destination_byte_count; i += 3) {
        destination_rgba[i+0] = (uint8_t)i;
        destination_rgba[i+1] = (255 - (uint8_t)i);
        destination_rgba[i+2] = (uint8_t)(i/1366);
    }
}

extern "C" void
kmain()
{
    kalloc_init();
    void *pointer_1 = kmalloc(135);
    kfree(kmalloc(234));
    kfree(pointer_1);

    void *pointer_3 = kmalloc(144);
    void *pointer_4 = kmalloc(144);
    PROBE_VALUE((size_t)pointer_4);
    PROBE_VALUE((size_t)pointer_3);

    fill_colorful_bitmap_u32((uint32_t*)VbeModeInfo.framebuffer, VbeModeInfo.width * VbeModeInfo.height);
}

