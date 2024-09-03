#include <iostream>
#include <cstdint>

int main()
{
    std::cout << "Hello World!\n";

    uint16_t opcode;
    uint8_t memory[4096];
    uint8_t V[16];
    uint16_t I;
    uint16_t pc;

    // 0x000-0x1FF - Chip 8 interpreter
    // 0x050-0x0A0 - Used for 4x5 pixel font set(0-F)
    // 0x200-0xFFF - Program ROM and work RAM
    
    uint8_t gfx[64 * 32];

    uint8_t delayTimer;
    uint8_t soundTimer;

    uint16_t stack[16];
    uint16_t sp;

    uint8_t key[16];

    std::cin.get();

    return 0;
}
