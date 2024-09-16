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

    int counter = 2;

    std::ofstream file;
    file.open("opcodes.csv");

    for (int i = 0x200; i < 0x200 + rom.size; i += 2)
    {
        std::cout << std::dec << counter << ": " << "Address: 0x" << std::hex << std::setw(3) << std::setfill('0') << i
            << " Instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << (int)chip8.memory[i]
            << std::hex << std::setw(2) << std::setfill('0') << (int)chip8.memory[i + 1] << "\n";

        // write to csv
        if (file.is_open())
            file << "0x" << std::hex << std::setw(2) << std::setfill('0') << (int)chip8.memory[i] << std::hex << std::setw(2) << std::setfill('0') << (int)chip8.memory[i + 1] << "\n";

        counter++;
    }

    file.close();

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

                int flip = 31 - row;

                int pixel = chip8.display[i];

                if (pixel > 0)
                    Renderer::DrawQuad(col * 10, flip * 10, 10, 10, 1.0f, 1.0f, 1.0f, 1.0f);
            }
        }

        chip8.drawFlag = false;

        Renderer::EndBatch();

        window.Update();
    }

    Renderer::Shutdown();

    return 0;
}
