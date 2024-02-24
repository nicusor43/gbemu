//
// Created by nicusor43 on 2/24/24.
//

#include "memory.h"

#define set_io_register(register, value) (memory -> io_registers[register] = value)

gb_memory *memory;

void memory_init() {
    memory = malloc(sizeof(gb_memory));

    // Initialize the IO registers
    memory -> ie = 0;
    set_io_register(P1, 0xCF);
    set_io_register(SB, 0x00);
    set_io_register(SC, 0x7E);
    set_io_register(DIV, 0xAB);
    set_io_register(TIMA, 0x00);
    set_io_register(TMA, 0x00);
    set_io_register(TAC, 0xF8);
    set_io_register(IF, 0xE1);
    set_io_register(NR10, 0x80);
    set_io_register(NR11, 0xBF);
    set_io_register(NR12, 0xF3);
    set_io_register(NR13, 0xFF);
    set_io_register(NR14, 0xBF);
    // set_io_register(NR20, 0xFF); unused tbh

    // set the rest of the IO registers for the DMG

}