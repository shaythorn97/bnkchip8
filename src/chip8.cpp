#include "chip8.h"
#include <cstdint>

Chip8::Chip8()
{

}

Chip8::~Chip8()
{

}

void Chip8::ClearDisplay()
{

}

void Chip8::Return()
{
    // go back in the stack
    pc = stack[sp];
    sp--;
}

void Chip8::Jump()
{
    uint16_t nnn = opcode & 0x0FFF;
    pc = nnn;
}

void Chip8::CallSubroutine()
{
    // this will call a function pointer
}

void Chip8::SkipEqualVXNN() // 3XNN
{
    uint8_t nn = (opcode & 0x00FF);
    uint16_t vx = V[(opcode & 0x0F00) >> 8];

    if (nn == vx)
        pc += 4;
    else
        pc += 2;
}

void Chip8::SkipNotEqualVXNN() // 4XNN
{
    uint8_t nn = (opcode & 0x00FF);
    uint16_t vx = V[(opcode & 0x0F00) >> 8];

    if (nn != vx)
        pc += 4;
    else
        pc += 2;
}

void Chip8::SkipEqualVXVY() // 5XY0
{
    uint16_t vx = V[(opcode & 0x0F00) >> 8];
    uint16_t vy = V[(opcode & 0x00F0) >> 4];

    if (vx == vy)
        pc += 4;
    else
        pc += 2;
}

void Chip8::SetVXNN() //6XNN
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
