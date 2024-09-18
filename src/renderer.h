#pragma once

#include <string>

// we need vertex buffer, index buffer, vertex array, camera, shaders
class GLFWwindow;

struct Vec3
{
    float x, y, z;
};

struct Vec4
{
    float x, y, z, w;
};

struct Mat4
{
    Vec4 mat[4];

    Vec4& operator[](int index)
    {
        return mat[index];
    }

    const Vec4& operator[](int index) const
    {
        return mat[index];
    }
};

enum class Key
{
    KEY_1 = 49,
    KEY_2 = 50,
    KEY_3 = 51,
    KEY_4 = 52,

    KEY_Q = 81,
    KEY_W = 87,
    KEY_E = 69,
    KEY_R = 82,

    KEY_ESC = 256,
};

class Window
{
public:
    GLFWwindow* rawWindow;
    int width, height;

    Window(int width, int height, const std::string& title);
    ~Window();

    bool IsRunning();
    void Update();
};

class Input
{
public:
    static bool IsKeyPressed(const Window& window, Key key);
    static bool IsKeyReleased(const Window& window, Key key);
};

class Renderer
{
public:
    static void Init(int width, int height);
    static void Shutdown();

    static void BeginBatch();
    static void EndBatch();
    static void Flush();

    static void DrawElements();

    static void DrawQuad(float x, float y, int width, int height, float r, float g, float b, float a); 
};
