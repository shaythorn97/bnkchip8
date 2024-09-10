#include "chip8.h"
#include "renderer.h"


int main()
{
    Window window(640, 320, "Chip8");
    Chip8 chip8;

    chip8.LoadROM("roms/test.ch8");

    Renderer::Init(window.width, window.height);

    while (window.IsRunning())
    {
        //chip8.EmulateCycle();

        Renderer::BeginBatch();

        Renderer::DrawQuad(0.0f, 0.0f, 20.0f, 20.0f, 1.0f, 0.0f, 0.0, 1.0f);

        Renderer::EndBatch();

        window.Update();
    }

    Renderer::Shutdown();

    return 0;
}
