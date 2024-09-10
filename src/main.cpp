#include "chip8.h"
#include "renderer.h"

// need to write a renderer quick, fuck dsa

int main()
{
    Window window(640, 320, "Chip8");
    Chip8 chip8;

    chip8.LoadROM("roms/test.ch8");

    while (window.IsRunning())
    {
        if (Input::IsKeyPressed(window, Key::KEY_1))
            chip8.EmulateCycle();

        window.Update();
    }

    return 0;
}
