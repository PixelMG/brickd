#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/gl.h>

#include "../common/linmath.h"
#include "../common/types.hpp"

class Camera
{
    private:
    Rectangle view;
    Vector2 offset;
    mat4x4 identity, translation, matrix;

    public:
    Camera(float x, float y, float width, float height);
    void SetPosition(float x, float y);
    void Move(float x, float y);
    void GetProjection(mat4x4 mvp);
    float GetOffsetX() { return offset.x; }
    float GetOffsetY() { return offset.y; }
};

class Base
{

    public:
    Base();
    int CreateWindow(int width, int height, const char* title);
    bool IsRunning();
    float GetTime() { return (float)glfwGetTime(); }
    void InitializeRenderer();
    void CreateCamera();
    void AttachCamera(Camera* camera) { ActiveCamera = camera; }
    void Clear(float r, float g, float b, float a);
    void Begin();
    void Draw(GLuint sprite, Rectangle source, Rectangle clip, Color color);
    void End();
    void PollInput();
    bool KeyDown(int key);
    unsigned int LoadContent(const std::string& path);
    int Close();

    private:
    GLFWwindow* window;
    GLuint program;
    GLuint v_buffer, i_buffer, mvp_location;
    size_t QuadCount = 0;
    static const size_t MaxQuadCount = 10000;
    const size_t MaxVertexCount = MaxQuadCount * 4;
    const size_t MaxIndexCount = MaxQuadCount * 6;
    std::array<Vertex, MaxQuadCount*4> verts;
    std::array<Texture, 32> textures;
    Vertex* buffer;
    Camera* ActiveCamera = new Camera(0.0f, 0.0f, 640, 360);
    uint32_t i_count, s_count = 0;
    unsigned int VBO, VAO;
    int WIDTH;
    int HEIGHT;
    static Vertex* CreateQuad(Vertex* target, Rectangle source, Rectangle clip, float texID, Color color);
    static Texture LoadTexture(const std::string& path);
};