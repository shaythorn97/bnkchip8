#include "renderer.h"

#include <iostream>
#include <vector>

#include "glad/glad.h"
#include "glfw/glfw3.h"

Window::Window(int width, int height, const std::string& title)
{
    if (!glfwInit())
    {
        std::cout << "GLFW failed to initialise\n";
        return;
    }

    rawWindow = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
}

Window::~Window()
{
    if (rawWindow)
        glfwDestroyWindow(rawWindow);

    if (glfwInit())
        glfwTerminate();
}

bool Window::IsRunning()
{
    return !glfwWindowShouldClose(rawWindow);
}

void Window::Update()
{
    glfwPollEvents();
    glfwSwapBuffers(rawWindow);
}

bool Input::IsKeyPressed(Window window, Key key)
{
    int state = glfwGetKey(window.rawWindow, (int)key);

    return state == GLFW_PRESS ? true : false;
}

bool Input::IsKeyReleased(Window window, Key key)
{
    int state = glfwGetKey(window.rawWindow, (int)key);

    return state == GLFW_RELEASE ? true : false;
}

const GLuint MAX_QUADS = 1000;
const GLuint MAX_VERTICES = MAX_QUADS * 4;
const GLuint MAX_INDICES = MAX_QUADS * 6;

struct Vertex
{
    float x, y, z;
    float r, g, b, a;
};

struct RendererData
{
    GLuint vbo = 0;
    GLuint ibo = 0;
    GLuint vao = 0;

    GLuint indexCount = 0;

    std::vector<Vertex> vertices;
};

static RendererData rendererData;

void Renderer::Init()
{
    if (!gladLoadGL())
    {
        std::cout << "GLAD failed to initialise\n";
        return;
    }

    rendererData.vertices.reserve(MAX_VERTICES);

    glCreateBuffers(1, &rendererData.vbo);
    glCreateBuffers(1, &rendererData.ibo);
    glCreateVertexArrays(1, &rendererData.vao);

    glNamedBufferData(rendererData.vbo, 0, nullptr, GL_DYNAMIC_DRAW); 
    glNamedBufferData(rendererData.ibo, 0, nullptr, GL_STATIC_DRAW);

    // vertex array layout 
    // 3 floats, 4 floats 
    
    // vertex shader, fragment shader

    GLuint indices[MAX_INDICES];
    GLint index = 0;

    for (GLint i = 0; i < MAX_INDICES; i += 6)
    {
        indices[i] = index;
        indices[i + 1] = index + 1;
        indices[i + 2] = index + 2;

        indices[i + 3] = index;
        indices[i + 4] = index + 2;
        indices[i + 5] = index + 3;
    }

    glNamedBufferSubData(rendererData.ibo, 0, MAX_INDICES * sizeof(GLuint), nullptr); 
}

void Renderer::Shutdown()
{
    glDeleteBuffers(1, &rendererData.vbo);
    glDeleteBuffers(1, &rendererData.ibo);
    glDeleteVertexArrays(1, &rendererData.vao);
}

void Renderer::BeginBatch()
{

}

void Renderer::EndBatch()
{}

void Renderer::Flush()
{
    // setup for draw
    glNamedBufferSubData(rendererData.vbo, 0, rendererData.vertices.size() * sizeof(Vertex), rendererData.vertices.data());

    // draw 
    DrawElements();

    // reset
    rendererData.vertices.clear();
    rendererData.indexCount = 0;
}

void Renderer::DrawElements()
{
    glDrawElements(GL_TRIANGLES, rendererData.indexCount, GL_UNSIGNED_INT, nullptr);
}

void Renderer::DrawQuad(float x, float y, int width, int height, float r, float g, float b, float a)
{
    if (rendererData.indexCount > MAX_INDICES - 1)
        Flush();

    rendererData.indexCount += 6;
}
