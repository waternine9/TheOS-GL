#include "pic.hpp"

/* PIT Timer */
extern "C" void CHandlerIRQ0()
{
    PIC_EndOfInterrupt(0);
}
/* Keyboard Interrupt */
extern "C" void CHandlerIRQ1()
{
    PIC_EndOfInterrupt(1);
}
/* Channel for Secondary PIC, don't use. */
extern "C" void CHandlerIRQ2()
{
    PIC_EndOfInterrupt(2);
}
/* COM2 */
extern "C" void CHandlerIRQ3()
{
    PIC_EndOfInterrupt(3);
}
/* COM1 */
extern "C" void CHandlerIRQ4()
{
    PIC_EndOfInterrupt(4);
}
/* LPT2 */
extern "C" void CHandlerIRQ5()
{
    PIC_EndOfInterrupt(5);
}
/* Floppy Disk */
extern "C" void CHandlerIRQ6()
{
    PIC_EndOfInterrupt(6);
}
/* LPT1 (spurious) */
extern "C" void CHandlerIRQ7()
{
    PIC_EndOfInterrupt(7);
}
/* CMOS Real time clock */
extern "C" void CHandlerIRQ8()
{
    PIC_EndOfInterrupt(8);
}
/* Free for peripherals */
extern "C" void CHandlerIRQ9()
{
    PIC_EndOfInterrupt(9);
}
/* Free for peripherals */
extern "C" void CHandlerIRQ10()
{
    PIC_EndOfInterrupt(10);
}
/* Free for peripherals */
extern "C" void CHandlerIRQ11()
{
    PIC_EndOfInterrupt(11);
}
/* PS/2 Mouse */
extern "C" void CHandlerIRQ12()
{
    PIC_EndOfInterrupt(12);
}
/* FPU */
extern "C" void CHandlerIRQ13()
{
    PIC_EndOfInterrupt(13);
}
/* Primary ATA */
extern "C" void CHandlerIRQ14()
{
    PIC_EndOfInterrupt(14);
}
/* Secondary ATA */
extern "C" void CHandlerIRQ15()
{
    PIC_EndOfInterrupt(15);
}