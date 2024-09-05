#include "chip8.h"

Chip8::Chip8()
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
    uint16_t vx = V[(opcode + 0x0F00) >> 8];

    if (nn == vx)
        pc += 4;
    else 
        pc += 2;
}

void Chip8::SkipNotEqualVXNN() // 4XNN
{
    uint8_t nn = (opcode & 0x00FF);
    uint16_t vx = V[(opcode + 0x0F00) >> 8];

    if (nn != vx)
        pc += 4;
    else 
        pc += 2;
}

void Chip8::SkipEqualVXVY() // 5XY0
{
    uint16_t vx = V[(opcode + 0x0F00) >> 8];
    uint16_t vy = V[(opcode & 0x00F0) >> 4];

    if (vx == vy)
        pc += 4;
    else
        pc += 2;
}

void Chip8::SetVXNN()
{}

void Chip8::AddNNVX()
{}

void Chip8::SetVXVY()
{}

void Chip8::OR()
{}

void Chip8::AND()
{}

void Chip8::XOR()
{}

void Chip8::AddVYVX()
{}

void Chip8::SubtractVYVX()
{}

void Chip8::ShiftVXRight()
{

}

void Chip8::SetVXVYMinusVX()
{}

void Chip8::ShiftVXLeft()
{}

void Chip8::SkipNotEqualVXVY() // 9XY0
{
    uint16_t vx = V[(opcode & 0x0F00) >> 8];
    uint16_t vy = V[(opcode & 0x00F0) >> 4];

    if (vx != vy)
        pc += 4;
    else
        pc += 2;
}

void Chip8::SetINN()
{}

void Chip8::JumpV0()
{}

void Chip8::SetVXRand()
{}

void Chip8::DrawDisplay()
{}

void Chip8::SkipIfKeyPressed()
{}

void Chip8::SkipIfKeyReleased()
{}

void Chip8::SetVXDelayTimer()
{}

void Chip8::SetVXSoundTimer()
{}

void Chip8::AddVXI()
{}

void Chip8::SetIVX()
{}

void Chip8::SaveVX()
{}

void Chip8::LoadVX()
{}
