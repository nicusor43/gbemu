//
// Created by nicusor43 on 2/24/24.
//

#pragma once

#include "stdlib.h"
#include "stdint.h"

#define WRAM_SIZE 0x2000
#define VRAM_SIZE 0x2000
#define ERAM_SIZE 0x2000 // honestly I don't know how much this is supposed to be; I will take it as what the cpu can address
#define OAM_SIZE 0x00A0
#define HRAM_SIZE 0x007F
#define IO_REGISTERS_SIZE 0x0080

typedef struct gb_memory {
    uint8_t wram[WRAM_SIZE]; // work ram => 2 banks of 0x1000 bytes each
    uint8_t eram[ERAM_SIZE]; // external ram
    uint8_t vram[VRAM_SIZE]; // video ram
    uint8_t oam[OAM_SIZE];   // object attribute memory
    uint8_t hram[HRAM_SIZE]; // high ram

    uint8_t io_registers[IO_REGISTERS_SIZE]; // io registers
    uint8_t ie;

    // TODO: fix the eram and find out if the mirror needs to be implemented


} gb_memory;

typedef enum io_registers_map {
    P1 = 0x00, // Joypad

    // Serial Data Transfer
    SB = 0x01, // Serial transfer data
    SC = 0x02, // Serial transfer control

    // Timer
    DIV = 0x04,  // Divider Register
    TIMA = 0x05, // Timer counter
    TMA = 0x06,  // Timer Modulo
    TAC = 0x07,  // Timer Contro
    IF = 0x0F, // Interrupt Fla

    // Sound
    NR10 = 0x10, // Channel 1 Sweep register
    NR11 = 0x11, // Channel 1 Sound Length/Wave Pattern Duty
    NR12 = 0x12, // Channel 1 Volume Envelope
    NR13 = 0x13, // Channel 1 Frequency lo data
    NR14 = 0x14, // Channel 1 Frequency hi data
    NR20 = 0x15, // Unused
    NR21 = 0x16, // Channel 2 Sound Length/Wave Pattern Duty
    NR22 = 0x17, // Channel 2 Volume Envelope
    NR23 = 0x18, // Channel 2 Frequency lo data
    NR24 = 0x19, // Channel 2 Frequency hi data
    NR30 = 0x1A, // Channel 3 Sound on/off
    NR31 = 0x1B, // Channel 3 Sound Length
    NR32 = 0x1C, // Channel 3 Select output level
    NR33 = 0x1D, // Channel 3 Frequency lo data
    NR34 = 0x1E, // Channel 3 Frequency hi data
    NR40 = 0x1F, // Unused
    NR41 = 0x20, // Channel 4 Sound Length
    NR42 = 0x21, // Channel 4 Volume Envelope
    NR43 = 0x22, // Channel 4 Polynomial Counter
    NR44 = 0x23, // Channel 4 Counter/consecutive; Initial
    NR50 = 0x24, // Channel control / ON-OFF / Volume
    NR51 = 0x25, // Selection of Sound output terminal
    NR52 = 0x26, // Sound on/of
    WAVE_RAM = 0x30, // Wave Pattern RA

    // Pixel Processing Unit (PPU)
    LCDC = 0x40, // LCD Control
    STAT = 0x41, // LCD Status
    SCY = 0x42,  // Scroll Y
    SCX = 0x43,  // Scroll X
    LY = 0x44,   // LCD Y-Coordinate
    LYC = 0x45,  // LY Compare
    DMA = 0x46,  // DMA Transfer and Start Address
    BGP = 0x47,  // BG Palette Data
    OBP0 = 0x48, // Object Palette 0 Data
    OBP1 = 0x49, // Object Palette 1 Data
    WY = 0x4A,   // Window Y Position
    WX = 0x4B,   // Window X Position +
    KEY0 = 0x4C,
    KEY1 = 0x4D, // Prepare Speed Switch (CGB-only
    VBK = 0x4F, // VRAM Bank (CGB-only
    BANK = 0x50,

    // Direct Memory Access (DMA)
    HDMA1 = 0x51, // New DMA Source, High (CGB-only)
    HDMA2 = 0x52, // New DMA Source, Low (CGB-only)
    HDMA3 = 0x53, // New DMA Destination, High (CGB-only)
    HDMA4 = 0x54, // New DMA Destination, Low (CGB-only)
    HDMA5 = 0x55, // New DMA Length/Mode/Start (CGB-only
    RP = 0x56, // Infrared Communication Port (CGB-only
    BGPI = 0x68, // Background Palette Index (CGB-only)
    BGPD = 0x69, // Background Palette Data (CGB-only)
    OBPI = 0x6A, // Object Palette Index (CGB-only)
    OBPD = 0x6B, // Object Palette Data (CGB-only
    OPRI = 0x6C, // Object Priority Mode (CGB-only
    SVBK = 0x70, // WRAM Bank (CGB-only
    PCM12 = 0x76, // PCM amplitudes 1 & 2 (CGB-only)
    PCM34 = 0x77 // PCM amplitudes 3 & 4 (CGB-only)

} gb_io_registers_map;

