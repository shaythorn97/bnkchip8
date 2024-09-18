#include "chip8.h"

int main()
{
    ROM rom("test.ch8");
    Chip8 chip8(rom);

    while (chip8.IsRunning())
    {
        // update
        chip8.EmulateCycle();

        // draw
        chip8.Display();
    }

    return 0;
}
