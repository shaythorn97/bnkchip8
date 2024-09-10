#include "chip8.h"
#include "renderer.h"

// need to write a renderer quick, fuck dsa

int main()
{
    Window window(640, 320, "Chip8");
    Chip8 chip8;

    chip8.LoadROM("roms/test.ch8");

    Renderer::Init();

    while (window.IsRunning())
    {
        chip8.EmulateCycle();

        window.Update();
    }

    Renderer::Shutdown();

    return 0;
}
