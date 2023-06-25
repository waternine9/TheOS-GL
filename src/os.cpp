// GENERAL INCLUDES

#include "kernel.hpp"
#include "memory.hpp"
#include "idt.hpp"
#include "pic.hpp"
#include "windowing.hpp"

// GRAPHICS INCLUDES
#include "gl/swgl.h"
#include "render.hpp"

// DRIVER INCLUDES
#include "drivers/mouse/mouse.hpp"
#include "drivers/keyboard/keyboard.hpp"

// APPLICATION INCLUDES
#include "applications/cmd.hpp"
#include "applications/GlTest.hpp"

// OS DRIVER CODE STARTS HERE

uint32_t RESX = 640;
uint32_t RESY = 480;

extern int32_t MouseX;
extern int32_t MouseY;
extern int8_t MouseLmbClicked;
extern int8_t MouseRmbClicked;

keyboard_key Keys[32];
keyboard Kbd = { 0 };

Renderer Render;

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

    Render = Renderer();

    Render.Init();
    

    uint32_t KeysCount = 0;

    WindowSystem Windowing = WindowSystem();

    WindowDescriptor* CmdWindow0 = App_GlTestNewWindow();
    WindowDescriptor* CmdWindow1 = App_CmdNewWindow();

    Windowing.AddWindow(CmdWindow0);
    Windowing.AddWindow(CmdWindow1);

    CmdWindow1->X = MouseX;
    CmdWindow1->Y = MouseY;

    while (true)
    {
        CmdWindow0->X = MouseX;
        CmdWindow0->Y = MouseY;

        Render.ClearScreen(1.0, 1.0, 1.0, 1.0);
        Keyboard_CollectEvents(&Kbd, Keys, 32, &KeysCount);
        for (int i = 0; i < KeysCount; i++)
        {
            Windowing.HandleKeyPress(Keys[i]);
        }
        Windowing.Tick();

        Render.DrawCursor(MouseX, MouseY, 35, 35, 1.0f, 1.0f, 1.0f, 1.0f);

        Render.DrawLetter('L', 0, 0, MouseX, MouseY, 0.0f, 1.0f, 0.0f, 1.0f);

        Render.UpdateScreen();
    }
}

