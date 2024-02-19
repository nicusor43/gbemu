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

static inline void nop();

static inline void inc8(uint8_t *reg);

static inline void inc8_hl(uint8_t *hl);

static inline void dec8(uint8_t *reg);

static inline void dec8_hl(uint8_t *hl);

static inline void add8_r(uint8_t reg_value);

static inline void add8_hl_n(uint8_t value);

static inline void adc_r(uint8_t reg_value);

static inline void adc_hl_n(uint8_t value);

static inline void sub8_r(uint8_t reg_value);

static inline void sub8_hl_n(uint8_t value);

static inline void sbc_r(uint8_t reg_value);

static inline void sbc_hl_n(uint8_t value);

static inline void and_r(uint8_t reg_value);

static inline void and_hl_n(uint8_t value);

static inline void or_r(uint8_t reg_value);

static inline void or_hl_n(uint8_t value);

static inline void xor_r(uint8_t reg_value);

static inline void xor_hl_n(uint8_t value);

static inline void ccf();

static inline void scf();

static inline void cpl();

static inline void cp_r(uint8_t reg_value);

static inline void cp_hl_n(uint8_t value);

static inline void daa();

// 16-bit arithmetic
static inline void inc16(uint16_t *reg);

static inline void dec16(uint16_t *reg);

static inline void add16_hl(uint16_t reg_value);

static inline void add16_sp(int8_t value);

static inline void rlca();

static inline void rrca();






