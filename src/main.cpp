#include "chip8.h"

int main()
{
    ROM rom("test.ch8");
    Chip8 chip8(rom);

    if (!chip8.rom)
        return -1;

    while (chip8.IsRunning())
    {
        // update
        chip8.EmulateCycle();

        // draw
        chip8.Display();
    }

    return 0;
}
