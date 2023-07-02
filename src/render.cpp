#include "render.hpp"
#include "kernel.hpp"
#include "memory.hpp"

#define SWGL_FREESTANDING
#include "gl/swgl.h"

extern uint32_t RESX;
extern uint32_t RESY;

void DisplayBuffer(uint32_t* Buff)
{
    uint8_t* VesaFramebuff = (uint8_t*)VbeModeInfo.framebuffer;
    for (int i = 0;i < RESX * RESY;i++)
    {
        VesaFramebuff[2] = (Buff[i] >> 24);
        VesaFramebuff[1] = (Buff[i] >> 16) & 0xFF;
        VesaFramebuff[0] = (Buff[i] >> 8) & 0xFF;

        VesaFramebuff += 3;
    }
}

static const char* BGFragShaderSource = "out vec4 OutColor;\nin vec3 FragColor;\nint main(){\nOutColor = vec4(FragColor.x, FragColor.y, FragColor.z, 1.0);\n}";
static const char* BGVertexShaderSource = "layout(location = 0) vec3 InPos;\nlayout(location = 1) vec3 InCol;\nout vec3 FragColor;\nuniform float Tick;\nint main(){\ngl_Position = vec4(cos(Tick) + InPos.x, sin(Tick) + InPos.y, InPos.z, 1.0);FragColor = InCol;}";
static const char* GlyphFragShaderSource = "out vec4 OutColor;\nin vec2 UV;\nuniform vec4 Color;\nuniform sampler2D Glyph;\nint main(){\nOutColor = texture(Glyph, UV) * Color;}";
static const char* GlyphVertexShaderSource = "layout(location = 0) vec3 InPos;\nlayout(location = 1) vec2 InUV;\nout vec2 UV;\nint main(){\ngl_Position = vec4(InPos.x, InPos.y, InPos.z, 1.0);UV = InUV;}";

GLuint BGFragShader, BGVertShader, GlyphFragShader, GlyphVertShader;
GLuint BGProgram, GlyphProgram;

GLuint BGVAO, BGVBO;
GLuint GlyphVAO, GlyphVBO;

GLuint GlyphTextures[256];
GLuint CursorTexture;

float BGTick;

volatile void Renderer::Init()
{
    glInit(RESX, RESY, malloc(100000), malloc(100000), malloc(100000), malloc(100000), malloc(10000));
    glViewport(0, 0, RESX, RESY);

    BGTick = 0.5f;

    BGVertShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(BGVertShader, BGVertexShaderSource);
    glCompileShader(BGVertShader);

    BGFragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(BGFragShader, BGFragShaderSource);
    glCompileShader(BGFragShader);

    BGProgram = glCreateProgram();
    glAttachShader(BGProgram, BGVertShader);
    glAttachShader(BGProgram, BGFragShader);

    glLinkProgram(BGProgram);
    
    GlyphVertShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(GlyphVertShader, GlyphVertexShaderSource);
    glCompileShader(GlyphVertShader);

    GlyphFragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(GlyphFragShader, GlyphFragShaderSource);
    glCompileShader(GlyphFragShader);

    GlyphProgram = glCreateProgram();
    glAttachShader(GlyphProgram, GlyphVertShader);
    glAttachShader(GlyphProgram, GlyphFragShader);

    glLinkProgram(GlyphProgram);

    glGenVertexArrays(1, &BGVAO);
    glGenBuffers(1, &BGVBO);

    glBindVertexArray(BGVAO);
    glBindBuffer(GL_ARRAY_BUFFER, BGVBO);

    float BGvertices[] = {
        1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        -1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f
    };

    glBufferData(GL_ARRAY_BUFFER, sizeof(BGvertices), BGvertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

    glGenVertexArrays(1, &GlyphVAO);
    glGenBuffers(1, &GlyphVBO);

    glBindVertexArray(GlyphVAO);
    glBindBuffer(GL_ARRAY_BUFFER, GlyphVBO);

    float Glyphvertices[] = {
        -1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, -1.0f, 0.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f, 0.0f, 1.0f
    };

    glBufferData(GL_ARRAY_BUFFER, sizeof(Glyphvertices), Glyphvertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    for (int i = 0;i < 255;i++)
    {
        glGenTextures(1, &GlyphTextures[i]);
        glBindTexture(GL_TEXTURE_2D, GlyphTextures[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 80, 160, 0, GL_RGBA, GL_UNSIGNED_BYTE, (uint8_t*)(&GlyphLabel) + i * 80 * 160 * 4);
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    glGenTextures(1, &CursorTexture);
    glBindTexture(GL_TEXTURE_2D, CursorTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 32, 48, 0, GL_RGBA, GL_UNSIGNED_BYTE, (uint8_t*)(&ImageLabel));
    glGenerateMipmap(GL_TEXTURE_2D);
}
volatile void Renderer::ClearScreen(float red, float green, float blue, float alpha)
{
    glClearColor(red, green, blue, alpha);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
volatile void Renderer::DrawBackground()
{
    glUseProgram(BGProgram);
    glBindVertexArray(BGVAO);

    GLuint TickLoc = glGetUniformLocation(BGProgram, "Tick");


    glUniform1f(TickLoc, BGTick);

    BGTick += 0.2f;

    glDrawArrays(GL_TRIANGLES, 0, 3);
}
volatile void Renderer::DrawLetter(uint8_t letter, float x, float y, float width, float height, float red, float green, float blue, float alpha)
{
    glUseProgram(GlyphProgram);
    glBindVertexArray(GlyphVAO);

    GLuint GlyphLoc = glGetUniformLocation(GlyphProgram, "Glyph");
    glUniform1i(GlyphLoc, 0);

    GLuint ColorLoc = glGetUniformLocation(GlyphProgram, "Color");
    glUniform4f(ColorLoc, red, green, blue, alpha);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, GlyphTextures[letter - 1]);

    glViewport(x, y, width, height);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glViewport(0, 0, RESX, RESY);
}
volatile void Renderer::DrawCursor(float x, float y, float width, float height, float red, float green, float blue, float alpha)
{
    glUseProgram(GlyphProgram);
    glBindVertexArray(GlyphVAO);

    GLuint GlyphLoc = glGetUniformLocation(GlyphProgram, "Glyph");
    glUniform1i(GlyphLoc, 0);

    GLuint ColorLoc = glGetUniformLocation(GlyphProgram, "Color");
    glUniform4f(ColorLoc, red, green, blue, alpha);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, CursorTexture);

    glViewport(x, y, width, height);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glViewport(0, 0, RESX, RESY);
}
volatile void Renderer::UpdateScreen()
{
    DisplayBuffer(glGetFramePtr());
}