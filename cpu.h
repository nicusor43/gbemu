//
// Created by nicusor43 on 2/16/24.
//

#pragma once

#include "stdint.h"
#include "stdbool.h"
#include "time.h"


extern const int clockSpeed; // 4.194304 MHz

// The registers of the Gameboy CPU, as described in the Pan Docs
struct gb_registers {
    union {
        struct {
            uint8_t f; // flags: bit 7 is zero, bit 6 is subtract, bit 5 is half-carry, bit 4 is carry; 3 through 0 are NEVER used.
            uint8_t a; // accumulator
        };
        uint16_t af; // accumulator and flags
    };
    union {
        struct {
            uint8_t c; // low byte
            uint8_t b;
        };
        uint16_t bc;
    };
    union {
        struct {
            uint8_t e; // low byte
            uint8_t d;
        };
        uint16_t de;
    };
    union {
        struct {
            uint8_t l; // low byte
            uint8_t h;
        };
        uint16_t hl;
    };
    uint16_t sp; // stack pointer
    uint16_t pc; // program counter
};

// A struture to hold the timing information for the CPU
struct gb_timer {
    uint16_t div; // divider register, actually the upper 8 bits of the number of clock cycles
    uint8_t tima; // timer counter
    uint8_t tma; // timer modulo
    uint8_t tac; // timer control
};

void cpu_start();

// Should have probably just written it in C++ atp.
bool getFlagZ();

void setFlagZ(bool value);

bool getFlagN();

void setFlagN(bool value);

bool getFlagH();

void setFlagH(bool value);

bool getFlagC();

void setFlagC(bool value);



void cycle_clock(int time);

void run_cpu();







