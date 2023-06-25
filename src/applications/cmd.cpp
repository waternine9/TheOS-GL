#include "cmd.hpp"
#include "../gl/swgl.h"
#include "../memory.hpp"
#include "../render.hpp"
#include "../utils/string.hpp"

struct CmdStorage
{
    _String* Text;
};

extern int32_t MouseX;
extern int32_t MouseY;

extern Renderer Render;

void App_CmdProc(WindowDescriptor* Self)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    CmdStorage* Storage = (CmdStorage*)Self->Storage;

    while (Self->EventCounter > 0)
    {
        Self->EventCounter--;
        uint32_t Event = Self->Events[Self->EventCounter].Keyboard;
        if (Event)
        {
            if (Event & 0x100)
            {
                StringPop(Storage->Text);
            }
            else
            {
                StringPush(Storage->Text, Event & 0xFF);
            }
        }
    }

    int X = Self->X + 20;
    int Y = Self->Y + 20;

    for (int i = 0;i < Storage->Text->Size;i++)
    {
        char C = StringGet(Storage->Text, i);
        switch (C)
        {
        case '\n':
            X = Self->X + 10;
            Y += 25;
            break;
        case '\t':
            X += 40;
            break;
        default:
            Render.DrawLetter(C, X, Y, 10, 20, 1.0f, 1.0f, 1.0f, 1.0f);
            X += 10;
            break;
        }
        if (X + 20 > Self->X + Self->Width)
        {
            X = Self->X + 10;
            Y += 25;
        }
    }
}

void App_CmdDestruc(WindowDescriptor* Self)
{

}

WindowDescriptor* App_CmdNewWindow()
{
    WindowDescriptor* Window = (WindowDescriptor*)malloc(sizeof(WindowDescriptor));
    Window->X = 0;
    Window->Y = 0;
    Window->Width = 400;
    Window->Height = 200;
    Window->EventCounter = 0;

    CmdStorage* NewStorage = (CmdStorage*)malloc(sizeof(CmdStorage));

    NewStorage->Text = NewString();

    Window->Storage = NewStorage;

    Window->Name = CString2String("Freespace");
    Window->WinProc = &App_CmdProc;
    Window->WinDestruc = &App_CmdDestruc;

    return Window;
}