#pragma once

#include <cstdint>
#include <functional>
#include <string>
#include <unordered_map>

using Instruction = std::function<void()>;

class Chip8
{
public:
    uint8_t memory[4096]; // this is our ram
    uint8_t V[16]; // these are our registers
    uint8_t I; // this is the index register
    uint16_t pc; // this is the program counter
    uint8_t display[64 * 32]; // this is our display
    uint8_t delayTimer; // this is for timing the events in the game
    uint8_t soundTimer; // this is for sound but we are probs not going to use it because I cant be assed
    uint8_t stack[16]; // this is for storing our return addresses for when we enter functions
    uint16_t sp; // this is the stack pointer, it points to the current location in the stack
    uint16_t opcode; // this is the current opcode we are executing
    uint8_t keys[16]; // this is for storing our keycodes for input
    uint8_t fontset[80]; // this is for loading sprites we could probably load these in with a text file instead, we can also use raw string literals which we can pull in through separate files theyre pretty cool
    std::unordered_map<uint16_t, Instruction> instructions;

    // constructor is cringe but its C++ feature
    Chip8();
    ~Chip8();

    // loop functions
    void EmulateCycle();
    void Execute(uint16_t oc);
    void UpdateTimers();

    void LoadROM(const std::string& fileName);

    // opcode functions - read the wiki or that thing I sent you, some of the names are a bit pants but I think they make sense
    void CLS();
    void RET();
    void JPaddr();
    void CALLaddr();
    void SEVxByte();
    void SNEVxByte();
    void SEVxVy();
    void SetVXNN();
    void AddNNVX();
    void SetVXVY();
    void OR();
    void AND();
    void XOR();
    void AddVYVX();
    void SubtractVYVX();
    void ShiftVXRight();
    void SetVXVYMinusVX();
    void ShiftVXLeft();
    void SkipNotEqualVXVY();
    void SetINN();
    void JumpV0();
    void SetVXRand();
    void DrawDisplay();
    void SkipIfKeyPressed();
    void SkipIfKeyReleased();
    void SetVXDelayTimer();
    void WaitForKeyPress();
    void SetDelayTimerVX();
    void SetSoundTimerVX();
    void AddVXI();
    void SetIVX();
    void SetBCDVX();
    void SaveVX();
    void LoadVX();
};
