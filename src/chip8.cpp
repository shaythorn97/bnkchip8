#include "chip8.h"

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
    
    // load fontset from rom into memory
     
    // reset timers
    delayTimer = 0;
    soundTimer = 0;
}

Chip8::~Chip8()
{

}

void Chip8::EmulateCycle()
{
    Fetch();

    Decode();

    Execute();

    UpdateTimers();
}

void Chip8::Fetch()
{
    opcode = memory[pc] << 8 | memory[pc + 1];
}

void Chip8::Decode()
{
    // here we need to get the opcode instruction from the opcode
    // this is going to be a bit hacky because we need to be able to nest if statements or something
    //
    // we can atleast separate our logic into here so we know which instruction to run, I think the switch statement method is pants
}

void Chip8::Execute()
{
    // run the instruction with the current opcode key
    // we can probs do something like this
    //
    // this is a function pointer, this is the C++ way
    // using Instruction = std::function<void()>;
    //
    // this is the C way
    // typedef void(*Instruction)();
    //
    // its probably worth having a read about them, they are sick
    //
    // std::unsigned_map<uint16_t, Instruction> instructions;
    //
    // if (instructions.contains(opcode))
    //     instructions[opcode];
    // else
    //     print some error message because the opcode doesn't exist
}

void Chip8::UpdateTimers()
{
    // this is for our timers, we want 60 opcodes executed per cycle as it runs at 60hz
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
    // this will call a function pointer
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

void Chip8::AddNNVX() //7XNN
{
    uint8_t nn = (opcode & 0x00FF);
    V[(opcode & 0x0F00) >> 8] = (V[(opcode & 0x0F00) >> 8] + nn);
}

void Chip8::SetVXVY() //8XY0
{
    V[(opcode & 0x0F00) >> 8] = (V[(opcode & 0x00F0) >> 4]);
}

void Chip8::OR() //8XY1
{
    V[(opcode & 0x0F00) >> 8] = (V[(opcode & 0x0F00) >> 8] | (V[(opcode & 0x00F0) >> 4]));
}

void Chip8::AND() //8XY2
{
    V[(opcode & 0x0F00) >> 8] = (V[(opcode & 0x0F00) >> 8] & (V[(opcode & 0x00F0) >> 4]));
}

void Chip8::XOR() //8XY3
{
    V[(opcode & 0x0F00) >> 8] = (V[(opcode & 0x0F00) >> 8] ^  (V[(opcode & 0x00F0) >> 4]));
}

void Chip8::AddVYVX() //8XY4 Sets VF to 1 if there is an overflow 
{
    uint16_t VX = (V[(opcode & 0x0F00) >> 8] + (V[(opcode & 0x00F0) >> 4]));
    if (VX > 0x00FF) {
        V[0xF] = 1;
    }
    else {
        V[0xF] = 0;
    }
    V[(opcode & 0x0F00) >> 8] = (V[(opcode & 0x0F00) >> 8] + (V[(opcode & 0x00F0) >> 4]));
}

void Chip8::SubtractVYVX() //8XY5 Sets VF to 0 if there is an underflow 
{
  
    if (V[(opcode & 0x0F00) >> 8] > (V[(opcode & 0x00F0) >> 4])) {
        V[0xF] = 1;
    }
    else {
        V[0xF] = 0;
    }
    V[(opcode & 0x0F00) >> 8] = (V[(opcode & 0x0F00) >> 8] - (V[(opcode & 0x00F0) >> 4]));
}

void Chip8::ShiftVXRight() //8XY6 Shifts VX to the right by 1 bit, if LSB is
{
    if ((V[(opcode & 0x0F00) >> 8] % 2) > 0) { //this may need to change, unsure if flag logic runs before or after shift 
        V[0xF] = 1;
    }
    else {
        V[0xF] = 0;
    }
    V[(opcode & 0x0F00)] >>= 1;
}

void Chip8::SetVXVYMinusVX() // 8XY7 Sets VF to 0 if there is an underflow
{
    if (V[(opcode & 0x0F00) >> 8] < (V[(opcode & 0x00F0) >> 4])) {
        V[0xF] = 1;
    }
    else {
        V[0xF] = 0;
    }
    V[(opcode & 0x0F00) >> 8] = ((V[(opcode & 0x00F0) >> 4]) - V[(opcode & 0x0F00) >> 8]);
}

void Chip8::ShiftVXLeft() // 8XYE
{
    if ((V[(opcode & 0x0F00) >> 11]) > 0) { //this may need to change, unsure if flag logic runs before or after shift 
        V[0xF] = 1;
    }
    else {
        V[0xF] = 0;
    }
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
