#include "kernel.hpp"
#include "memory.hpp"

#define SWGL_FREESTANDING

#include "gl/swgl.h"

#define RESX 640
#define RESY 480

volatile void DisplayBuffer(volatile uint32_t* Buff)
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
static const char* FragShaderSource = "out vec4 OutColor;\nin vec3 Bruh;\nint main(){\nOutColor = vec4(Bruh.x, Bruh.y, Bruh.z, 1.0);\n}";

static const char* VertexShaderSource = "layout(location = 0) vec3 InPos;\nlayout(location = 1) vec3 InCol;\nuniform sampler2D Texture;\nout vec3 Bruh;\nuniform float Time;\nint main(){\ngl_Position = vec4((InPos.x * cos(Time)) - (InPos.y * sin(Time)), (InPos.x * sin(Time)) + (InPos.y * cos(Time)), InPos.z, 1.0 + cos(Time * 1.4));\nvec4 TexAt = texture(Texture, InCol.xy);Bruh = InCol.xyz;\n}";

extern "C" void kmain()
{
    memset((uint8_t*)0x1000000 + 0x7C00, 0, 100000);
    
    allocInit();

    glInit(RESX, RESY);

    glViewport(0, 0, RESX, RESY);

    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

    uint32_t* GLframe = glGetFramePtr();

    
    GLuint VertShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(VertShader, VertexShaderSource);
    glCompileShader(VertShader);

    
    
    GLuint FragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(FragShader, FragShaderSource);
    glCompileShader(FragShader);

    GLuint Program = glCreateProgram();

    glAttachShader(Program, VertShader);
    glAttachShader(Program, FragShader);

    glLinkProgram(Program);

    glUseProgram(Program);

    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    float vertices[] = {
        10.0f, 10.0f, 4.0f, 1.0f, 0.0f, 1.0f,
        0.0f, -10.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        -10.0f, 10.0f, 1.0f, 1.0f, 0.0f, 0.0f
    };

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

    GLuint Texture;

    glGenTextures(1, &Texture);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, Texture);
    
    for (int i = 0;i < 40000;i += 4)
    {
        *(uint8_t*)(0x1000000 + i) = i / 400;
        *(uint8_t*)(0x1000000 + i + 1) = i / 400;
        *(uint8_t*)(0x1000000 + i + 2) = i / 400;
        *(uint8_t*)(0x1000000 + i + 3) = i / 400;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 100, 100, 0, GL_RGBA, GL_UNSIGNED_BYTE, (void*)0x1000000);

    GLuint TextureLoc = glGetUniformLocation(Program, "Texture");
    GLuint TimeLoc = glGetUniformLocation(Program, "Time");
    glUniform1i(TextureLoc, 0);

    float Tick = 3.0f;
    while (true)
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        Tick += 0.5f;
        glUniform1f(TimeLoc, Tick);
        
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        DisplayBuffer(GLframe);
    }
}

