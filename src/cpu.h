#ifndef CPU_H
#define CPU_H

// https://problemkaputt.de/gbatek.htm#armcpuoverview
#include "memory.h"

typedef enum {
    ARM_CODE,
    THUMB_CODE
} CPU_STATE;

typedef enum {
    FIQ,
    SVC,
    ABT,
    IRQ,
    UND
} CPU_EXCEPTIONS;

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
} CPSR_BIT_FLAGS;

typedef enum {
    ADC,
    ADD,
    AND,
    B,
    BIC,
    BL,
    BX,
    CDP,
    CMN,
    CMP,
    EOR,
    LDC,
    LDM,
    LDR,
    MCR,
    MLA,
    MOV,
    MRC,
    MRS,
    MSR,
    MUL,
    MVN,
    ORR,
    RSB,
    RSC,
    SBC,
    STC,
    STM,
    STR,
    SUB,
    SWI,
    SWP,
    TEQ,
    TST,
    NO_MATCH
} ARM_INSTRUCTION;

class CPU {
    private:
    CPU_STATE state;
    word registers[13];  // General purpose registers R0 - R12
    word SP;             // Stack pointer
    word LR;             // Link register
    word PC;             // Program counter (find out more about read-ahead pipelining and the fact that reading from this register returns PC + nn)
    word CPSR;           // Current and Stored Program status register
    word SPSR[5];
    Memory& mem;

    public:
    CPU(Memory& mem);
    ~CPU();
    void run();
    void execute_ARM(word instruction);
    void execute_THUMB(halfword instruction);
    bool check_condition(INSTRUCTION_CONDITION cond);
    bool check_flag(CPSR_BIT_FLAGS flag);
    static ARM_INSTRUCTION decode_arm_instruction(word instruction);
    static ARM_INSTRUCTION decode_thumb_instruction(word instruction);
};

#endif