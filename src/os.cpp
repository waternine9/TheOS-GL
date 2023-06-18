#include "kernel.hpp"

void kmain()
{
    kmemset((void*)0x1000000 + 0x7C00, 0, 70000);
}