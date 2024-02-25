//
// Created by nicusor43 on 2/24/24.
//

#include "memory.h"

#define SET_IO_REGISTERS(register, value) (memory -> io_registers[register] = value)

gb_memory *memory;

void memory_init(void) {
    memory = malloc(sizeof(gb_memory));

    // Initialize the IO registers
    memory -> ie = 0;
    SET_IO_REGISTERS(P1, 0xCF);
    SET_IO_REGISTERS(SB, 0x00);
    SET_IO_REGISTERS(SC, 0x7E);
    SET_IO_REGISTERS(DIV, 0xAB);
    SET_IO_REGISTERS(TIMA, 0x00);
    SET_IO_REGISTERS(TMA, 0x00);
    SET_IO_REGISTERS(TAC, 0xF8);
    SET_IO_REGISTERS(IF, 0xE1);
    SET_IO_REGISTERS(NR10, 0x80);
    SET_IO_REGISTERS(NR11, 0xBF);
    SET_IO_REGISTERS(NR12, 0xF3);
    SET_IO_REGISTERS(NR13, 0xFF);
    SET_IO_REGISTERS(NR14, 0xBF);
    // SET_IO_REGISTERS(NR20, 0xFF); unused tbh
    SET_IO_REGISTERS(NR21, 0x3F);
    SET_IO_REGISTERS(NR22, 0x00);
    SET_IO_REGISTERS(NR23, 0xFF);
    SET_IO_REGISTERS(NR24, 0xBF);
    SET_IO_REGISTERS(NR30, 0x7F);
    SET_IO_REGISTERS(NR31, 0xFF);
    SET_IO_REGISTERS(NR32, 0x9F);
    SET_IO_REGISTERS(NR33, 0xFF);
    SET_IO_REGISTERS(NR34, 0xBF);
    SET_IO_REGISTERS(NR41, 0xFF);
    SET_IO_REGISTERS(NR42, 0x00);
    SET_IO_REGISTERS(NR43, 0x00);
    SET_IO_REGISTERS(NR44, 0xBF);
    SET_IO_REGISTERS(NR50, 0x77);
    SET_IO_REGISTERS(NR51, 0xF3);
    SET_IO_REGISTERS(NR52, 0xF1);
    SET_IO_REGISTERS(LCDC, 0x91);
    SET_IO_REGISTERS(STAT, 0x85);
    SET_IO_REGISTERS(SCY, 0x00);
    SET_IO_REGISTERS(SCX, 0x00);
    SET_IO_REGISTERS(LY, 0x00);
    SET_IO_REGISTERS(LYC, 0x00);
    SET_IO_REGISTERS(DMA, 0xFF);
    SET_IO_REGISTERS(BGP, 0xFC);

    // On real hardware, these are left uninitialized;
    SET_IO_REGISTERS(OBP0, 0x00);
    SET_IO_REGISTERS(OBP1, 0x00);


    SET_IO_REGISTERS(WY, 0x00);
    SET_IO_REGISTERS(WX, 0x00);


}
