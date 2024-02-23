//
// Created by nicusor43 on 2/16/24.
//

#include "cpu.h"

#define CHECK_BIT(x, n) (registers.x >> n & 1)

struct gb_registers registers;
struct gb_timer timer;

// Funny programming language.
static inline void nop(void);

static inline void inc8(uint8_t *reg);

static inline void inc8_hl(uint8_t *hl);

static inline void dec8(uint8_t *reg);

static inline void dec8_hl(uint8_t *hl);

static inline void add8_r(uint8_t reg_value);

static inline void add8_hl(uint8_t value);

static inline void add8_n(uint8_t value);


static inline void adc_r(uint8_t reg_value);

static inline void adc_hl(uint8_t value);

static inline void adc_n(uint8_t value);

static inline void sub8_r(uint8_t reg_value);

static inline void sub8_hl(uint8_t value);

static inline void sub8_n(uint8_t value);


static inline void sbc_r(uint8_t reg_value);

static inline void sbc_hl(uint8_t value);

static inline void sbc_n(uint8_t value);


static inline void and_r(uint8_t reg_value);

static inline void and_hl(uint8_t value);

static inline void and_n(uint8_t value);


static inline void or_r(uint8_t reg_value);

static inline void or_hl(uint8_t value);

static inline void or_n(uint8_t value);


static inline void xor_r(uint8_t reg_value);

static inline void xor_hl(uint8_t value);

static inline void xor_n(uint8_t value);

static inline void ccf(void);

static inline void scf(void);

static inline void cpl(void);

static inline void cp_r(uint8_t reg_value);

static inline void cp_hl(uint8_t value);

static inline void cp_n(uint8_t value);


static inline void daa(void);

// 16-bit arithmetic
static inline void inc16(uint16_t *reg);

static inline void dec16(uint16_t *reg);

static inline void add16_hl(uint16_t reg_value);

static inline void add16_sp(int8_t value);

static inline void rlca(void);

static inline void rrca(void);

static inline void rla(void);

static inline void rra(void);

static inline void rl(uint8_t *reg);

static inline void rr(uint8_t *reg);

static inline void rlc(uint8_t *reg);

static inline void rrc(uint8_t *reg);

static inline void rl_hl(uint8_t *hl);

static inline void rr_hl(uint8_t *hl);

static inline void rlc_hl(uint8_t *hl);

static inline void rrc_hl(uint8_t *hl);

static inline void sla(uint8_t *reg);

static inline void sra(uint8_t *reg);

static inline void sla_hl(uint8_t *hl);

static inline void sra_hl(uint8_t *hl);

static inline void srl(uint8_t *reg);

static inline void srl_hl(uint8_t *hl);

static inline void swap(uint8_t *reg);

static inline void swap_hl(uint8_t *hl);

static inline void set(uint8_t *reg, uint8_t bit);

static inline void set_hl(uint8_t *hl, uint8_t bit);

static inline void res(uint8_t *reg, uint8_t bit);

static inline void res_hl(uint8_t *hl, uint8_t bit);

static inline void bit(uint8_t reg, uint8_t bit);

static inline void bit_hl(uint8_t hl, uint8_t bit);

static inline void ld_r_r(uint8_t *dest, uint8_t src);

static inline void ld_r_n(uint8_t *dest, uint8_t value);

static inline void ld_r_hl(uint8_t *dest, uint8_t *hl);

static inline void ld_hl_r(uint8_t *hl, uint8_t *src);

static inline void ld16_r_n(uint16_t *dest, uint16_t value);

static inline void ld8_hl_n(uint8_t *hl, uint8_t value);

static inline void ld16_r_A(uint16_t *dest);

static inline void ld16_n_A(uint8_t *dest);

static inline void ldh16_n_A(uint16_t address);

static inline void ldh_C_A(uint8_t *dest);

static inline void ld16_A_r(uint8_t src);

static inline void ldh16_A_n(uint8_t src);

static inline void ldh_A_C(uint8_t src);

static inline void ld16_A_n(uint8_t src);

static inline void ld_hli_A(uint8_t *hl);

static inline void ld_hld_A(uint8_t *hl);

static inline void ld_A_hli(uint8_t *hl);

static inline void ld_A_hld(uint8_t *hl);

static inline void ld16_sp_n(uint16_t value);

static inline void ld16_n_sp(uint8_t *dest);

static inline void ld_hl_sp_e8(int8_t signed_value);

static inline void ld_sp_hl(void);


void cpu_start(void) {
    registers.af = 0x01B0;
    registers.bc = 0x0013;
    registers.de = 0x00D8;
    registers.hl = 0x014D;
    registers.sp = 0xFFFE;
    registers.pc = 0x1000;

    timer.div = 0xABCC;

}

bool getFlagZ(void) {
    return registers.af >> 7 & 1;
}

bool getFlagN(void) {
    return registers.af >> 6 & 1;
}

bool getFlagH(void) {
    return registers.af >> 5 & 1;
}

bool getFlagC(void) {
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

    if (((registers.a & 0x0F) + (reg_value & 0x0F)) > 0x0F) {
        setFlagH(true);
    } else {
        setFlagH(false);
    }

    setFlagN(false);

    if (result == 0) {
        setFlagZ(true);
    } else {
        setFlagZ(false);
    }

    registers.a = result & 0xFF;

    // Maybe I should have tried Rust?

    registers.pc++;
    cycle_clock(1);
}

static inline void add8_hl(uint8_t value) {
    unsigned int result = registers.a + value;

    if (result > 0xFF) {
        setFlagC(true);
    } else {
        setFlagC(false);
    }

    if (((registers.a & 0x0F) + (value & 0x0F)) > 0x0F) {
        setFlagH(true);
    } else {
        setFlagH(false);
    }

    setFlagN(false);

    if (result == 0) {
        setFlagZ(true);
    } else {
        setFlagZ(false);
    }

    registers.a = result & 0xFF;

    registers.pc++;
    cycle_clock(2);
}

static inline void add8_n(uint8_t value) {
    unsigned int result = registers.a + value;

    if (result > 0xFF) {
        setFlagC(true);
    } else {
        setFlagC(false);
    }

    if (((registers.a & 0x0F) + (value & 0x0F)) > 0x0F) {
        setFlagH(true);
    } else {
        setFlagH(false);
    }

    setFlagN(false);

    if (result == 0) {
        setFlagZ(true);
    } else {
        setFlagZ(false);
    }

    registers.a = result & 0xFF;

    registers.pc += 2;
    cycle_clock(2);
}

static inline void adc_r(uint8_t reg_value) {
    unsigned int result = registers.a + reg_value + getFlagC();

    if (((registers.a & 0x0F) + (reg_value & 0x0F) + getFlagC()) > 0x0F) {
        setFlagH(true);
    } else {
        setFlagH(false);
    }

    if (result > 0xFF) {
        setFlagC(true);
    } else {
        setFlagC(false);
    }

    setFlagN(false);

    if (result == 0) {
        setFlagZ(true);
    } else {
        setFlagZ(false);
    }

    registers.a = result & 0xFF;

    registers.pc++;
    cycle_clock(1);
}

static inline void adc_hl(uint8_t value) {
    unsigned int result = registers.a + value + getFlagC();

    if (result > 0xFF) {
        setFlagC(true);
    } else {
        setFlagC(false);
    }

    if (((registers.a & 0x0F) + (value & 0x0F) + getFlagC()) > 0x0F) {
        setFlagH(true);
    } else {
        setFlagH(false);
    }

    setFlagN(false);

    if (result == 0) {
        setFlagZ(true);
    } else {
        setFlagZ(false);
    }

    registers.a = result & 0xFF;

    registers.pc++;
    cycle_clock(2);
}

static inline void adc_n(uint8_t value) {
    unsigned int result = registers.a + value + getFlagC();

    if (result > 0xFF) {
        setFlagC(true);
    } else {
        setFlagC(false);
    }

    if (((registers.a & 0x0F) + (value & 0x0F) + getFlagC()) > 0x0F) {
        setFlagH(true);
    } else {
        setFlagH(false);
    }

    setFlagN(false);

    if (result == 0) {
        setFlagZ(true);
    } else {
        setFlagZ(false);
    }

    registers.a = result & 0xFF;

    registers.pc += 2;
    cycle_clock(2);
}

static inline void sub8_r(uint8_t reg_value) {
    unsigned int result = registers.a - reg_value;

    if (reg_value > registers.a) {
        setFlagC(true);
    } else {
        setFlagC(false);
    }

    if ((registers.a & 0x0F) < (reg_value & 0x0F)) {
        setFlagH(true);
    } else {
        setFlagH(false);
    }

    setFlagN(true);

    if (result == 0) {
        setFlagZ(true);
    } else {
        setFlagZ(false);
    }

    registers.a = result;

    registers.pc++;
    cycle_clock(1);
}

static inline void sub8_hl(uint8_t value) {
    unsigned int result = registers.a - value;

    if (value > registers.a) {
        setFlagC(true);
    } else {
        setFlagC(false);
    }

    if ((registers.a & 0x0F) < (value & 0x0F)) {
        setFlagH(true);
    } else {
        setFlagH(false);
    }

    setFlagN(true);

    if (result == 0) {
        setFlagZ(true);
    } else {
        setFlagZ(false);
    }

    registers.a = result;

    registers.pc++;
    cycle_clock(2);
}

static inline void sub8_n(uint8_t value) {
    unsigned int result = registers.a - value;

    if (value > registers.a) {
        setFlagC(true);
    } else {
        setFlagC(false);
    }

    if ((registers.a & 0x0F) < (value & 0x0F)) {
        setFlagH(true);
    } else {
        setFlagH(false);
    }

    setFlagN(true);

    if (result == 0) {
        setFlagZ(true);
    } else {
        setFlagZ(false);
    }

    registers.a = result;

    registers.pc += 2;
    cycle_clock(2);
}

static inline void sbc_r(uint8_t reg_value) {
    unsigned int result = registers.a - reg_value - getFlagC();


    if ((registers.a & 0x0F) < ((reg_value & 0x0F) + getFlagC())) {
        setFlagH(true);
    } else {
        setFlagH(false);
    }

    if ((reg_value + getFlagC()) > registers.a) {
        setFlagC(true);
    } else {
        setFlagC(false);
    }

    setFlagN(true);

    if (result == 0) {
        setFlagZ(true);
    } else {
        setFlagZ(false);
    }

    registers.a = result;

    registers.pc++;
    cycle_clock(1);
}

static inline void sbc_hl(uint8_t value) {
    unsigned int result = registers.a - value - getFlagC();

    if ((registers.a & 0x0F) < ((value & 0x0F) + getFlagC())) {
        setFlagH(true);
    } else {
        setFlagH(false);
    }

    if ((value + getFlagC()) > registers.a) {
        setFlagC(true);
    } else {
        setFlagC(false);
    }

    setFlagN(true);

    if (result == 0) {
        setFlagZ(true);
    } else {
        setFlagZ(false);
    }

    registers.a = result;

    registers.pc++;
    cycle_clock(2);
}

static inline void sbc_n(uint8_t value) {
    unsigned int result = registers.a - value - getFlagC();

    if ((registers.a & 0x0F) < ((value & 0x0F) + getFlagC())) {
        setFlagH(true);
    } else {
        setFlagH(false);
    }

    if ((value + getFlagC()) > registers.a) {
        setFlagC(true);
    } else {
        setFlagC(false);
    }

    setFlagN(true);

    if (result == 0) {
        setFlagZ(true);
    } else {
        setFlagZ(false);
    }

    registers.a = result;

    registers.pc += 2;
    cycle_clock(2);
}

static inline void and_r(uint8_t reg_value) {
    registers.a &= reg_value;

    if (registers.a == 0) {
        setFlagZ(true);
    } else {
        setFlagZ(false);
    }

    setFlagN(false);
    setFlagH(true);
    setFlagC(false);

    registers.pc++;
    cycle_clock(1);
}

static inline void and_hl(uint8_t value) {
    registers.a &= value;

    if (registers.a == 0) {
        setFlagZ(true);
    } else {
        setFlagZ(false);
    }

    setFlagN(false);
    setFlagH(true);
    setFlagC(false);

    registers.pc++;
    cycle_clock(2);
}

static inline void and_n(uint8_t value) {
    registers.a &= value;

    if (registers.a == 0) {
        setFlagZ(true);
    } else {
        setFlagZ(false);
    }

    setFlagN(false);
    setFlagH(true);
    setFlagC(false);

    registers.pc += 2;
    cycle_clock(2);
}


static inline void or_r(uint8_t reg_value) {
    registers.a |= reg_value;

    if (registers.a == 0) {
        setFlagZ(true);
    } else {
        setFlagZ(false);
    }

    setFlagN(false);
    setFlagH(false);
    setFlagC(false);

    registers.pc++;
    cycle_clock(1);
}

static inline void or_hl(uint8_t value) {
    registers.a |= value;

    if (registers.a == 0) {
        setFlagZ(true);
    } else {
        setFlagZ(false);
    }

    setFlagN(false);
    setFlagH(false);
    setFlagC(false);

    registers.pc++;
    cycle_clock(2);
}

static inline void or_n(uint8_t value) {
    registers.a |= value;

    if (registers.a == 0) {
        setFlagZ(true);
    } else {
        setFlagZ(false);
    }

    setFlagN(false);
    setFlagH(false);
    setFlagC(false);

    registers.pc += 2;
    cycle_clock(2);
}

static inline void xor_r(uint8_t reg_value) {
    registers.a ^= reg_value;

    if (registers.a == 0) {
        setFlagZ(true);
    } else {
        setFlagZ(false);
    }

    setFlagN(false);
    setFlagH(false);
    setFlagC(false);

    registers.pc++;
    cycle_clock(1);
}

static inline void xor_hl(uint8_t value) {
    registers.a ^= value;

    if (registers.a == 0) {
        setFlagZ(true);
    } else {
        setFlagZ(false);
    }

    setFlagN(false);
    setFlagH(false);
    setFlagC(false);

    registers.pc++;
    cycle_clock(2);
}


static inline void xor_n(uint8_t value) {
    registers.a ^= value;

    if (registers.a == 0) {
        setFlagZ(true);
    } else {
        setFlagZ(false);
    }

    setFlagN(false);
    setFlagH(false);
    setFlagC(false);

    registers.pc += 2;
    cycle_clock(2);
}

static inline void ccf() {
    setFlagN(false);
    setFlagH(false);
    setFlagC(!getFlagC());

    registers.pc++;
    cycle_clock(1);
}

static inline void scf() {
    setFlagN(false);
    setFlagH(false);
    setFlagC(true);

    registers.pc++;
    cycle_clock(1);
}

static inline void cpl() {
    registers.a = ~registers.a;

    setFlagN(true);
    setFlagH(true);

    registers.pc++;
    cycle_clock(1);
}

static inline void cp_r(uint8_t reg_value) {
    unsigned int result = registers.a - reg_value;

    if (reg_value > registers.a) {
        setFlagC(true);
    } else {
        setFlagC(false);
    }

    if ((registers.a & 0x0F) < (reg_value & 0x0F)) {
        setFlagH(true);
    } else {
        setFlagH(false);
    }

    setFlagN(true);

    if (result == 0) {
        setFlagZ(true);
    } else {
        setFlagZ(false);
    }

    registers.pc++;
    cycle_clock(1);
}

static inline void cp_hl(uint8_t value) {
    unsigned int result = registers.a - value;

    if (value > registers.a) {
        setFlagC(true);
    } else {
        setFlagC(false);
    }

    if ((registers.a & 0x0F) < (value & 0x0F)) {
        setFlagH(true);
    } else {
        setFlagH(false);
    }

    setFlagN(true);

    if (result == 0) {
        setFlagZ(true);
    } else {
        setFlagZ(false);
    }

    registers.pc++;
    cycle_clock(2);
}

static inline void cp_n(uint8_t value) {
    unsigned int result = registers.a - value;

    if (value > registers.a) {
        setFlagC(true);
    } else {
        setFlagC(false);
    }

    if ((registers.a & 0x0F) < (value & 0x0F)) {
        setFlagH(true);
    } else {
        setFlagH(false);
    }

    setFlagN(true);

    if (result == 0) {
        setFlagZ(true);
    } else {
        setFlagZ(false);
    }

    registers.pc += 2;
    cycle_clock(2);
}

static inline void daa() {
    if (!getFlagN()) {
        if (getFlagH() || (registers.a & 0x0F) > 9) {
            registers.a += 0x06;
        }
        if (getFlagC() || registers.a > 0x99) {
            registers.a += 0x60;
            setFlagC(true);
        }
    } else {
        if (getFlagH()) {
            registers.a -= 6;
        }
        if (getFlagC()) {
            registers.a -= 0x60;
        }
    }

    if (registers.a == 0) {
        setFlagZ(true);
    } else {
        setFlagZ(false);
    }

    setFlagH(false);

    registers.pc++;
    cycle_clock(1);
}

// 16-bit arithmetic
static inline void inc16(uint16_t *reg) {
    (*reg)++;

    registers.pc++;
    cycle_clock(2);
}

static inline void dec16(uint16_t *reg) {
    (*reg)--;

    registers.pc++;
    cycle_clock(2);
}

static inline void add16_hl(uint16_t reg_value) {
    unsigned int result = registers.hl + reg_value;

    if (result > 0xFFFF) {
        setFlagC(true);
    } else {
        setFlagC(false);
    }

    if (((registers.hl & 0x0FFF) + (reg_value & 0x0FFF)) > 0x0FFF) {
        setFlagH(true);
    } else {
        setFlagH(false);
    }

    setFlagN(false);

    registers.hl = result & 0xFFFF;

    registers.pc++;
    cycle_clock(2);
}


// Does this even exist?
static inline void add16_sp(int8_t signed_value) {
    unsigned int result = registers.sp + signed_value;

    if (result > 0xFF) {
        setFlagC(true);
    } else {
        setFlagC(false);
    }

    if (((registers.sp & 0x0F) + (signed_value & 0x0F)) > 0x0F) {
        setFlagH(true);
    } else {
        setFlagH(false);
    }

    setFlagN(false);

    registers.sp = result & 0xFFFF;

    registers.pc += 2;
    cycle_clock(4);
}

// Rotate instructions
static inline void rlca(void) {
    setFlagC(registers.a >> 7 & 1);

    registers.a = (registers.a << 1) | (registers.a >> 7);

    setFlagN(false);
    setFlagH(false);
    setFlagZ(false);

    registers.pc++;
    cycle_clock(1);
}

static inline void rrca(void) {
    setFlagC(registers.a & 1);

    registers.a = (registers.a >> 1) | (registers.a << 7);

    setFlagN(false);
    setFlagH(false);
    setFlagZ(false);

    registers.pc++;
    cycle_clock(1);
}

static inline void rla(void) {
    uint8_t new_carry = (registers.a >> 7) & 1;

    registers.a = (registers.a << 1) | getFlagC();

    setFlagC(new_carry == 1);

    setFlagH(false);
    setFlagZ(false);
    setFlagN(false);
}

static inline void rra(void) {
    uint8_t new_carry = registers.a & 1;

    registers.a = (registers.a >> 1) | (getFlagC() << 7);

    setFlagC(new_carry == 1);

    setFlagH(false);
    setFlagZ(false);
    setFlagN(false);

    registers.pc++;
    cycle_clock(1);
}

static inline void rl(uint8_t *reg) {
    uint8_t new_carry = (*reg >> 7) & 1;

    *reg = (*reg << 1) | getFlagC();

    setFlagC(new_carry == 1);

    setFlagH(false);
    setFlagN(false);
    setFlagZ(*reg == 0);


    registers.pc += 2;
    cycle_clock(2);
}

static inline void rr(uint8_t *reg) {
    uint8_t new_carry = *reg & 1;

    *reg = (*reg >> 1) | (getFlagC() << 7);

    setFlagC(new_carry == 1);

    setFlagH(false);
    setFlagN(false);
    setFlagZ(*reg == 0);


    registers.pc += 2;
    cycle_clock(2);
}

static inline void rlc(uint8_t *reg) {
    setFlagC(*reg >> 7 & 1);

    *reg = (*reg << 1) | (*reg >> 7);

    setFlagN(false);
    setFlagH(false);
    setFlagZ(*reg == 0);

    registers.pc += 2;
    cycle_clock(2);
}

static inline void rrc(uint8_t *reg) {
    setFlagC(*reg & 1);

    *reg = (*reg >> 1) | (*reg << 7);

    setFlagN(false);
    setFlagH(false);
    setFlagZ(*reg == 0);

    registers.pc += 2;
    cycle_clock(2);
}

static inline void rl_hl(uint8_t *hl) {
    uint8_t new_carry = (*hl >> 7) & 1;

    *hl = (*hl << 1) | getFlagC();

    setFlagC(new_carry == 1);

    setFlagH(false);
    setFlagN(false);
    setFlagZ(*hl == 0);

    registers.pc += 2;
    cycle_clock(4);
}

static inline void rr_hl(uint8_t *hl) {
    uint8_t new_carry = *hl & 1;

    *hl = (*hl >> 1) | (getFlagC() << 7);

    setFlagC(new_carry == 1);

    setFlagH(false);
    setFlagN(false);
    setFlagZ(*hl == 0);

    registers.pc += 2;
    cycle_clock(4);
}

static inline void rlc_hl(uint8_t *hl) {
    setFlagC(*hl >> 7 & 1);

    *hl = (*hl << 1) | (*hl >> 7);

    setFlagN(false);
    setFlagH(false);
    setFlagZ(*hl == 0);

    registers.pc += 2;
    cycle_clock(4);
}

static inline void rrc_hl(uint8_t *hl) {
    setFlagC(*hl & 1);

    *hl = (*hl >> 1) | (*hl << 7);

    setFlagN(false);
    setFlagH(false);
    setFlagZ(*hl == 0);

    registers.pc += 2;
    cycle_clock(4);
}

static inline void sla(uint8_t *reg) {
    setFlagC(*reg >> 7 & 1);

    *reg <<= 1;

    setFlagN(false);
    setFlagH(false);
    setFlagZ(*reg == 0);

    registers.pc += 2;
    cycle_clock(2);
}

static inline void sra(uint8_t *reg) {
    setFlagC(*reg & 1);

    int msb = *reg >> 7 & 1;
    *reg = (*reg >> 1) | (msb << 7);


    setFlagN(false);
    setFlagH(false);
    setFlagZ(*reg == 0);

    registers.pc += 2;
    cycle_clock(2);
}

static inline void sla_hl(uint8_t *hl) {
    setFlagC(*hl >> 7 & 1);

    *hl <<= 1;

    setFlagN(false);
    setFlagH(false);
    setFlagZ(*hl == 0);

    registers.pc += 2;
    cycle_clock(4);
}

static inline void sra_hl(uint8_t *hl) {
    setFlagC(*hl & 1);

    int msb = *hl >> 7 & 1;
    *hl = (*hl >> 1) | (msb << 7);

    setFlagN(false);
    setFlagH(false);
    setFlagZ(*hl == 0);

    registers.pc += 2;
    cycle_clock(4);
}

static inline void srl(uint8_t *reg) {
    setFlagC(*reg & 1);

    *reg >>= 1;

    setFlagN(false);
    setFlagH(false);
    setFlagZ(*reg == 0);

    registers.pc += 2;
    cycle_clock(2);
}

static inline void srl_hl(uint8_t *hl) {
    setFlagC(*hl & 1);

    *hl >>= 1;

    setFlagN(false);
    setFlagH(false);
    setFlagZ(*hl == 0);

    registers.pc += 2;
    cycle_clock(4);
}

static inline void swap(uint8_t *reg) {
    *reg = (*reg >> 4) | (*reg << 4);

    setFlagN(false);
    setFlagH(false);
    setFlagC(false);
    setFlagZ(*reg == 0);

    registers.pc += 2;
    cycle_clock(2);
}

static inline void swap_hl(uint8_t *hl) {
    *hl = (*hl >> 4) | (*hl << 4);

    setFlagN(false);
    setFlagH(false);
    setFlagC(false);
    setFlagZ(*hl == 0);

    registers.pc += 2;
    cycle_clock(4);
}

static inline void set(uint8_t *reg, uint8_t bit) {
    *reg |= 1 << bit;

    registers.pc += 2;
    cycle_clock(2);
}

static inline void set_hl(uint8_t *hl, uint8_t bit) {
    *hl |= 1 << bit;

    registers.pc += 2;
    cycle_clock(4);
}

static inline void res(uint8_t *reg, uint8_t bit) {
    *reg &= ~(1 << bit);

    registers.pc += 2;
    cycle_clock(2);
}

static inline void res_hl(uint8_t *hl, uint8_t bit) {
    *hl &= ~(1 << bit);

    registers.pc += 2;
    cycle_clock(4);
}

static inline void bit(uint8_t reg, uint8_t bit) {
    if (reg >> bit & 1) {
        setFlagZ(false);
    } else {
        setFlagZ(true);
    }

    setFlagN(false);
    setFlagH(true);

    registers.pc += 2;
    cycle_clock(2);
}

static inline void bit_hl(uint8_t hl, uint8_t bit) {
    if (hl >> bit & 1) {
        setFlagZ(false);
    } else {
        setFlagZ(true);
    }

    setFlagN(false);
    setFlagH(true);

    registers.pc += 2;
    cycle_clock(3);
}

static inline void ld_r_r(uint8_t *dest, uint8_t src) {
    *dest = src;

    registers.pc++;
    cycle_clock(1);
}

static inline void ld_r_n(uint8_t *dest, uint8_t value) {
    *dest = value;

    registers.pc += 2;
    cycle_clock(2);
}

static inline void ld16_r_n(uint16_t *dest, uint16_t value) {
    *dest = value;

    registers.pc += 3;
    cycle_clock(3);
}

static inline void ld_r_hl(uint8_t *dest, uint8_t *hl) {
    *dest = *hl;

    registers.pc++;
    cycle_clock(2);
}

static inline void ld_hl_r(uint8_t *hl, uint8_t *src) {
    *hl = *src;

    registers.pc++;
    cycle_clock(2);
}

static inline void ld8_hl_n(uint8_t *hl, uint8_t value) {
    *hl = value;

    registers.pc += 2;
    cycle_clock(3);
}

static inline void ld16_r_A(uint16_t *dest) {
    *dest = registers.a;

    registers.pc++;
    cycle_clock(2);
}

static inline void ld16_n_A(uint8_t *dest) {
    *dest = registers.a;

    registers.pc += 3;
    cycle_clock(4);
}

static inline void ldh16_n_A(uint16_t address) {
    // TODO: When the memory is finished, do this

    registers.pc += 2;
    cycle_clock(3);
}

static inline void ldh_C_A(uint8_t *dest) {
    // TODO

    registers.pc++;
    cycle_clock(2);
}

static inline void ld16_A_n(uint8_t src) {
    registers.a = src;

    registers.pc += 3;
    cycle_clock(4);
}

static inline void ld16_A_r(uint8_t src) {
    registers.a = src;

    registers.pc++;
    cycle_clock(2);
}

static inline void ldh16_A_n(uint8_t src) {
    // TODO

    registers.pc += 2;
    cycle_clock(3);
}

static inline void ldh_A_C(uint8_t src) {
    // TODO

    registers.pc++;
    cycle_clock(2);
}


static inline void ld_hli_A(uint8_t *hl) {
    *hl = registers.a;
    registers.hl++;

    registers.pc++;
    cycle_clock(2);
}

static inline void ld_hld_A(uint8_t *hl) {
    *hl = registers.a;
    registers.hl--;

    registers.pc++;
    cycle_clock(2);
}

static inline void ld_A_hli(uint8_t *hl) {
    registers.a = *hl;
    registers.hl++;

    registers.pc++;
    cycle_clock(2);
}

static inline void ld_A_hld(uint8_t *hl) {
    registers.a = *hl;
    registers.hl--;

    registers.pc++;
    cycle_clock(2);
}

static inline void ld16_sp_n(uint16_t value) {
    registers.sp = value;

    registers.pc += 3;
    cycle_clock(3);
}

static inline void ld16_n_sp(uint8_t *dest) {
    *dest = registers.sp & 0xFF;
    // TODO dest + 1

    registers.pc += 3;
    cycle_clock(5);
}

static inline void ld_hl_sp_e8(int8_t signed_value) {
    registers.hl = signed_value + registers.hl;


    registers.pc += 2;
    cycle_clock(3);
}

static inline void ld_sp_hl(void) {
    registers.sp = registers.hl;

    registers.pc++;
    cycle_clock(2);
}

