#include "chip8.h"
#include "renderer.h"

int main()
{
    Window window(640, 320, "Chip8");
    Chip8 chip8;

    if (!chip8.LoadROM("roms/ibm.ch8"))
        return -1;

    Renderer::Init(window.width, window.height);

    while (window.IsRunning())
    {
        chip8.EmulateCycle();

        Renderer::BeginBatch();

        if (chip8.drawFlag)
        {
            for (int i = 0; i < 64 * 32; i++)
            {
                int row = i / 64;
                int col = i % 64;

                int pixel = chip8.display[i];

                if (pixel > 0)
                    Renderer::DrawQuad(col * 10, row * 10, 10, 10, 1.0f, 1.0f, 1.0f, 1.0f);
            }
        }

        //Renderer::DrawQuad(0, 0, 10, 10, 1.0f, 1.0f, 1.0f, 1.0f);
        //Renderer::DrawQuad(50, 50, 10, 10, 1.0f, 1.0f, 1.0f, 1.0f);

        chip8.drawFlag = false;

        Renderer::EndBatch();

        window.Update();
    }

    Renderer::Shutdown();

    return 0;
}
