#ifndef H_TOS_RENDER
#define H_TOS_RENDER

class Renderer
{
public:
    volatile void Init(int width, int height);
    volatile void ClearScreen(float red, float green, float blue, float alpha);
    volatile void DrawBackground();
    volatile void DrawLetter(char letter, float x, float y, float scale, float red, float green, float blue, float alpha);
    volatile void UpdateScreen();
};

#endif // H_TOS_RENDER