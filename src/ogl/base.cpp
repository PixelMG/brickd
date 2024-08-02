#include "../application.hpp"
#include "base.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "../ogl/stb_image.h"

using namespace Engine;

Game1* game;

void ErrorCallback(int error, const char* description)
{
    fprintf(stderr, "Error %s\n", description);
}

// Camera
Camera::Camera(float x, float y, float width, float height)
{
    view = { x, y, width, height };
    offset = { 0.0f, 0.0f };

    float near = 0.0f, far = 100.0f;

    mat4x4_identity(matrix);
    mat4x4_ortho(translation, 0.0f, width, height, 0.0f, near, far);

    mat4x4_translate(matrix, view.x, view.y, -1.0f);
}

void Camera::Move(float x, float y)
{
    offset.x += x;
    offset.y += y;
    mat4x4_translate(matrix, view.x, view.y, -1.0f);
}

void Camera::GetProjection(mat4x4 mvp)
{
    mat4x4_mul(mvp, translation, matrix);
}
// end Camera

Base::Base() {};

int Base::CreateWindow(int width, int height, const char* title)
{
    // store window dimensions
    WIDTH = width, HEIGHT = height;

    if(!glfwInit())
        return -1;

    glfwSetErrorCallback(ErrorCallback);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // #ifdef __APPLE__
    //     glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    // #endif
    
    window = glfwCreateWindow(width, height, title, NULL, NULL);

    if(!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    gladLoadGL(glfwGetProcAddress);
    glfwSwapInterval(1);

    game->Run();

    while(IsRunning()) game->Loop();

    return 1;
}

void Base::InitializeRenderer()
{
    GLuint vertex_shader, fragment_shader;

    static const char* vsSource =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos; // position\n"
    "layout (location = 1) in vec4 aCol; // color\n"
    "layout (location = 2) in vec2 aTexCoord; // texture coordinates\n"
    "layout (location = 3) in float aTexID;"
    "\n"
    "uniform mat4 uMVP;\n"
    "\n"
    "out vec4 vColor;\n"
    "out vec2 vTexCoord;\n"
    "out float texIndex;\n"
    "\n"
    "void main()\n"
    "{\n"
    "   vColor      = aCol;\n"
    "   vTexCoord   = aTexCoord;\n"
    "   texIndex    = aTexID;\n"
    "   gl_Position = uMVP * vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
    
    static const char* fsSource =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "\n"
    "in vec4 vColor;\n"
    "in vec2 vTexCoord;\n"
    "in float texIndex;\n"
    "\n"
    "uniform sampler2D uTextures[2];"
    "\n"
    "void main()\n"
    "{\n"
    "   int tID = int(texIndex);\n"
    "   FragColor = texture(uTextures[tID], vTexCoord) * vColor;\n"
    "}\n";

    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vsSource, NULL);
    glCompileShader(vertex_shader);

    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fsSource, NULL);
    glCompileShader(fragment_shader);

    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    // cleanup garbage
    glDetachShader(program, vertex_shader);
    glDetachShader(program, fragment_shader);
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}

void Base::Clear(float r, float g, float b, float a)
{
    float ratio;
    int width, height;

    glfwGetFramebufferSize(window, &width, &height);
    ratio = width / (float) height;

    glViewport(0, 0, width, height);
    glClearColor(r, g, b, a);
    glClearDepth(1.0);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Base::Begin()
{
    GLint vtexture;

    i_count = 0;
    QuadCount = 0;

    mvp_location = glGetUniformLocation(program, "uMVP");
    vtexture     = glGetUniformLocation(program, "uTextures");
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, MaxVertexCount * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Position));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Color));
    glEnableVertexAttribArray(1);
    
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, TexCoords));
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, TexID));
    glEnableVertexAttribArray(3);

    int sampler[1] = { 0 };
    glUniform1iv(vtexture, 1, sampler);

    uint32_t indices[MaxIndexCount];
    uint32_t offset = 0;
    for(size_t i = 0; i < MaxIndexCount; i += 6)
    {
        indices[i + 0] = 3 + offset;
        indices[i + 1] = 2 + offset;
        indices[i + 2] = 1 + offset;

        indices[i + 3] = 3 + offset;
        indices[i + 4] = 1 + offset;
        indices[i + 5] = 0 + offset;

        offset += 4;
    }

    glGenBuffers(1, &i_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, i_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);

    buffer = verts.data();
}

void Base::Draw(GLuint sprite, Rectangle source, Rectangle clip, Color color)
{
    if(QuadCount >= MaxQuadCount)
    {
        End();
        Begin();
        QuadCount = 0;
    }

    Rectangle s = source;
    Rectangle c = clip;
    Rectangle d;

    Texture t = textures[sprite];

    if(c.w + c.h == 0)
    {
        d.x = 0.0f, d.y = 0.0f, d.w = 1.0, d.h = 1.0;
    }
    else
    {
        d.x = c.x / t.width;
        d.y = (t.height - c.y - c.h) / t.height;
        d.w = (c.x + c.w) / t.width;
        d.h = (t.height - c.y) / t.height;
    }

    buffer = CreateQuad(buffer, s, d, (float)sprite, color);
    i_count += 6;
    QuadCount++;
}

void Base::End()
{
    glBindVertexArray(VAO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, verts.size() * sizeof(Vertex), verts.data());
    
    mat4x4 mvp;

    ActiveCamera->GetProjection(mvp);

    glUseProgram(program);

    for(int i = 0; i < s_count; i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, textures[i].ID);
    }


    glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*) mvp);
    glDrawElements(GL_TRIANGLES, i_count, GL_UNSIGNED_INT, nullptr);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &i_buffer);
    glfwSwapBuffers(window);
}

void Base::PollInput()
{
    glfwPollEvents();
}

bool Base::KeyDown(int key)
{
    if(glfwGetKey(window, key) == GLFW_PRESS)
        return true;
    else
        return false;
}

bool Base::IsRunning()
{
    return !glfwWindowShouldClose(window);
}

Texture Base::LoadTexture(const std::string& path)
{
    int w, h, bits;

    stbi_set_flip_vertically_on_load(1);
    auto* pixels = stbi_load(path.c_str(), &w, &h, &bits, STBI_rgb_alpha);
    GLuint TextureID;
    glGenTextures(1, &TextureID);
    glBindTexture(GL_TEXTURE_2D, TextureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    return { (float)w, (float)h, TextureID };
}

unsigned int Base::LoadContent(const std::string& path)
{
    auto tex = LoadTexture(path);
    unsigned int tID = tex.ID - 1;
    textures[tID] = tex;
    s_count++;
    return tID;
}

Vertex* Base::CreateQuad(Vertex* target, Rectangle source, Rectangle clip, float texID, Color color)
{
    Rectangle s = source;
    Rectangle d = clip;

    target->Position = { s.x, s.y + s.h, 0.0f };
    target->Color = color;
    target->TexCoords = { d.x, d.y };
    target->TexID = texID;
    target++;

    target->Position = { s.x + s.w, s.y + s.h, 0.0f };
    target->Color = color;
    target->TexCoords = { d.w, d.y };
    target->TexID = texID;
    target++;

    target->Position = { s.x + s.w, s.y, 0.0f };
    target->Color = color;
    target->TexCoords = { d.w, d.h };
    target->TexID = texID;
    target++;

    target->Position = { s.x, s.y, 0.0f };
    target->Color = color;
    target->TexCoords = { d.x, d.h };
    target->TexID = texID;
    target++;

    return target;
}

int Base::Close()
{
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

Application::Application() {}

void Application::Run()
{
    game = new Game1(1280, 720, "Brick\'d!");
    game->Setup();
}

void Application::Quit()
{
    game->Stop();
}