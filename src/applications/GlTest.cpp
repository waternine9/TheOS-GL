#include "GlTest.hpp"
#include "../gl/swgl.h"
#include "../memory.hpp"
#include "../render.hpp"
#include "../math.hpp"

const char* App_GlTestVertShaderSource = "layout(location = 0) vec3 InPos;layout(location = 1) vec3 InCol;out vec3 FragCol;uniform mat4 MVP;uniform float Tick;int main(){gl_Position = MVP * vec4(InPos.x, InPos.y, InPos.z, 1.0);FragCol = InCol;}";
const char* App_GlTestFragShaderSource = "out vec4 OutCol;in vec3 FragCol;int main(){OutCol = vec4(FragCol.x, FragCol.y, FragCol.z, 1.0);}";

struct GlTestStorage
{
    GLuint ShaderProgram;
    GLuint VAO;
    float Tick;
};

extern int32_t MouseX;
extern int32_t MouseY;

extern uint32_t RESX;
extern uint32_t RESY;

void App_GlTestProc(WindowDescriptor* Self)
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    GlTestStorage* Storage = (GlTestStorage*)Self->Storage;
    glUseProgram(Storage->ShaderProgram);
    glBindVertexArray(Storage->VAO);

    GLuint TickLoc = glGetUniformLocation(Storage->ShaderProgram, "Tick");
    glUniform1f(TickLoc, (Storage->Tick++) / 100.0f);

    GLuint MVPLoc = glGetUniformLocation(Storage->ShaderProgram, "MVP");

    float mvp[16] = { 0.0f };

    mat4_translation(0.5f, 0.0f, 0.0f, mvp);

    glUniformMatrix4fv(MVPLoc, 1, GL_TRUE, mvp);

    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void App_GlTestDestruc(WindowDescriptor* Self)
{

}

WindowDescriptor* App_GlTestNewWindow()
{
    WindowDescriptor* Window = (WindowDescriptor*)malloc(sizeof(WindowDescriptor));
    Window->X = 0;
    Window->Y = 0;
    Window->Width = 400;
    Window->Height = 200;
    Window->EventCounter = 0;

    GlTestStorage* NewStorage = (GlTestStorage*)malloc(sizeof(GlTestStorage));

    GLuint VertShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(VertShader, App_GlTestVertShaderSource);
    glShaderSource(FragShader, App_GlTestFragShaderSource);

    glCompileShader(VertShader);
    glCompileShader(FragShader);

    NewStorage->ShaderProgram = glCreateProgram();
    glAttachShader(NewStorage->ShaderProgram, VertShader);
    glAttachShader(NewStorage->ShaderProgram, FragShader);

    glLinkProgram(NewStorage->ShaderProgram);

    glGenVertexArrays(1, &NewStorage->VAO);
    GLuint VBO;
    glGenBuffers(1, &VBO);

    glBindVertexArray(NewStorage->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, NewStorage->VAO);

    float Vertices[] = {
        -1.0f, -0.2f, 2.0f, 1.0f, 0.0f, 0.0f,
        0.2f, -0.1f, 2.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.5f, 2.0f, 0.0f, 0.0f, 1.0f
    };
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, false, 6 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, false, 6 * sizeof(float), (void*)(3 * sizeof(float)));

    NewStorage->Tick = 0.0f;

    Window->Storage = NewStorage;

    Window->Name = CString2String("GlTest");
    Window->WinProc = &App_GlTestProc;
    Window->WinDestruc = &App_GlTestDestruc;

    return Window;
}