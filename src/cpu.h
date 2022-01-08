#ifndef CPU_H
#define CPU_H

// https://problemkaputt.de/gbatek.htm#armcpuoverview
#include "memory.h"
#include "utils.h"

typedef enum {
    ARM_CODE,
    THUMB_CODE
} CPU_STATE;

typedef enum {
    USR,
    FIQ,
    IRQ,
    SVC,
    ABT,
    UND
} CPU_OPERATING_MODE;

typedef enum {
    EQ,  // EQUAL | Z set
    NE,  // NOT EQUAL | Z clear
    CS,  // UNSIGNED HIGHER OR SAME | C set
    CC,  // UNSIGNED LOWER | C clear
    MI,  // NEGATIVE | N set
    PL,  // POSITIVE OR ZERO | N clear
    VS,  // OVERFLOW | V set
    VC,  // NO OVERFLOW | V clear
    HI,  // UNSIGNED HIGHER | C set && Z clear
    LS,  // UNSIGNED LOWER OR SAME | C clear || Z set
    GE,  // GREATER OR EQUAL | N == V
    LT,  // LESS THAN | N != V
    GT,  // GREATHER THAN | Z clear && (N == V)
    LE,  // LESSER OR EQUAL | Z set || (N != V)
    AL   // ALWAYS
} INSTRUCTION_CONDITION;

typedef enum {
    M0              = 1 << 0,
    M1              = 1 << 1,
    M2              = 1 << 2,
    M3              = 1 << 3,
    M4              = 1 << 4,
    STATE_BIT       = 1 << 5,  // ARM(0) or THUMB(1)
    FIQ_DISABLE     = 1 << 6,
    IRQ_DISABLE     = 1 << 7,
    ABORT_DISABLE   = 1 << 8,
    ENDIAN          = 1 << 9,
    JAZELLE_MODE    = 1 << 24,
    STICKY_OVERFLOW = 1 << 27,
    // FLAGS
    OVERFLOW_FLAG = 1 << 28,  // V flag
    CARRY_FLAG    = 1 << 29,  // C flag
    ZERO_FLAG     = 1 << 30,  // Z flag
    SIGN_FLAG     = 1 << 31   // N flag
} CPSR_BIT_FLAG;

typedef enum class ARM_INSTRUCTION {
    ADC,
    ADD,
    AND,
    B,
    BIC,
    BL,
    BX,
    CMN,
    CMP,
    EOR,
    LDM,
    LDR,
    MLA,
    MOV,
    MRS,
    MSR,
    MUL,
    MVN,
    ORR,
    RSB,
    RSC,
    SBC,
    STM,
    STR,
    SUB,
    SWI,
    SWP,
    TEQ,
    TST,
    NO_MATCH,
#ifdef COPROC_SUPPORT
    CDP,
    LDC,
    MCR,
    MRC,
    STC,
#endif
} ARM_INSTRUCTION;

typedef enum class THUMB_INSTRUCTION {
    ADC,
    ADD,
    AND,
    ASR,
    B,
    BXX,
    BIC,
    BL,
    BX,
    CMN,
    CMP,
    EOR,
    LDMIA,
    LDR,
    LDRB,
    LDRH,
    LSL,
    LDSB,
    LDSH,
    LSR,
    MOV,
    MUL,
    MVN,
    NEG,
    ORR,
    POP,
    PUSH,
    ROR,
    SBC,
    STMIA,
    STR,
    STRB,
    STRH,
    SWI,
    SUB,
    TST,
    NO_MATCH
} THUMB_INSTRUCTION;

class CPU {
    private:
    CPU_STATE state;
    CPU_OPERATING_MODE mode;

    /*
    word registers[16];
    word FIQ_banked_registers[7];  // FIQ_banked_registers[n] is register R(N+8) for FIQ
    word SVC_banked_registers[2];  // All following banked registers are R(N+13) for their corresponding op mode
    word ABT_banked_registers[2];
    word IRQ_banked_registers[2];
    word UND_banked_registers[2];
    word CPSR;
    word SPSR_FIQ;
    word SPSR_SVC;
    word SPSR_ABT;
    word SPSR_IRQ;
    word SPSR_UND;
    */

    word r0;
    word r1;
    word r2;
    word r3;
    word r4;
    word r5;
    word r6;
    word r7;
    word r8;
    word r9;
    word r10;
    word r11;
    word r12;
    word r13;
    word r14;
    word r15;
    word fiq_r8;
    word fiq_r9;
    word fiq_r10;
    word fiq_r11;
    word fiq_r12;
    word fiq_r13;
    word fiq_r14;
    word svc_r13;
    word svc_r14;
    word abt_r13;
    word abt_r14;
    word irq_r13;
    word irq_r14;
    word und_r13;
    word und_r14;

    word CPSR;
    word fiq_SPSR;
    word svc_SPSR;
    word abt_SPSR;
    word irq_SPSR;
    word und_SPSR;

    word* r[6][16] = {
        {&r0, &r1, &r2, &r3, &r4, &r5, &r6, &r7, &r8, &r9, &r10, &r11, &r12, &r13, &r14, &r15},                              // usr
        {&r0, &r1, &r2, &r3, &r4, &r5, &r6, &r7, &fiq_r8, &fiq_r9, &fiq_r10, &fiq_r11, &fiq_r12, &fiq_r13, &fiq_r14, &r15},  // fiq
        {&r0, &r1, &r2, &r3, &r4, &r5, &r6, &r7, &r8, &r9, &r10, &r11, &r12, &svc_r13, &svc_r14, &r15},                      // svc
        {&r0, &r1, &r2, &r3, &r4, &r5, &r6, &r7, &r8, &r9, &r10, &r11, &r12, &abt_r13, &abt_r14, &r15},                      // abt
        {&r0, &r1, &r2, &r3, &r4, &r5, &r6, &r7, &r8, &r9, &r10, &r11, &r12, &irq_r13, &irq_r14, &r15},                      // irq
        {&r0, &r1, &r2, &r3, &r4, &r5, &r6, &r7, &r8, &r9, &r10, &r11, &r12, &und_r13, &und_r14, &r15}                       // und
    };

    word& PC = r15;

    Memory& mem;

    public:
    CPU(Memory& mem);
    ~CPU();
    void run();
    word* get_reg(int r);
    void execute_ARM(word instruction);
    void execute_THUMB(halfword instruction);
    bool check_condition(INSTRUCTION_CONDITION cond);
    bool check_flag(CPSR_BIT_FLAG flag);
    void arm_branch_exchange(word instruction);
    void arm_branch(word instruction);
    void arm_branch_link(word instruction);
    void arm_and(word instruction);
    void arm_edor(word instruction);
    void arm_sub(word instruction);
    void arm_reverse_sub(word instruction);
    void arm_add(word instruction);
    void arm_add_carry(word instruction);
    void arm_sub_carry(word instruction);
    void arm_reverse_sub_carry(word instruction);
    void arm_test_bits(word instruction);
    void arm_test_bitwise_equality(word instruction);
    void arm_compare(word instruction);
    void arm_compare_negative(word instruction);
    void arm_or(word instruction);
    void arm_mov(word instruction);
    void arm_bit_clear(word instruction);
    void arm_mov_negative(word instruction);
    void arm_mov_psr_reg(word instruction);
    void arm_mov_reg_psr(word instruction);
    void arm_multiply(word instruction);
    void arm_multiply_accumulate(word instruction);
    void arm_multiply_long(word instruction);
    void arm_multiply_long_accumulate(word instruction);
    void arm_store_reg_mem(word instruction);
    void arm_load_mem_reg(word instruction);
    void arm_store_reg_mem_halfword(word instruction);
    void arm_load_mem_reg_halfword(word instruction);
    void arm_store_reg_mem_byte(word instruction);
    void arm_load_mem_reg_byte(word instruction);
    void arm_store_multiple(word instruction);
    void arm_load_multiple(word instruction);
    void arm_single_data_swap(word instruction);
    void arm_software_interrupt(word instruction);
    static ARM_INSTRUCTION decode_arm_instruction(word instruction);
    static THUMB_INSTRUCTION decode_thumb_instruction(word instruction);
};

#endif