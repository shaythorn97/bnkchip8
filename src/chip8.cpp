#include "chip8.h"

#include <cstdlib>
#include <ctime>
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

    // seed rand
    srand(time(NULL));

    // here we are going to add our instructions into the map
    instructions[0x0000] = std::bind(&Chip8::CLS, this);  
    instructions[0x000E] = std::bind(&Chip8::RET, this);
    instructions[0x1000] = std::bind(&Chip8::JPaddr, this);
    instructions[0x2000] = std::bind(&Chip8::CALLaddr, this);
    instructions[0x3000] = std::bind(&Chip8::SEVxByte, this);
    instructions[0x4000] = std::bind(&Chip8::SNEVxByte, this);
    instructions[0x5000] = std::bind(&Chip8::SEVxVy, this);
    instructions[0x6000] = std::bind(&Chip8::SetVXNN, this);
    instructions[0x7000] = std::bind(&Chip8::AddNNVX, this);
    instructions[0x8000] = std::bind(&Chip8::SetVXVY, this);
    instructions[0x8001] = std::bind(&Chip8::OR, this);
    instructions[0x8002] = std::bind(&Chip8::AND, this);
    instructions[0x8003] = std::bind(&Chip8::XOR, this);
    instructions[0x8004] = std::bind(&Chip8::AddVYVX, this);
    instructions[0x8005] = std::bind(&Chip8::SubtractVYVX, this);
    instructions[0x8006] = std::bind(&Chip8::ShiftVXRight, this);
    instructions[0x8007] = std::bind(&Chip8::SetVXVYMinusVX, this);
    instructions[0x800E] = std::bind(&Chip8::ShiftVXLeft, this);
    instructions[0x9000] = std::bind(&Chip8::SkipNotEqualVXVY, this);
    instructions[0xA000] = std::bind(&Chip8::SetINN, this);
    instructions[0xB000] = std::bind(&Chip8::JumpV0, this);
    instructions[0xC000] = std::bind(&Chip8::SetVXRand, this);
    instructions[0xD000] = std::bind(&Chip8::DrawDisplay, this);
    instructions[0xE09E] = std::bind(&Chip8::SkipIfKeyPressed, this);
    instructions[0xE0A1] = std::bind(&Chip8::SkipIfKeyReleased, this);
    instructions[0xF007] = std::bind(&Chip8::SetVXDelayTimer, this);
    instructions[0xF00A] = std::bind(&Chip8::WaitForKeyPress, this);
    instructions[0xF015] = std::bind(&Chip8::SetDelayTimerVX, this);
    instructions[0xF018] = std::bind(&Chip8::SetSoundTimerVX, this);
    instructions[0xF01E] = std::bind(&Chip8::AddVXI, this);
    instructions[0xF029] = std::bind(&Chip8::SetIVX, this);
    instructions[0xF033] = std::bind(&Chip8::SetBCDVX, this);
    instructions[0xF055] = std::bind(&Chip8::SaveVX, this);
    instructions[0xF065] = std::bind(&Chip8::LoadVX, this);
}

Chip8::~Chip8()
{

}

void Chip8::EmulateCycle()
{
    opcode = memory[pc] << 8 | memory[pc + 1];

    // here we need to get the opcode instruction from the opcode
    // this is going to be a bit hacky because we need to be able to nest if statements or something
    //
    // we can atleast separate our logic into here so we know which instruction to run, I think the switch statement method is pants
    //
    // we need to go in at the addresses 0x0000, 0x8000, 0xE000, 0xF000
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

void Chip8::JPaddr() // 1NNN
{
    uint16_t nnn = opcode & 0x0FFF;

    pc = nnn;
}

void Chip8::CALLaddr() // 2NNN
{
    uint16_t nnn = opcode & 0x0FFF;

    stack[sp] = pc;
    sp++;
    pc = nnn;
}

void Chip8::SEVxByte() // 3XNN
{
    uint8_t nn = (opcode & 0x00FF);
    uint8_t vx = V[(opcode & 0x0F00) >> 8];

    if (nn == vx)
        pc += 4;
    else
        pc += 2;
}

void Chip8::SNEVxByte() // 4XNN
{
    uint8_t nn = (opcode & 0x00FF);
    uint8_t vx = V[(opcode & 0x0F00) >> 8];

    if (nn != vx)
        pc += 4;
    else
        pc += 2;
}

void Chip8::SEVxVy() // 5XY0
{
    uint8_t vx = V[(opcode & 0x0F00) >> 8];
    uint8_t vy = V[(opcode & 0x00F0) >> 4];

    if (vx == vy)
        pc += 4;
    else
        pc += 2;
}

void Chip8::SetVXNN() // 6XNN
{
    uint8_t nn = (opcode & 0x00FF);

    V[(opcode & 0x0F00) >> 8] = nn;
    pc += 2;
}

void Chip8::AddNNVX() // 7XNN
{
    uint8_t nn = (opcode & 0x00FF);

    V[(opcode & 0x0F00) >> 8] += nn;
    pc += 2;
}

void Chip8::SetVXVY() // 8XY0
{
    V[(opcode & 0x0F00) >> 8] = (V[(opcode & 0x00F0) >> 4]);
    pc += 2;
}

void Chip8::OR() // 8XY1
{
    V[(opcode & 0x0F00) >> 8] |= V[(opcode & 0x00F0) >> 4];
    pc += 2;
}

void Chip8::AND() // 8XY2
{
    V[(opcode & 0x0F00) >> 8] &= V[(opcode & 0x00F0) >> 4];
    pc += 2;
}

void Chip8::XOR() // 8XY3
{
    V[(opcode & 0x0F00) >> 8] ^=  V[(opcode & 0x00F0) >> 4];
    pc += 2;
}

void Chip8::AddVYVX() // 8XY4 Sets VF to 1 if there is an overflow 
{
    uint8_t sum = V[(opcode & 0x0F00) >> 8] + V[(opcode & 0x00F0) >> 4];

    if (sum > 0x00FF)
        V[0xF] = 1;
    else
        V[0xF] = 0;
    
    V[(opcode & 0x0F00) >> 8] += V[(opcode & 0x00F0) >> 4];
    pc += 2;
}

void Chip8::SubtractVYVX() // 8XY5 Sets VF to 0 if there is an underflow 
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

void Chip8::ShiftVXRight() // 8XY6 Shifts VX to the right by 1 bit, if LSB is 1
{
    uint8_t vx = V[(opcode & 0x0F00) >> 8];

    // this gets us the LSB 
    if (vx & 0x1)
        V[0xF] = 1;
    else
        V[0xF] = 0;

    // modulus is a very expensive operation so I changed it
    //if ((V[(opcode & 0x0F00) >> 8] % 2) > 0) // this may need to change, unsure if flag logic runs before or after shift 
    //    V[0xF] = 1;
    //else
    //    V[0xF] = 0;

    V[(opcode & 0x0F00) >> 8] >>= 1;
    pc += 2;
}

void Chip8::SetVXVYMinusVX() // 8XY7 Sets VF to 0 if there is an underflow
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

void Chip8::ShiftVXLeft() // 8XYE, we check MSB
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

void Chip8::SkipNotEqualVXVY() // 9XY0
{
    uint8_t vx = V[(opcode & 0x0F00) >> 8];
    uint8_t vy = V[(opcode & 0x00F0) >> 4];

    if (vx != vy)
        pc += 4;
    else
        pc += 2;
}

void Chip8::SetINN() // ANNN
{
    uint16_t nnn = opcode & 0x0FFF;

    I = nnn;
    pc += 2; 
}

void Chip8::JumpV0() // BNNN
{
    uint16_t nnn = opcode & 0x0FFF;
    uint16_t loc = nnn + V[0];

    pc = loc;
    pc += 2;
}

void Chip8::SetVXRand() // CXKK
{
    // random number between 0 and 255
    uint8_t rnd = (rand() % 0xFF) & 0x00FF; 

    V[(opcode & 0x0F00) >> 8] = rnd;
    pc += 2;
}

void Chip8::DrawDisplay() // DXYN
{
    // this is drawing operation, leave this for now
}

void Chip8::SkipIfKeyPressed() // EX9E
{
    //if(V[(opcode & 0x0F00) >> 8] = key)
    uint8_t vx = V[(opcode & 0x0F00) >> 8];

    if (keys[vx] > 0)
        pc += 4;
    else
        pc += 2;
}

void Chip8::SkipIfKeyReleased() // EXA1
{
    uint8_t vx = V[(opcode & 0x0F00) >> 8];

    if (keys[vx] == 0)
        pc += 4;
    else
        pc += 2;
}

void Chip8::SetVXDelayTimer() // FX07
{
    V[(opcode & 0x0F00) >> 8] = delayTimer;
    pc += 2;
}

void Chip8::WaitForKeyPress() // FX0A
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

void Chip8::SetDelayTimerVX() // FX15
{
    uint8_t vx = V[(opcode & 0x0F00) >> 8];

    delayTimer = vx;
    pc += 2;
}

void Chip8::SetSoundTimerVX() // FX18  
{
    uint8_t vx = V[(opcode & 0x0F00) >> 8];

    soundTimer = vx;
    pc += 2;
}

void Chip8::AddVXI() // FX1E
{
    uint8_t vx = V[(opcode & 0x0F00) >> 8];

    if (I + vx > 0xFFF)
        V[0xF] = 1;
    else
        V[0xF] = 0;
    
    I += vx;
    pc += 2;
}

void Chip8::SetIVX() // FX29
{
    uint8_t vx = V[(opcode & 0x0F00) >> 8];

    I = vx * 0x5;
    pc += 2;
}

void Chip8::SetBCDVX() // FX33
{
    // this is a pretty hard one, it needs to store some memory in a couple places
    // we need to split it into 3 parts
    // 100 goes in I
    // 10 goes in I + 1
    // 1 goes in I + 2
    //

    uint8_t vx = V[(opcode & 0x0F00) >> 8];

    memory[I] = vx;
    memory[I + 1] = vx;
    memory[I + 2] = vx;

    pc += 2;
}

void Chip8::SaveVX() // FX55
{

    pc += 2;
}

void Chip8::LoadVX() // FX65
{

    pc += 2;
}
