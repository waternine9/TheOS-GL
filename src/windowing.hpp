#ifndef H_TOS_WINDOWING
#define H_TOS_WINDOWING

#include <cstdint>
#include "utils/string.hpp"
#include "utils/vector.hpp"

#include "drivers/keyboard/keyboard.hpp"

struct EventDescriptor
{
    // 0b1? LMB clicked
    // 0b10? RMB clicked
    uint8_t Mouse;
    // First 8 bits store ASCII value of the pressed key.
    // 9th bit? Backspace pressed
    uint32_t Keyboard;
};

struct WindowDescriptor
{
    _String* Name;
    void* Storage;

    int X;
    int Y;
    int Width;
    int Height;

    void(*WinProc)(WindowDescriptor* Self);
    void(*WinDestruc)(WindowDescriptor* Self);

    EventDescriptor Events[256];
    uint8_t EventCounter;
};

class WindowSystem
{
private:
    _Vector* _Windows;
public:
    WindowDescriptor* Focus;
    WindowSystem();
    void AddWindow(WindowDescriptor* Window);
    void HandleClick();
    void HandleKeyPress(keyboard_key Key);
    void Tick();
};

#endif // H_TOS_WINDOWING