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
    KEY_1,
    KEY_2,
    KEY_3,
    KEY_4
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
    static bool IsKeyPressed(Window window, Key key);
    static bool IsKeyReleased(Window window, Key key);
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
