#include "kernel.hpp"
#include "memory.hpp"

extern "C" void
kmain()
{
    kmemset((uint8_t*)0x1000000 + 0x7C00, 0, 70000);
}
