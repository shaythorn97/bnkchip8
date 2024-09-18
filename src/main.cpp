#include <iostream>
#include <iomanip>
#include <fstream>

#include "chip8.h"
#include "renderer.h"

int main()
{
    Window window(640, 320, "Chip8");
    ROM rom;

    if (!rom.Load("roms/test.ch8"))
        return -1;

    Chip8 chip8(rom);

    int counter = 1;

    for (int i = 0x200; i < 0x200 + rom.size; i += 2)
    {
        std::cout << std::dec << counter << ": " << "Address: 0x" << std::hex << std::setw(3) << std::setfill('0') << i
            << " Instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << (int)chip8.memory[i]
            << std::hex << std::setw(2) << std::setfill('0') << (int)chip8.memory[i + 1] << "\n";

        counter++;
    }

    Renderer::Init(window.width, window.height);

    while (window.IsRunning())
    {
        chip8.EmulateCycle();

        Renderer::BeginBatch();

        for (int i = 0; i < 64 * 32; i++)
        {
            int row = i / 64;
            int col = i % 64;

            int flip = 31 - row;

            int pixel = chip8.display[i];

            if (pixel == 1)
                Renderer::DrawQuad(col * 10, flip * 10, 10, 10, 0.0f, 1.0f, 0.0f, 1.0f);
        }

        Renderer::EndBatch();

        window.Update();
    }

    Renderer::Shutdown();

    return 0;
}
