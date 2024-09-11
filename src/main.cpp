#include "chip8.h"
#include "renderer.h"

int main()
{
    Window window(640, 320, "Chip8");
    Chip8 chip8;

    if (!chip8.LoadROM("roms/pong.ch8"))
        return -1;

    Renderer::Init(window.width, window.height);

    while (window.IsRunning())
    {
        chip8.EmulateCycle();

        Renderer::BeginBatch();

        if (chip8.drawFlag)
        {
            for (int i = 0; i < 64; i++)
            {
                for (int j = 0; j < 32; j++)
                {
                    if (chip8.display[i * j] > 0)
                        Renderer::DrawQuad(0 + (10.0f * i), 0 + (10.0f * j), 10, 10, 1.0f, 1.0f, 1.0f, 1.0f);
                }
            }
        }

        chip8.drawFlag = false;

        Renderer::EndBatch();

        window.Update();
    }

    Renderer::Shutdown();

    return 0;
}
