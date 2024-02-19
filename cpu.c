//
// Created by nicusor43 on 2/16/24.
//

#include "cpu.h"

#define CHECK_BIT(x, n) (registers.x >> n & 1)

struct gb_registers registers;
struct gb_timer timer;

void cpu_start() {
    registers.af = 0x01B0;
    registers.bc = 0x0013;
    registers.de = 0x00D8;
    registers.hl = 0x014D;
    registers.sp = 0xFFFE;
    registers.pc = 0x1000;

    timer.div = 0xABCC;

}

bool getFlagZ() {
    return registers.af >> 7 & 1;
}

bool getFlagN() {
    return registers.af >> 6 & 1;
}

bool getFlagH() {
    return registers.af >> 5 & 1;
}

bool getFlagC() {
    return registers.af >> 4 & 1;
}

void setFlagZ(bool value) {
    if (value) {
        registers.af |= 1 << 7;
    } else {
        registers.af &= ~(1 << 7);
    }
}

void setFlagN(bool value) {
    if (value) {
        registers.af |= 1 << 6;
    } else {
        registers.af &= ~(1 << 6);
    }
}

void setFlagH(bool value) {
    if (value) {
        registers.af |= 1 << 5;
    } else {
        registers.af &= ~(1 << 5);
    }
}

void setFlagC(bool value) {
    if (value) {
        registers.af |= 1 << 4;
    } else {
        registers.af &= ~(1 << 4);
    }
}


void cycle_clock(int cycles) {
    timer.div += cycles;
    // TODO: Actually implement the timer like it should be implemented; this is half-assed.
    if (timer.tac >> 2 & 1) {
        switch (timer.tac & 0b11) {
            case 0b00:
                if (cycles % 1024 == 0) {
                    timer.tima++;
                    if (timer.tima == 0) {
                        timer.tima = timer.tma;
                    }
                }
                break;
            case 0b01:
                if (cycles % 16 == 0) {
                    timer.tima++;
                    if (timer.tima == 0) {
                        timer.tima = timer.tma;
                    }
                }
                break;
            case 0b10:
                if (cycles % 64 == 0) {
                    timer.tima++;
                    if (timer.tima == 0) {
                        timer.tima = timer.tma;
                    }
                }
                break;
            case 0b11:
                if (cycles % 256 == 0) {
                    timer.tima++;
                    if (timer.tima == 0) {
                        timer.tima = timer.tma;
                    }
                }
                break;
        }
    }

    // Bad idea actually, sleep is fairly incosistent.
    // nanosleep((const struct timespec[]){{0, 238L}}, NULL);


}

void run_cpu() {

}

static inline void nop() {
    registers.pc++;
    cycle_clock(1);
}

// When called, it will look something like this inc8(&(&registers->l)); dumb language
static inline void inc8(uint8_t *reg) {
    if ((*reg & 0x0F) == 0x0F) {
        setFlagH(true);
    } else {
        setFlagH(false);
    }
    (*reg)++;
    if (*reg == 0) {
        setFlagZ(true);
    } else {
        setFlagZ(false);
    }
    setFlagN(false);

    registers.pc++;
    cycle_clock(1);
}

static inline void inc8_hl(uint8_t *hl) {
    if ((*hl & 0x0F) == 0x0F) {
        setFlagH(true);
    } else {
        setFlagH(false);
    }
    (*hl)++;
    if (*hl == 0) {
        setFlagZ(true);
    } else {
        setFlagZ(false);
    }
    setFlagN(false);

    registers.pc++;
    cycle_clock(3);
}

static inline void dec8(uint8_t *reg) {
    if ((*reg & 0x0F) == 0x00) {
        setFlagH(true);
    } else {
        setFlagH(false);
    }
    (*reg)--;
    if (*reg == 0) {
        setFlagZ(true);
    } else {
        setFlagZ(false);
    }
    setFlagN(false);

    registers.pc++;
    cycle_clock(1);
}

static inline void dec8_hl(uint8_t *hl) {
    if ((*hl & 0x0F) == 0x00) {
        setFlagH(true);
    } else {
        setFlagH(false);
    }
    (*hl)--;
    if (*hl == 0) {
        setFlagZ(true);
    } else {
        setFlagZ(false);
    }
    setFlagN(false);

    registers.pc++;
    cycle_clock(3);
}

static inline void add8_r(uint8_t reg_value) {
    unsigned int result = registers.a + reg_value;

    if (result > 0xFF) {
        setFlagC(true);
    } else {
        setFlagC(false);
    }

    if ( ((registers.a & 0x0F) + (reg_value & 0x0F)) > 0x0F) {
        setFlagH(true);
    } else {
        setFlagH(false);
    }

    setFlagN(false);

    if(result == 0) {
        setFlagZ(true);
    } else {
        setFlagZ(false);
    }

    registers.a = result & 0xFF;

    // Maybe I should have tried Rust?

    registers.pc++;
    cycle_clock(1);
}

static inline void add8_hl_n(uint8_t value) {
    unsigned int result = registers.a + value;

    if (result > 0xFF) {
        setFlagC(true);
    } else {
        setFlagC(false);
    }

    if ( ((registers.a & 0x0F) + (value & 0x0F)) > 0x0F) {
        setFlagH(true);
    } else {
        setFlagH(false);
    }

    setFlagN(false);

    if(result == 0) {
        setFlagZ(true);
    } else {
        setFlagZ(false);
    }

    registers.a = result & 0xFF;

    registers.pc++;
    cycle_clock(2);
}




