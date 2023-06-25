#ifndef H_TOS_RENDER
#define H_TOS_RENDER

class Renderer
{
public:
    volatile void Init();
    volatile void ClearScreen(float red, float green, float blue, float alpha);
    volatile void DrawBackground();
    volatile void DrawLetter(char letter, float x, float y, float width, float height, float red, float green, float blue, float alpha);
    volatile void DrawCursor(float x, float y, float width, float height, float red, float green, float blue, float alpha);
    volatile void UpdateScreen();
};

#endif // H_TOS_RENDER