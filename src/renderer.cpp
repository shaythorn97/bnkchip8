#include "renderer.h"

#include <iostream>

#include "glad/glad.h"
#include "glfw/glfw3.h"

Window::Window(int width, int height, const std::string& title)
{
    if (!glfwInit())
        std::cout << "GLFW failed to initialise\n";

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

void Renderer::Init()
{}

void Renderer::Shutdown()
{}

void Renderer::BeginBatch()
{}

void Renderer::EndBatch()
{}

void Renderer::Flush()
{}

void Renderer::Draw()
{}

void Renderer::DrawQuad(float x, float y, int width, int height, float r, float g, float b, float a)
{}
