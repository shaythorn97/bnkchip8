#include "chip8.h"

int main()
{
    Chip8 chip8;

    chip8.LoadROM("roms/test.ch8");

    chip8.EmulateCycle();

    return 0;
}
