#include "kernel.hpp"
#include "memory.hpp"

extern "C" void
kmain()
{
    kmemset((uint8_t*)0x1000000 + 0x7C00, 0, 70000);
    
    ((uint8_t*)VbeModeInfo.framebuffer)[0] = 0xFF;
    ((uint8_t*)VbeModeInfo.framebuffer)[1] = 0xFF;
    ((uint8_t*)VbeModeInfo.framebuffer)[2] = 0xFF;
}
