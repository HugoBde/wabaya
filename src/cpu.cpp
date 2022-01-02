#include "cpu.h"

#include <bitset>
#include <iostream>

#include "memory.h"
#include "utils.h"

inline bool is_bit_set(word x, int offset) {
    return (x & 1 << offset);
}

CPU::CPU(Memory& _mem)
    : mem(_mem) {
    PC    = PAK_ROM_WAIT_STATE_0_START;
    state = ARM_CODE;
}

CPU::~CPU() {
}

void CPU::run() {
    word instruction = (mem[PC] << 24) | (mem[PC + 1] << 16) | (mem[PC + 2] << 8) | mem[PC + 3];
    switch (state) {
        case ARM_CODE:
            execute_ARM(instruction);
            break;
        case THUMB_CODE:
            execute_THUMB(instruction);
            break;
    }
}

bool CPU::check_condition(INSTRUCTION_CONDITION cond) {
    switch (cond) {
        case EQ:
            return check_flag(CPSR_BIT_FLAGS::ZERO_FLAG);
        case NE:
            return !check_flag(CPSR_BIT_FLAGS::ZERO_FLAG);
        case CS:
            return check_flag(CPSR_BIT_FLAGS::CARRY_FLAG);
        case CC:
            return !check_flag(CPSR_BIT_FLAGS::CARRY_FLAG);
        case MI:
            return check_flag(CPSR_BIT_FLAGS::SIGN_FLAG);
        case PL:
            return !check_flag(CPSR_BIT_FLAGS::SIGN_FLAG);
        case VS:
            return check_flag(CPSR_BIT_FLAGS::OVERFLOW_FLAG);
        case VC:
            return !check_flag(CPSR_BIT_FLAGS::OVERFLOW_FLAG);
        case HI:
            return check_flag(CPSR_BIT_FLAGS::CARRY_FLAG) && !check_flag(CPSR_BIT_FLAGS::ZERO_FLAG);
        case LS:
            return !check_flag(CPSR_BIT_FLAGS::CARRY_FLAG) || check_flag(CPSR_BIT_FLAGS::ZERO_FLAG);
        case GE:
            return check_flag(CPSR_BIT_FLAGS::SIGN_FLAG) == check_flag(CPSR_BIT_FLAGS::OVERFLOW_FLAG);
        case LT:
            return check_flag(CPSR_BIT_FLAGS::SIGN_FLAG) != check_flag(CPSR_BIT_FLAGS::OVERFLOW_FLAG);
        case GT:
            return !check_flag(CPSR_BIT_FLAGS::ZERO_FLAG) && (check_flag(CPSR_BIT_FLAGS::SIGN_FLAG) == check_flag(CPSR_BIT_FLAGS::OVERFLOW_FLAG));
        case LE:
            return check_flag(CPSR_BIT_FLAGS::ZERO_FLAG) || (check_flag(CPSR_BIT_FLAGS::SIGN_FLAG) != check_flag(CPSR_BIT_FLAGS::OVERFLOW_FLAG));
        case AL:
            return true;
        default:
            log_error("Incorrect Instruction condition");
    }
    return true;
}

void CPU::execute_ARM(word instruction) {
    std::cout << std::bitset<32>(instruction);
    check_condition(static_cast<INSTRUCTION_CONDITION>(instruction >> 28));
    switch (decode_arm_instruction(instruction)) {
        default:
        case ADC:
            // do something
            break;
        case ADD:
            // do something
            break;
        case AND:
            // do something
            break;
        case B:
            // do something
            break;
        case BIC:
            // do something
            break;
        case BL:
            // do something
            break;
        case BX:
            // do something
            break;
        case CDP:
            // do something
            break;
        case CMN:
            // do something
            break;
        case CMP:
            // do something
            break;
        case EOR:
            // do something
            break;
        case LDC:
            // do something
            break;
        case LDM:
            // do something
            break;
        case LDR:
            // do something
            break;
        case MCR:
            // do something
            break;
        case MLA:
            // do something
            break;
        case MOV:
            // do something
            break;
        case MRC:
            // do something
            break;
        case MRS:
            // do something
            break;
        case MSR:
            // do something
            break;
        case MUL:
            // do something
            break;
        case MVN:
            // do something
            break;
        case ORR:
            // do something
            break;
        case RSB:
            // do something
            break;
        case RSC:
            // do something
            break;
        case SBC:
            // do something
            break;
        case STC:
            // do something
            break;
        case STM:
            // do something
            break;
        case STR:
            // do something
            break;
        case SUB:
            // do something
            break;
        case SWI:
            // do something
            break;
        case SWP:
            // do something
            break;
        case TEQ:
            // do something
            break;
        case TST:
            // do something
            break;
        case NO_MATCH:
            // do something
            break;
    }
}

ARM_INSTRUCTION CPU::decode_arm_instruction(word instruction) {
    if ((instruction & 0x0F000000) == 0x0F000000) return SWI;
    if ((instruction & 0x0F100010) == 0x0E100010) return MRC;
    if ((instruction & 0x0F100010) == 0x0E000010) return MCR;
    if ((instruction & 0x0F000010) == 0x0E000000) return CDP;
    if ((instruction & 0x0E100000) == 0x0C100000) return LDC;
    if ((instruction & 0x0E100000) == 0x0C000000) return STC;
    if ((instruction & 0x0F000000) == 0x0B000000) return BL;
    if ((instruction & 0x0F000000) == 0x0A000000) return B;
    if ((instruction & 0x0E100000) == 0x08100000) return LDM;
    if ((instruction & 0x0E100000) == 0x08000000) return STM;
    // implement undefined instruction trap maybe
    if ((instruction & 0x0C100000) == 0x04100000) return LDR;
    if ((instruction & 0x0C100000) == 0x04000000) return STR;
    // skipped halfword data transfers as documentation is lacking
    if ((instruction & 0x0FFFFFF0) == 0x012FFF10) return BX;
    if ((instruction & 0x0FB00FF0) == 0x01000090) return SWP;
    // if ((instruction & 0x0FA000F0) == 0x00A00090) return MLAL; // maybe add MLAL to enum of ARM Instructions
    // if ((instruction & 0x0FA000F0) == 0x00800090) return MULL; // maybe add MLAL to enum of ARM Instructions
    if ((instruction & 0x0FE000F0) == 0x00200090) return MLA;
    if ((instruction & 0x0FE000F0) == 0x00000090) return MUL;
    if ((instruction & 0x0A000000) == 0x00000000) {  // data processing
        switch (instruction >> 21 & 0xF) {
            case 0x0:
                return AND;
            case 0x1:
                return EOR;
            case 0x2:
                return SUB;
            case 0x3:
                return RSB;
            case 0x4:
                return ADD;
            case 0x5:
                return ADC;
            case 0x6:
                return SBC;
            case 0x7:
                return RSC;
            case 0x8:
                return TST;
            case 0x9:
                return TEQ;
            case 0xA:
                return CMP;
            case 0xB:
                return CMN;
            case 0xC:
                return ORR;
            case 0xD:
                return MOV;
            case 0xE:
                return BIC;
            case 0xF:
                return MVN;
        }
    }
    if ((instruction & 0x0FBF0FFF) == 0x010F0000) return MRS;
    if ((instruction & 0x0FBFFFF0) == 0x0129F000) return MSR;
    if ((instruction & 0x0DBFF000) == 0x0128F000) return MSR;
    log_error("Could not match ARM instruction");
    return NO_MATCH;
}

void CPU::execute_THUMB(halfword instruction) {}

bool CPU::check_flag(CPSR_BIT_FLAGS flag) {
    return CPSR & flag;
}