#include "chip8.h"
#include "bnkdraw/bnkdraw.h"

int main()
{
    Chip8 chip8;

    chip8.LoadROM("roms/test.ch8");

    return 0;
}
