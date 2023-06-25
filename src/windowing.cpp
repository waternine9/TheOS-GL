#include "windowing.hpp"
#include "render.hpp"
#include "gl/swgl.h"

extern uint32_t RESX;
extern uint32_t RESY;

extern Renderer Render;

const char* WindowVertShaderSource = "layout(location = 0) vec3 InPos;layout(location = 1) vec2 InUV;out vec2 UV;int main(){gl_Position = vec4(InPos.x, InPos.y, InPos.z, 1.0);UV = InUV;}";
const char* WindowFragShaderSource = "out vec4 FragColor;in vec2 UV;int main(){float shadeY = UV.y / 2.0;FragColor=vec4(0.0, 0.0, 0.0, shadeY + 0.25);}";

GLuint BorderVAO;
GLuint BorderProgram;

WindowSystem::WindowSystem()
{
    Focus = 0;
    _Windows = NewVector(sizeof(WindowDescriptor*));

    GLuint VertShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragShader = glCreateShader(GL_FRAGMENT_SHADER);
    
    glShaderSource(VertShader, WindowVertShaderSource);
    glShaderSource(FragShader, WindowFragShaderSource);

    glCompileShader(VertShader);
    glCompileShader(FragShader);
    
    BorderProgram = glCreateProgram();

    glAttachShader(BorderProgram, VertShader);
    glAttachShader(BorderProgram, FragShader);
    
    glLinkProgram(BorderProgram);

    glGenVertexArrays(1, &BorderVAO);
    GLuint VBO;
    glGenBuffers(1, &VBO);

    glBindVertexArray(BorderVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    float Vertices[] = {
        -1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        1.0f, -1.0f, 0.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        -1.0f, -1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, -1.0f, 0.0f, 1.0f, 1.0f
    };

    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
}

void WindowSystem::AddWindow(WindowDescriptor* Window)
{
    VectorPushBack(_Windows, &Window);
}

void WindowSystem::HandleClick()
{

}

void WindowSystem::Tick()
{
    if (_Windows->Size > 0) VectorRead(_Windows, &Focus, _Windows->Size - 1);
    else Focus = 0;

    for (int i = 0;i < _Windows->Size;i++)
    {
        WindowDescriptor* Window;
        VectorRead(_Windows, &Window, i);

        

        glViewport(Window->X, Window->Y - 14, Window->Width, 15);
        glUseProgram(BorderProgram);
        glBindVertexArray(BorderVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        
        float StepX = 0.0f;

        for (int j = 0;j < Window->Name->Size;j++)
        {
            Render.DrawLetter(StringGet(Window->Name, j), Window->X + (StepX += 20) - 10, Window->Y - 15, 20, 14, 1.0, 1.0, 1.0, 1.0);
        }

        glViewport(Window->X, Window->Y, Window->Width, Window->Height);
        (*Window->WinProc)(Window);
    }
    glViewport(0, 0, RESX, RESY);
}

void WindowSystem::HandleKeyPress(keyboard_key Key)
{
    if (!Focus) return;
    if (Key.Released) return;

    uint32_t EventSignal = 0;
    if (Key.Scancode == KEY_BACKSPACE)
    {
        EventSignal |= 0x100;
    }
    EventSignal |= Key.ASCII;

    EventDescriptor Event;
    Event.Keyboard = EventSignal;
    Event.Mouse = 0;

    Focus->Events[Focus->EventCounter++] = Event;
}