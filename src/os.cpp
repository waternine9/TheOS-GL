#include "kernel.hpp"
#include "memory.hpp"
#include "idt.hpp"
#include "pic.hpp"

#include "gl/swgl.h"

#include "drivers/mouse/mouse.hpp"
#include "drivers/keyboard/keyboard.hpp"
#include "render.hpp"

#define RESX 640
#define RESY 480

extern int32_t MouseX;
extern int32_t MouseY;
extern int8_t MouseLmbClicked;
extern int8_t MouseRmbClicked;

keyboard_key Keys[32];
keyboard Kbd = { 0 };

extern "C" void kmain()
{
    memset((uint8_t*)0x1000000 + 0x7C00, 0, 100000);
    
    allocInit();
    
    PIC_Init();
    PIC_SetMask(0xFFFF); // Disable all irqs

    MouseInstall();

    MouseX = RESX / 2;
    MouseY = RESY / 2;

    IDT_Init();
    PIC_SetMask(0x0000); // Enable all irqs

    Renderer Render = Renderer();

    Render.Init(RESX, RESY);
    
    Render.ClearScreen(1.0, 1.0, 1.0, 1.0);
    Render.DrawBackground();

    uint32_t KeysCount = 0;

    float X = -1;
    float Y = 0.9;
    while (true)
    {
        Keyboard_CollectEvents(&Kbd, Keys, 32, &KeysCount);
        for (int I = 0; I < KeysCount; I++)
        {
            if (!Keys[I].Released)
            {
                if (Keys[I].ASCII == '\n') 
                {
                    X = -1;
                    Y -= 0.3f;
                }
                else if (Keys[I].ASCII == '\t') X += 0.6f;
                else Render.DrawLetter(Keys[I].ASCII, X += 0.15f, Y, 0.1, 0.0, 0.0, 0.0, 1.0);
            }
        }
        Render.UpdateScreen();
    }
}

