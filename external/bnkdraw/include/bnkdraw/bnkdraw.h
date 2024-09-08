#pragma once

#include <memory>
#include <string>

#include "bnkdraw-core/bnkmaths.h"
#include "bnkdraw-core/bnkcamera.h"

// Forward declarations
class WindowGLFW;
class BnkCamera2D;

// bnkdraw API
namespace bnk
{
    // Colours
    enum class Colour
    {
        RED,
        GREEN,
        BLUE,
        WHITE,    
        BLACK,
        YELLOW,
    };
    
    // TODO - Maybe we should assign the glfw values here instead of using a lookup function
    // Keys
    enum class Keyboard
    {
        KEY_0 = 48,
        KEY_1 = 49,
        KEY_2 = 50,
        KEY_3 = 51,
        KEY_4 = 52,
        KEY_5 = 53,
        KEY_6 = 54,
        KEY_7 = 55,
        KEY_8 = 56,
        KEY_9 = 57,

        KEY_A = 65,
        KEY_B = 66,
        KEY_C = 67,
        KEY_D = 68,
        KEY_E = 69,
        KEY_F = 70,
        KEY_G = 71,
        KEY_H = 72,
        KEY_I = 73,
        KEY_J = 74,
        KEY_K = 75,
        KEY_L = 76,
        KEY_M = 77,
        KEY_N = 78,
        KEY_O = 79,
        KEY_P = 80,
        KEY_Q = 81,
        KEY_R = 82,
        KEY_S = 83,
        KEY_T = 84,
        KEY_U = 85,
        KEY_V = 86,
        KEY_W = 87,
        KEY_X = 88,
        KEY_Y = 89,
        KEY_Z = 90,

        KEY_ESC = 256,
    };

    // TODO - Same as with keys, maybe these should just be assigned here instead of a map
    // Buttons
    enum class Mouse
    {
        BUTTON_1 = 0,
        BUTTON_2 = 1,
        BUTTON_3 = 2,
        BUTTON_4 = 3,
        BUTTON_5 = 4,
        BUTTON_6 = 5,
        BUTTON_7 = 6,
        BUTTON_8 = 7
    };

    // Window
    class Window
    {
    public:
        Window(int width = 800, int height = 600, const std::string& title = "Window title");
        ~Window();
        
        unsigned int GetWidth() const;
        unsigned int GetHeight() const;
        std::string GetTitle() const;
        bool GetVsync() const;
        bmth::Vec2d GetMousePosition() const;

        void SetSize(int width, int height);
        void SetTitle(const std::string& title);
        void SetVsync(bool state);

        void Close();
        void Update();

        bool IsRunning();
    private:
        std::unique_ptr<WindowGLFW> rawWindow;
    };

    // Camera2D
    class Camera2D
    {
    public:
        Camera2D(int width, int height);
        ~Camera2D();

        void Move(const bmth::Vec2f& position);
        void Zoom(float zoom);

        bmth::Mat4 GetViewProjectionMatrix() const;
        bmth::Mat4 GetViewMatrix() const;
        bmth::Mat4 GetProjectionMatrix() const;
        bmth::Vec3f GetPosition() const;
        float GetZoomLevel() const;
        float GetRotation() const;

        void SetPosition(float x, float y, float z);
        void SetPosition(const bmth::Vec3f& newPosition);
        void SetZoomLevel(float newZoom);
        void SetRotation(float newRotation);

        void ScreenToWorldPosition();
    private:
        BnkCamera2D rawCamera;
    };

    // Renderer
    class Renderer
    {
    public:
        Renderer();
        ~Renderer();

        // TODO - Create a renderer settings class/struct that will handle this sort of stuff???
        void SetRenderingMode();
        void GetRenderingMode();

        void Clear(Colour colour = Colour::WHITE);
        void Clear(bmth::Vec4f colour);

        void StartDrawing(const Camera2D& camera);
        void StopDrawing();
        
        void DrawCircle();
        void DrawCircle(float circumference, Colour colour);
        void DrawCircle(float circumference, bmth::Vec4f colour);

        void DrawLine();

        void DrawPixel();

        void DrawQuad(float posX, float posY, int width, int height, Colour colour, int textureId = -1);
        void DrawQuad(bmth::Vec2f position, bmth::Vec2i size, Colour colour, int textureId = -1);
        void DrawQuad(bmth::Vec2f position, bmth::Vec2i size, bmth::Vec4f colour, int textureId = -1);
        void DrawRotatedQuad(bmth::Vec2f position, bmth::Vec2f size, float angle, Colour colour);

        void DrawTriangle();

        void DrawText(const std::string& text, int size);

        void AddTexture(const std::string& filePath);

        //void AddTexture();
        //void AddTextureAtlas();

        //void AddShader(const std::string& filepath);
        //void AddShader();
    private:
    };

    // Input
    class Input
    {
    public:
        static bool IsKeyPressed(Keyboard key);
        static bool IsKeyReleased(Keyboard key);
        static bool IsKeyRepeated(Keyboard key);

        static bool IsMousePressed(Mouse button);
        static bool IsMouseReleased(Mouse button);
        static bool IsMouseRepeated(Mouse button);
        static bool IsMouseScrolledUp();
        static bool IsMouseScrolledDown();

        static bmth::Vec2d GetMousePosition();
    };

    // Texture2D
    class Texture2D
    {};
}
