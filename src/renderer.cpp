#include "renderer.h"

#include <cstddef>
#include <iostream>
#include <vector>

#include "glad/glad.h"
#include "glfw/glfw3.h"

Window::Window(int width, int height, const std::string& title)
    : width(width), height(height)
{
    if (!glfwInit())
    {
        std::cout << "GLFW failed to initialise\n";
        return;
    }

    rawWindow = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);

    // add the context
    glfwMakeContextCurrent(rawWindow);

    glfwSwapInterval(1);
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

const GLuint MAX_QUADS = 500;
const GLuint MAX_VERTICES = MAX_QUADS * 4;
const GLuint MAX_INDICES = MAX_QUADS * 6;

struct Vertex
{
    Vec3 pos; 
    Vec4 col;
};

struct RendererData
{
    GLuint vbo = 0;
    GLuint ibo = 0;
    GLuint vao = 0;
    GLuint shaderProgram = 0;

    GLuint indexCount = 0;

    std::vector<Vertex> vertices;
};

static RendererData rendererData;

void Renderer::Init(int width, int height)
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

    glNamedBufferData(rendererData.vbo, MAX_VERTICES * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW); 
    glNamedBufferData(rendererData.ibo, MAX_INDICES * sizeof(GLuint), nullptr, GL_STATIC_DRAW);

    glVertexArrayVertexBuffer(rendererData.vao, 0, rendererData.vbo, 0, sizeof(Vertex));
    glVertexArrayElementBuffer(rendererData.vao, rendererData.ibo);

    glEnableVertexArrayAttrib(rendererData.vao, 0);
    glEnableVertexArrayAttrib(rendererData.vao, 1);

    glVertexArrayAttribFormat(rendererData.vao, 0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, pos));
    glVertexArrayAttribFormat(rendererData.vao, 1, 4, GL_FLOAT, GL_FALSE, offsetof(Vertex, col));

    glVertexArrayAttribBinding(rendererData.vao, 0, 0);
    glVertexArrayAttribBinding(rendererData.vao, 1, 0);

    glBindVertexArray(rendererData.vao);

    // vertex shader, fragment shader
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);

    std::string vsSourceStr =
        #include "shaders/vertex.vert"
        ;
    std::string fsSourceStr =
        #include "shaders/fragment.frag"
        ;

    const char* vsSource = vsSourceStr.c_str();
    const char* fsSource = fsSourceStr.c_str();

    glShaderSource(vs, 1, &vsSource, NULL);
    glShaderSource(fs, 1, &fsSource, NULL);

    glCompileShader(vs);

    GLint success;
    glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(vs, 512, NULL, infoLog);
        std::cerr << "Vertex Shader Compilation Error:\n" << infoLog << std::endl;
    }

    glCompileShader(fs);

    glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(fs, 512, NULL, infoLog);
        std::cerr << "Fragment Shader Compilation Error:\n" << infoLog << std::endl;
    }

    rendererData.shaderProgram = glCreateProgram();
    glAttachShader(rendererData.shaderProgram, vs);
    glAttachShader(rendererData.shaderProgram, fs);

    glLinkProgram(rendererData.shaderProgram);

    glGetProgramiv(rendererData.shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(rendererData.shaderProgram, 512, NULL, infoLog);
        std::cerr << "Shader Program Linking Error:\n" << infoLog << std::endl;
    }

    glUseProgram(rendererData.shaderProgram);

    glDeleteShader(vs);
    glDeleteShader(fs);

    // upload the uniform here because it will never transform
    GLfloat left = 0.0f;
    GLfloat right = width;
    GLfloat bottom = 0.0f;
    GLfloat top = height;
    GLfloat near = -1.0f;
    GLfloat far = 1.0f;
    
    Mat4 viewProj
    {
        {
            2 / (right - left), 0.0f, 0.0f, 0.0f,
            0.0f, 2 / (top - bottom), 0.0f, 0.0f,
            0.0f, 0.0f, -2 / (far - near), 0.0f,
            -((right + left) / (right - left)), -((top + bottom) / (top - bottom)), -((far + near) / (far - near)), 1.0f,
        },
    };

    GLint viewProjUniform = glGetUniformLocation(rendererData.shaderProgram, "u_vp");
    glUniformMatrix4fv(viewProjUniform, 1, GL_FALSE, &viewProj[0].x);

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

    glNamedBufferSubData(rendererData.ibo, 0, MAX_INDICES * sizeof(GLuint), indices); 
}

void Renderer::Shutdown()
{
    glDeleteBuffers(1, &rendererData.vbo);
    glDeleteBuffers(1, &rendererData.ibo);
    glDeleteVertexArrays(1, &rendererData.vao);
    glDeleteProgram(rendererData.shaderProgram);
}

void Renderer::BeginBatch()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::EndBatch()
{
    if (!rendererData.vertices.empty())
        Flush();
}

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

    GLfloat positions[]
    {
        x, y,
        x + width, y,
        x + width, y + height,
        x, y + height,
    };

    for (GLint i = 0; i < 8; i += 2)
        rendererData.vertices.emplace_back(Vertex{ { positions[i], positions[i + 1], 1.0f }, { r, g, b, a } });  

    rendererData.indexCount += 6;
}
