#include "chip8.h"

#include <iostream>

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
    
    // load fontset into memory
     
    // reset timers
    delayTimer = 0;
    soundTimer = 0;

    // here we are going to add our instructions into the map
}

Chip8::~Chip8()
{

}

void Chip8::EmulateCycle()
{
    Fetch();

    DecodeAndExecute();

    UpdateTimers();
}

// mayebe we get rid of these they are very simple
void Chip8::Fetch()
{
    opcode = memory[pc] << 8 | memory[pc + 1];
}

void Chip8::DecodeAndExecute()
{
    // here we need to get the opcode instruction from the opcode
    // this is going to be a bit hacky because we need to be able to nest if statements or something
    //
    // we can atleast separate our logic into here so we know which instruction to run, I think the switch statement method is pants
    //
    // we need to go in at the addresses 0x0000, 0x8000, 0xE000, 0xF000
    uint16_t key = opcode & 0xF000;

    if (key == 0x0000 || key == 0x8000)
    {
        // we need to check the last four bits
        // 0x0000
        // 0x000E
        Execute(opcode & 0xF00F);
    }
    else if (key == 0xE000 || key == 0xF000)
    {
        // we need to check the last 8 bits 
        // 0xE09E
        // 0x00A1
        Execute(opcode & 0xF0FF);
    }
    else
    {
        Execute(key);
    }
}

void Chip8::Execute(uint16_t oc)
{
    if (instructions.contains(opcode))
        instructions[opcode]();
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

void Chip8::CLS()
{
    
}

void Chip8::RET()
{
    // go back in the stack
    pc = stack[sp];
    sp--;
}

void Chip8::JPaddr()
{
    uint16_t nnn = opcode & 0x0FFF;
    pc = nnn;
}

void Chip8::CALLaddr()
{
    stack[sp] = pc;
    sp++;
    pc = opcode & 0x0FFF;
}

void Chip8::SEVxByte() // 3XNN
{
    uint8_t nn = (opcode & 0x00FF);
    uint16_t vx = V[(opcode & 0x0F00) >> 8];

    if (nn == vx)
        pc += 4;
    else
        pc += 2;
}

void Chip8::SNEVxByte() // 4XNN
{
    uint8_t nn = (opcode & 0x00FF);
    uint16_t vx = V[(opcode & 0x0F00) >> 8];

    if (nn != vx)
        pc += 4;
    else
        pc += 2;
}

void Chip8::SEVxVy() // 5XY0
{
    uint16_t vx = V[(opcode & 0x0F00) >> 8];
    uint16_t vy = V[(opcode & 0x00F0) >> 4];

    if (vx == vy)
        pc += 4;
    else
        pc += 2;
}

void Chip8::SetVXNN() // 6XNN
{
    uint8_t nn = (opcode & 0x00FF);
    V[(opcode & 0x0F00) >> 8] = nn;
}

void Chip8::AddNNVX() // 7XNN
{
    uint8_t nn = (opcode & 0x00FF);
    V[(opcode & 0x0F00) >> 8] += nn;
}

void Chip8::SetVXVY() // 8XY0
{
    V[(opcode & 0x0F00) >> 8] = (V[(opcode & 0x00F0) >> 4]);
}

void Chip8::OR() // 8XY1
{
    V[(opcode & 0x0F00) >> 8] |= V[(opcode & 0x00F0) >> 4];
}

void Chip8::AND() // 8XY2
{
    V[(opcode & 0x0F00) >> 8] &= V[(opcode & 0x00F0) >> 4];
}

void Chip8::XOR() // 8XY3
{
    V[(opcode & 0x0F00) >> 8] ^=  V[(opcode & 0x00F0) >> 4];
}

void Chip8::AddVYVX() // 8XY4 Sets VF to 1 if there is an overflow 
{
    uint16_t vx = (V[(opcode & 0x0F00) >> 8] + (V[(opcode & 0x00F0) >> 4]));

    if (vx > 0x00FF)
        V[0xF] = 1;
    else
        V[0xF] = 0;
    
    V[(opcode & 0x0F00) >> 8] += V[(opcode & 0x00F0) >> 4];
}

void Chip8::SubtractVYVX() // 8XY5 Sets VF to 0 if there is an underflow 
{
    if (V[(opcode & 0x0F00) >> 8] > (V[(opcode & 0x00F0) >> 4]))
        V[0xF] = 1;
    else
        V[0xF] = 0;

    V[(opcode & 0x0F00) >> 8] -= V[(opcode & 0x00F0) >> 4];
}

void Chip8::ShiftVXRight() // 8XY6 Shifts VX to the right by 1 bit, if LSB is
{
    if ((V[(opcode & 0x0F00) >> 8] % 2) > 0) // this may need to change, unsure if flag logic runs before or after shift 
        V[0xF] = 1;
    else
        V[0xF] = 0;

    V[(opcode & 0x0F00)] >>= 1;
}

void Chip8::SetVXVYMinusVX() // 8XY7 Sets VF to 0 if there is an underflow
{
    if (V[(opcode & 0x0F00) >> 8] < (V[(opcode & 0x00F0) >> 4]))
        V[0xF] = 1;
    else
        V[0xF] = 0;

    V[(opcode & 0x00F0) >> 4] -= V[(opcode & 0x0F00) >> 8];
}

void Chip8::ShiftVXLeft() // 8XYE
{
    if ((V[(opcode & 0x0F00) >> 11]) > 0) // this may need to change, unsure if flag logic runs before or after shift 
        V[0xF] = 1;
    else
        V[0xF] = 0;

    V[(opcode & 0x0F00)] <<= 1;
}

void Chip8::SkipNotEqualVXVY() // 9XY0
{
    uint16_t vx = V[(opcode & 0x0F00) >> 8];
    uint16_t vy = V[(opcode & 0x00F0) >> 4];

    if (vx != vy)
        pc += 4;
    else
        pc += 2;
}

void Chip8::SetINN() // ANNN
{
    uint16_t nnn = opcode & 0x0FFF;
    I = nnn;
}

void Chip8::JumpV0() // BNNN
{
    uint16_t nnn = opcode & 0x0FFF;
    uint16_t loc = nnn + V[0];

    pc = loc;
}

void Chip8::SetVXRand() // CXKK
{
    // random number between 0 and 255
}

void Chip8::DrawDisplay() // DXYN
{
    // this is drawing operation, leave this for now
}

void Chip8::SkipIfKeyPressed() // EX9E
{
    //if(V[(opcode & 0x0F00) >> 8] = key)
}

void Chip8::SkipIfKeyReleased() // EXA1
{

}

void Chip8::SetVXDelayTimer() // FX07
{
    
}

void Chip8::WaitForKeyPress() // FX0A
{}

void Chip8::SetDelayTimerVX() // FX15
{}

void Chip8::SetSoundTimerVX() // FX18  
{

}

void Chip8::AddVXI() // FX1E
{
    uint8_t vx = V[(opcode & 0x0F00) >> 8];
    
    I += vx;
}

void Chip8::SetIVX() // FX29
{
    uint8_t vx = V[(opcode & 0x0F00) >> 8];

    // this will get the sprite from the fontset
}

void Chip8::SetBCDVX() // FX33
{
    // this is a pretty hard one, it needs to store some memory in a couple places
}

void Chip8::SaveVX() // FX55
{
      
}

void Chip8::LoadVX() // FX65
{

}
