#pragma once

#include <cstdint>
#include <functional>
#include <string>
#include <unordered_map>

using Instruction = std::function<void()>;

class Chip8
{
public:
    uint8_t display[64 * 32]; // this is our display
    bool drawFlag = false;
    uint8_t memory[4096]; // this is our ram
                              
    // constructor is cringe but its C++ feature
    Chip8();
    ~Chip8();

    // loop functions
    void EmulateCycle();

    bool LoadROM(const std::string& fileName);
private:
    uint8_t V[16]; // these are our registers
    uint8_t I; // this is the index register
    uint16_t pc; // this is the program counter
    uint8_t delayTimer; // this is for timing the events in the game
    uint8_t soundTimer; // this is for sound but we are probs not going to use it because I cant be assed
    uint8_t stack[16]; // this is for storing our return addresses for when we enter functions
    uint16_t sp; // this is the stack pointer, it points to the current location in the stack
    uint16_t opcode; // this is the current opcode we are executing
    uint8_t keys[16]; // this is for storing our keycodes for input
    std::unordered_map<uint16_t, Instruction> instructions;

    void UpdateTimers();
    void Execute(uint16_t oc);

    // opcode functions - read the wiki or that thing I sent you, some of the names are a bit pants but I think they make sense
    void CLS(); // 00E0
    void RET(); // 00EE
    void JPADDR(); // 1NNN
    void CALLADDR(); // 2NNN
    void SEVXBYTE(); // 3XNN
    void SNEVXBYTE(); // 4XNN
    void SEVXVY(); // 5XY0
    void LDVXBYTE(); // 6XNN
    void ADDVXBYTE(); // 7XNN
    void LDVXVY(); // 8XY0
    void ORVXVY(); // 8XY1
    void ANDVXVY(); // 8XY2
    void XORVXVY(); // 8XY3
    void AddVXVY(); // 8XY4
    void SUBVXVY(); // 8XY5
    void SHRVXVY(); // 8XY6
    void SUBNVXVY(); // 8XY7
    void SHLVXVY(); // 8XYE
    void SNEVXVY(); // 9XY0
    void LDIADDR(); // ANNN
    void JPV0ADDR(); // BNNN
    void RNDVXBYTE(); // CXNN
    void DRWVXVYNIBBLE(); // DXYN
    void SKPVX(); // EX9E
    void SKNPVX(); // EXA1
    void LDVXDT(); // FX07
    void LDVXK(); // FX0A
    void LDDTVX(); // FX15
    void LDSTVX(); // FX18
    void ADDIVX(); // FX1E
    void LDFVX(); // FX29
    void LDBVX(); // FX33
    void LDIVX(); // FX55
    void LDVXI(); // FX65
};
