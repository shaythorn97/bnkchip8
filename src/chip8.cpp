#include "chip8.h"

#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>
#include <iomanip>

Chip8::Chip8()
{
    // here we need to set our default values, we start at address 0x200
    pc = 0x200;
    opcode = 0;
    I = 0;
    sp = 0;

    // clear all the other shit
    for (int i = 0; i < 64 * 32; i++)
        display[i] = 0;

    for (int i = 0; i < 16; i++)
        V[i] = 0;

    for (int i = 0; i < 16; i++)
        stack[i] = 0;

    for (int i = 0; i < 4096; i++)
        memory[i] = 0;

    for (int i = 0; i < 16; i++)
        keys[i] = 0;
    
    // load fontset into memory
    uint8_t defaultFontset[80] = 
    {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };
     
    for (int i = 0; i < 80; i++)
        memory[i] = defaultFontset[i];

    // reset timers
    delayTimer = 0;
    soundTimer = 0;

    // seed rand
    srand(time(NULL));

    // here we are going to add our instructions into the map
    instructions[0x0000] = std::bind(&Chip8::CLS, this);  
    instructions[0x000E] = std::bind(&Chip8::RET, this);
    instructions[0x1000] = std::bind(&Chip8::JPADDR, this);
    instructions[0x2000] = std::bind(&Chip8::CALLADDR, this);
    instructions[0x3000] = std::bind(&Chip8::SEVXBYTE, this);
    instructions[0x4000] = std::bind(&Chip8::SNEVXBYTE, this);
    instructions[0x5000] = std::bind(&Chip8::SEVXVY, this);
    instructions[0x6000] = std::bind(&Chip8::LDVXBYTE, this);
    instructions[0x7000] = std::bind(&Chip8::ADDVXBYTE, this);
    instructions[0x8000] = std::bind(&Chip8::LDVXVY, this);
    instructions[0x8001] = std::bind(&Chip8::ORVXVY, this);
    instructions[0x8002] = std::bind(&Chip8::ANDVXVY, this);
    instructions[0x8003] = std::bind(&Chip8::XORVXVY, this);
    instructions[0x8004] = std::bind(&Chip8::AddVXVY, this);
    instructions[0x8005] = std::bind(&Chip8::SUBVXVY, this);
    instructions[0x8006] = std::bind(&Chip8::SHRVXVY, this);
    instructions[0x8007] = std::bind(&Chip8::SUBNVXVY, this);
    instructions[0x800E] = std::bind(&Chip8::SHLVXVY, this);
    instructions[0x9000] = std::bind(&Chip8::SNEVXVY, this);
    instructions[0xA000] = std::bind(&Chip8::LDIADDR, this);
    instructions[0xB000] = std::bind(&Chip8::JPV0ADDR, this);
    instructions[0xC000] = std::bind(&Chip8::RNDVXBYTE, this);
    instructions[0xD000] = std::bind(&Chip8::DRWVXVYNIBBLE, this);
    instructions[0xE09E] = std::bind(&Chip8::SKPVX, this);
    instructions[0xE0A1] = std::bind(&Chip8::SKNPVX, this);
    instructions[0xF007] = std::bind(&Chip8::LDVXDT, this);
    instructions[0xF00A] = std::bind(&Chip8::LDVXK, this);
    instructions[0xF015] = std::bind(&Chip8::LDDTVX, this);
    instructions[0xF018] = std::bind(&Chip8::LDSTVX, this);
    instructions[0xF01E] = std::bind(&Chip8::ADDIVX, this);
    instructions[0xF029] = std::bind(&Chip8::LDFVX, this);
    instructions[0xF033] = std::bind(&Chip8::LDBVX, this);
    instructions[0xF055] = std::bind(&Chip8::LDIVX, this);
    instructions[0xF065] = std::bind(&Chip8::LDVXI, this);
}

Chip8::~Chip8()
{

}

void Chip8::EmulateCycle()
{
    opcode = memory[pc] << 8 | memory[pc + 1];

    std::cout << "Opcode: 0x" << std::hex << std::setw(4) << std::setfill('0') << opcode << "\n";

    uint16_t key = opcode & 0xF000;

    if (key == 0x0000 || key == 0x8000)
        Execute(opcode & 0xF00F);
    else if (key == 0xE000 || key == 0xF000)
        Execute(opcode & 0xF0FF);
    else
        Execute(key);

    UpdateTimers();
}

void Chip8::Execute(uint16_t oc)
{
    if (instructions.contains(oc))
        instructions[oc]();
    else
        std::cout << "Invalid opcode\n";
}

void Chip8::UpdateTimers()
{
    // this is for our timers, we want 60 opcodes executed per cycle as it runs at 60hz
    if (delayTimer > 0)
        delayTimer--;

    if (soundTimer > 0)
    {
        if (soundTimer == 1)
            std::cout << "Beep!\n";

        soundTimer--;
    }
}

bool Chip8::LoadROM(const std::string& fileName)
{
    std::ifstream file(fileName, std::ios::binary | std::ios::ate);

    if (!file.is_open())
    {
        std::cout << "File could not be found\n";
        return false;
    }

    std::streamsize size = file.tellg();

    if (size < 1 || size > 4096 - 0x200)
    {
        std::cout << "ROM size is invalid\n";
        return false;
    }

    file.seekg(0, std::ios::beg);

    if (!file.read((char*)(memory + 0x200), size))
    {
        std::cout << "File could not be read\n";
        return false;
    }

    std::cout << "ROM '" << fileName << "' has been successfully loaded\n";
    return true;
}

void Chip8::CLS() // 00E0
{
    // clear screen
    for (int i = 0; i < 64 * 32; i++)
        display[i] = 0;
    
    pc += 2;
}

void Chip8::RET() // 00EE
{
    sp--;
    pc = stack[sp];
    pc += 2;
}

void Chip8::JPADDR() // 1NNN
{
    uint16_t nnn = opcode & 0x0FFF;

    pc = nnn;
}

void Chip8::CALLADDR() // 2NNN
{
    uint16_t nnn = opcode & 0x0FFF;

    stack[sp] = pc;
    sp++;
    pc = nnn;
}

void Chip8::SEVXBYTE() // 3XNN
{
    uint8_t nn = (opcode & 0x00FF);
    uint8_t vx = V[(opcode & 0x0F00) >> 8];

    if (nn == vx)
        pc += 4;
    else
        pc += 2;
}

void Chip8::SNEVXBYTE() // 4XNN
{
    uint8_t nn = (opcode & 0x00FF);
    uint8_t vx = V[(opcode & 0x0F00) >> 8];

    if (nn != vx)
        pc += 4;
    else
        pc += 2;
}

void Chip8::SEVXVY() // 5XY0
{
    uint8_t vx = V[(opcode & 0x0F00) >> 8];
    uint8_t vy = V[(opcode & 0x00F0) >> 4];

    if (vx == vy)
        pc += 4;
    else
        pc += 2;
}

void Chip8::LDVXBYTE() // 6XNN
{
    uint8_t nn = (opcode & 0x00FF);

    V[(opcode & 0x0F00) >> 8] = nn;
    pc += 2;
}

void Chip8::ADDVXBYTE() // 7XNN
{
    uint8_t nn = (opcode & 0x00FF);

    V[(opcode & 0x0F00) >> 8] += nn;
    pc += 2;
}

void Chip8::LDVXVY() // 8XY0
{
    V[(opcode & 0x0F00) >> 8] = (V[(opcode & 0x00F0) >> 4]);
    pc += 2;
}

void Chip8::ORVXVY() // 8XY1
{
    V[(opcode & 0x0F00) >> 8] |= V[(opcode & 0x00F0) >> 4];
    pc += 2;
}

void Chip8::ANDVXVY() // 8XY2
{
    V[(opcode & 0x0F00) >> 8] &= V[(opcode & 0x00F0) >> 4];
    pc += 2;
}

void Chip8::XORVXVY() // 8XY3
{
    V[(opcode & 0x0F00) >> 8] ^=  V[(opcode & 0x00F0) >> 4];
    pc += 2;
}

void Chip8::AddVXVY() // 8XY4 Sets VF to 1 if there is an overflow 
{
    uint8_t sum = V[(opcode & 0x0F00) >> 8] + V[(opcode & 0x00F0) >> 4];

    if (sum > 0x00FF)
        V[0xF] = 1;
    else
        V[0xF] = 0;
    
    V[(opcode & 0x0F00) >> 8] += V[(opcode & 0x00F0) >> 4];
    pc += 2;
}

void Chip8::SUBVXVY() // 8XY5 Sets VF to 0 if there is an underflow 
{
    uint8_t vx = V[(opcode & 0x0F00) >> 8];
    uint8_t vy = V[(opcode & 0x00F0) >> 4];

    if (vx > vy)
        V[0xF] = 1;
    else
        V[0xF] = 0;

    V[(opcode & 0x0F00) >> 8] -= V[(opcode & 0x00F0) >> 4];
    pc += 2;
}

void Chip8::SHRVXVY() // 8XY6 Shifts VX to the right by 1 bit, if LSB is 1
{
    uint8_t vx = V[(opcode & 0x0F00) >> 8];

    // this gets us the LSB 
    if (vx & 0x1)
        V[0xF] = 1;
    else
        V[0xF] = 0;

    V[(opcode & 0x0F00) >> 8] >>= 1;
    pc += 2;
}

void Chip8::SUBNVXVY() // 8XY7 Sets VF to 0 if there is an underflow
{
    uint8_t vx = V[(opcode & 0x0F00) >> 8];
    uint8_t vy = V[(opcode & 0x00F0) >> 4];

    if (vy > vx)
        V[0xF] = 1;
    else
        V[0xF] = 0;

    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4] - V[(opcode & 0x0F00) >> 8];
    pc += 2;
}

void Chip8::SHLVXVY() // 8XYE, we check MSB
{
    uint8_t vx = V[(opcode & 0x0F00) >> 8];

    // move the highest bit to the end and check if its 1
    // 10000000
    // 00000001
    if ((vx >> 7) & 1)
        V[0xF] = 1;
    else
        V[0xF] = 0;

    V[(opcode & 0x0F00) >> 8] <<= 1;
    pc += 2;
}

void Chip8::SNEVXVY() // 9XY0
{
    uint8_t vx = V[(opcode & 0x0F00) >> 8];
    uint8_t vy = V[(opcode & 0x00F0) >> 4];

    if (vx != vy)
        pc += 4;
    else
        pc += 2;
}

void Chip8::LDIADDR() // ANNN
{
    uint16_t nnn = opcode & 0x0FFF;

    I = nnn;
    pc += 2; 
}

void Chip8::JPV0ADDR() // BNNN
{
    uint16_t nnn = opcode & 0x0FFF;
    uint16_t loc = nnn + V[0];

    pc = loc;
    pc += 2;
}

void Chip8::RNDVXBYTE() // CXKK
{
    // random number between 0 and 255
    uint8_t rnd = (rand() % 0xFF) & 0x00FF; 

    V[(opcode & 0x0F00) >> 8] = rnd;
    pc += 2;
}

void Chip8::DRWVXVYNIBBLE() // DXYN
{
    uint8_t vx = V[(opcode & 0x0F00) >> 8];
    uint8_t vy = V[(opcode & 0x00F0) >> 4];
    uint8_t n = opcode & 0x000F;

    V[0xF] = 0; 
    
    for (int i = 0; i < n; i++)
    {
        uint8_t pixel = memory[I + i];

        for (int j = 0; j < 8; j++)
        {
            if ((pixel & (0x80 >> j)))
            {
                int x = (vx + j) % 64;
                int y = (vy + i) % 32;

                int index = x + (y * 64);

                if (display[index] == 1)
                    V[0xF] = 1;

                display [index] ^= 1;
            }
        }
    }

    drawFlag = true;

    pc += 2;
}

void Chip8::SKPVX() // EX9E
{
    uint8_t vx = V[(opcode & 0x0F00) >> 8];

    if (keys[vx] > 0)
        pc += 4;
    else
        pc += 2;
}

void Chip8::SKNPVX() // EXA1
{
    uint8_t vx = V[(opcode & 0x0F00) >> 8];

    if (keys[vx] == 0)
        pc += 4;
    else
        pc += 2;
}

void Chip8::LDVXDT() // FX07
{
    V[(opcode & 0x0F00) >> 8] = delayTimer;
    pc += 2;
}

void Chip8::LDVXK() // FX0A
{
    bool isKeyPressed = false;

    for (int i = 0; i < 16; i++)
    {
       if (keys[i] > 0)
       {
           V[(opcode & 0x0F00) >> 8] = i;
           isKeyPressed = true;
       }
    }

    // if nothing was pressed run this instruction again
    if (!isKeyPressed)
        return;

    pc += 2;
}

void Chip8::LDDTVX() // FX15
{
    uint8_t vx = V[(opcode & 0x0F00) >> 8];

    delayTimer = vx;
    pc += 2;
}

void Chip8::LDSTVX() // FX18  
{
    uint8_t vx = V[(opcode & 0x0F00) >> 8];

    soundTimer = vx;
    pc += 2;
}

void Chip8::ADDIVX() // FX1E
{
    uint8_t vx = V[(opcode & 0x0F00) >> 8];

    if (I + vx > 0xFFF)
        V[0xF] = 1;
    else
        V[0xF] = 0;
    
    I += vx;
    pc += 2;
}

void Chip8::LDFVX() // FX29
{
    uint8_t vx = V[(opcode & 0x0F00) >> 8];

    I = vx * 0x5;
    pc += 2;
}

void Chip8::LDBVX() // FX33
{
    // this is a pretty hard one, it needs to store some memory in a couple places
    // we need to split it into 3 parts
    // 100 goes in I
    // 10 goes in I + 1
    // 1 goes in I + 2

    uint8_t vx = V[(opcode & 0x0F00) >> 8];

    memory[I] = vx / 100;
    memory[I + 1] = (vx / 10) & 10;
    memory[I + 2] = (vx % 100) % 10;
    pc += 2;
}

void Chip8::LDIVX() // FX55
{
    uint8_t vx = V[(opcode & 0x0F00) >> 8];

    for (int i = 0; i < vx; i++)
        memory[I + i] = V[i]; 

    I += vx + 1;
    pc += 2;
}

void Chip8::LDVXI() // FX65
{
    uint8_t vx = V[(opcode & 0x0F00) >> 8];

    for (int i = 0; i < vx; i++)
        V[i] = memory[i + I]; 

    I += vx + 1;
    pc += 2;
}
