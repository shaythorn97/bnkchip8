// you use '#pragma once' so you dont recursively add header files, you can also do it like this which is the old C way
// #ifndef CHIP8_H
// define CHIP8_H
//
// code goes here
//
// endif
#pragma once

// this gives us the uint variables 
#include <cstdint>

// you probably remember what a class is but its important to remember that both struct and class are identical in C++ other than the fact that everything in a struct is public by default whereas in a class they are private
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
    uint8_t key[16]; // this is for storing our keycodes

    // constructor is cringe but its C++ feature
    Chip8();
    ~Chip8();

    // opcode functions - read the wiki or that thing I sent you, some of the names are a bit pants but I think they make sense
    void ClearDisplay();
    void Return();
    void Jump();
    void CallSubroutine();
    void SkipEqualVXNN();
    void SkipNotEqualVXNN();
    void SkipEqualVXVY();
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
    void SetVXSoundTimer();
    void AddVXI();
    void SetIVX();
    void SetBCDVX();
    void SaveVX();
    void LoadVX();
};
