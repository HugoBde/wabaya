#include "cpu.h"
#include <sys/types.h>

#include <bitset>
#include <iostream>

#include "memory.h"
#include "utils.h"

inline bool is_bit_set(word x, int offset) {
    return (x & 1 << offset);
}

inline word rotate_right(word value, int rotate_amount) {
    if (rotate_amount == 0) return value;
    word mask = (1 << rotate_amount) - 1;
    word temp = value & mask;
    value >>= rotate_amount;
    value |= temp << (32 - rotate_amount);
    return value;
}

inline word rotate_left(word value, int rotate_amount);

CPU::CPU(Memory& _mem)
    : mem(_mem) {
    PC = PAK_ROM_WAIT_STATE_0_START;
    state         = ARM_CODE;
}

CPU::~CPU() {
}

void CPU::run() {
    word instruction = mem.get_word(PC);
    switch (state) {
        case ARM_CODE:
            execute_ARM(instruction);
            break;
        case THUMB_CODE:
            execute_THUMB(instruction);
            break;
    }
}

word* CPU::get_reg(int reg) {
    return r[mode][reg];
}

bool CPU::check_condition(INSTRUCTION_CONDITION cond) {
    switch (cond) {
        case EQ:
            return check_flag(CPSR_BIT_FLAG::ZERO_FLAG);
        case NE:
            return !check_flag(CPSR_BIT_FLAG::ZERO_FLAG);
        case CS:
            return check_flag(CPSR_BIT_FLAG::CARRY_FLAG);
        case CC:
            return !check_flag(CPSR_BIT_FLAG::CARRY_FLAG);
        case MI:
            return check_flag(CPSR_BIT_FLAG::SIGN_FLAG);
        case PL:
            return !check_flag(CPSR_BIT_FLAG::SIGN_FLAG);
        case VS:
            return check_flag(CPSR_BIT_FLAG::OVERFLOW_FLAG);
        case VC:
            return !check_flag(CPSR_BIT_FLAG::OVERFLOW_FLAG);
        case HI:
            return check_flag(CPSR_BIT_FLAG::CARRY_FLAG) && !check_flag(CPSR_BIT_FLAG::ZERO_FLAG);
        case LS:
            return !check_flag(CPSR_BIT_FLAG::CARRY_FLAG) || check_flag(CPSR_BIT_FLAG::ZERO_FLAG);
        case GE:
            return check_flag(CPSR_BIT_FLAG::SIGN_FLAG) == check_flag(CPSR_BIT_FLAG::OVERFLOW_FLAG);
        case LT:
            return check_flag(CPSR_BIT_FLAG::SIGN_FLAG) != check_flag(CPSR_BIT_FLAG::OVERFLOW_FLAG);
        case GT:
            return !check_flag(CPSR_BIT_FLAG::ZERO_FLAG) && (check_flag(CPSR_BIT_FLAG::SIGN_FLAG) == check_flag(CPSR_BIT_FLAG::OVERFLOW_FLAG));
        case LE:
            return check_flag(CPSR_BIT_FLAG::ZERO_FLAG) || (check_flag(CPSR_BIT_FLAG::SIGN_FLAG) != check_flag(CPSR_BIT_FLAG::OVERFLOW_FLAG));
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
        case ARM_INSTRUCTION::ADC:
            // do something
            break;
        case ARM_INSTRUCTION::ADD:
            // do something
            break;
        case ARM_INSTRUCTION::AND:
            // do something
            break;
        case ARM_INSTRUCTION::B:
            // do something
            break;
        case ARM_INSTRUCTION::BIC:
            // do something
            break;
        case ARM_INSTRUCTION::BL:
            // do something
            break;
        case ARM_INSTRUCTION::BX:
            // do something
            break;
        case ARM_INSTRUCTION::CMN:
            // do something
            break;
        case ARM_INSTRUCTION::CMP:
            // do something
            break;
        case ARM_INSTRUCTION::EOR:
            // do something
            break;
        case ARM_INSTRUCTION::LDM:
            // do something
            break;
        case ARM_INSTRUCTION::LDR:
            // do something
            break;
        case ARM_INSTRUCTION::MLA:
            // do something
            break;
        case ARM_INSTRUCTION::MOV:
            // do something
            break;
        case ARM_INSTRUCTION::MRS:
            // do something
            break;
        case ARM_INSTRUCTION::MSR:
            // do something
            break;
        case ARM_INSTRUCTION::MUL:
            // do something
            break;
        case ARM_INSTRUCTION::MVN:
            // do something
            break;
        case ARM_INSTRUCTION::ORR:
            // do something
            break;
        case ARM_INSTRUCTION::RSB:
            // do something
            break;
        case ARM_INSTRUCTION::RSC:
            // do something
            break;
        case ARM_INSTRUCTION::SBC:
            // do something
            break;
        case ARM_INSTRUCTION::STM:
            // do something
            break;
        case ARM_INSTRUCTION::STR:
            // do something
            break;
        case ARM_INSTRUCTION::SUB:
            // do something
            break;
        case ARM_INSTRUCTION::SWI:
            // do something
            break;
        case ARM_INSTRUCTION::SWP:
            // do something
            break;
        case ARM_INSTRUCTION::TEQ:
            // do something
            break;
        case ARM_INSTRUCTION::TST:
            // do something
            break;
        case ARM_INSTRUCTION::NO_MATCH:
            // do something
            break;
#ifdef COPROC_SUPPORT
        case ARM_INSTRUCTION::CDP:  // CDP if coprocessor support
            // do something
            break;
        case ARM_INSTRUCTION::LDC:
            // do something
            break;
        case ARM_INSTRUCTION::MCR:
            // do something
            break;
        case ARM_INSTRUCTION::MRC:
            // do something
            break;

        case ARM_INSTRUCTION::STC:
            // do something
            break;
#endif
    }
}

ARM_INSTRUCTION CPU::decode_arm_instruction(word instruction) {
    if ((instruction & 0x0F000000) == 0x0F000000) return ARM_INSTRUCTION::SWI;
    if ((instruction & 0x0F000000) == 0x0B000000) return ARM_INSTRUCTION::BL;
    if ((instruction & 0x0F000000) == 0x0A000000) return ARM_INSTRUCTION::B;
    if ((instruction & 0x0E100000) == 0x08100000) return ARM_INSTRUCTION::LDM;
    if ((instruction & 0x0E100000) == 0x08000000) return ARM_INSTRUCTION::STM;
    // implement undefined instruction trap maybe
    if ((instruction & 0x0C100000) == 0x04100000) return ARM_INSTRUCTION::LDR;
    if ((instruction & 0x0C100000) == 0x04000000) return ARM_INSTRUCTION::STR;
    // skipped halfword data transfers as documentation is lacking
    if ((instruction & 0x0FFFFFF0) == 0x012FFF10) return ARM_INSTRUCTION::BX;
    if ((instruction & 0x0FB00FF0) == 0x01000090) return ARM_INSTRUCTION::SWP;
    // if ((instruction & 0x0FA000F0) == 0x00A00090) return MLAL; // maybe add MLAL to enum of ARM Instructions
    // if ((instruction & 0x0FA000F0) == 0x00800090) return MULL; // maybe add MLAL to enum of ARM Instructions
    if ((instruction & 0x0FE000F0) == 0x00200090) return ARM_INSTRUCTION::MLA;
    if ((instruction & 0x0FE000F0) == 0x00000090) return ARM_INSTRUCTION::MUL;
    if ((instruction & 0x0A000000) == 0x00000000) {  // data processing
        switch (instruction >> 21 & 0xF) {
            case 0x0:
                return ARM_INSTRUCTION::AND;
            case 0x1:
                return ARM_INSTRUCTION::EOR;
            case 0x2:
                return ARM_INSTRUCTION::SUB;
            case 0x3:
                return ARM_INSTRUCTION::RSB;
            case 0x4:
                return ARM_INSTRUCTION::ADD;
            case 0x5:
                return ARM_INSTRUCTION::ADC;
            case 0x6:
                return ARM_INSTRUCTION::SBC;
            case 0x7:
                return ARM_INSTRUCTION::RSC;
            case 0x8:
                return ARM_INSTRUCTION::TST;
            case 0x9:
                return ARM_INSTRUCTION::TEQ;
            case 0xA:
                return ARM_INSTRUCTION::CMP;
            case 0xB:
                return ARM_INSTRUCTION::CMN;
            case 0xC:
                return ARM_INSTRUCTION::ORR;
            case 0xD:
                return ARM_INSTRUCTION::MOV;
            case 0xE:
                return ARM_INSTRUCTION::BIC;
            case 0xF:
                return ARM_INSTRUCTION::MVN;
        }
    }
    if ((instruction & 0x0FBF0FFF) == 0x010F0000) return ARM_INSTRUCTION::MRS;
    if ((instruction & 0x0FBFFFF0) == 0x0129F000) return ARM_INSTRUCTION::MSR;
    if ((instruction & 0x0DBFF000) == 0x0128F000) return ARM_INSTRUCTION::MSR;
#ifdef COPROC_SUPPORT
    if ((instruction & 0x0F100010) == 0x0E100010) return ARM_INSTRUCTION::MRC;
    if ((instruction & 0x0F100010) == 0x0E000010) return ARM_INSTRUCTION::MCR;
    if ((instruction & 0x0F000010) == 0x0E000000) return ARM_INSTRUCTION::CDP;
    if ((instruction & 0x0E100000) == 0x0C100000) return ARM_INSTRUCTION::LDC;
    if ((instruction & 0x0E100000) == 0x0C000000) return ARM_INSTRUCTION::STC;
#endif
    log_error("Could not match ARM instruction");
    return ARM_INSTRUCTION::NO_MATCH;
}

THUMB_INSTRUCTION CPU::decode_thumb_instruction(word instruction) {
    if ((instruction & 0xE000) == 0x0000) {
        switch (instruction & 0x1800) {
            case 0x0000:
                return THUMB_INSTRUCTION::LSL;
            case 0x0800:
                return THUMB_INSTRUCTION::LSR;
            case 0x1000:
                return THUMB_INSTRUCTION::ASR;
        }
    }
    if ((instruction & 0xFA00) == 0x1A00) return THUMB_INSTRUCTION::ADD;
    if ((instruction & 0xFA00) == 0x1800) return THUMB_INSTRUCTION::SUB;
    if ((instruction & 0xE000) == 0x2000) {
        switch (instruction & 0x1800) {
            case 0x0000:
                return THUMB_INSTRUCTION::MOV;
            case 0x0800:
                return THUMB_INSTRUCTION::CMP;
            case 0x1000:
                return THUMB_INSTRUCTION::ADD;
            case 0x1800:
                return THUMB_INSTRUCTION::SUB;
        }
    }
    if ((instruction & 0xFC00) == 0x4000) {
        switch ((instruction >> 6) & 0xF) {
            case 0x0:
                return THUMB_INSTRUCTION::AND;
            case 0x1:
                return THUMB_INSTRUCTION::EOR;
            case 0x2:
                return THUMB_INSTRUCTION::LSL;
            case 0x3:
                return THUMB_INSTRUCTION::LSR;
            case 0x4:
                return THUMB_INSTRUCTION::ASR;
            case 0x5:
                return THUMB_INSTRUCTION::ADC;
            case 0x6:
                return THUMB_INSTRUCTION::SBC;
            case 0x7:
                return THUMB_INSTRUCTION::ROR;
            case 0x8:
                return THUMB_INSTRUCTION::TST;
            case 0x9:
                return THUMB_INSTRUCTION::NEG;
            case 0xA:
                return THUMB_INSTRUCTION::CMP;
            case 0xB:
                return THUMB_INSTRUCTION::CMN;
            case 0xC:
                return THUMB_INSTRUCTION::ORR;
            case 0xD:
                return THUMB_INSTRUCTION::MUL;
            case 0xE:
                return THUMB_INSTRUCTION::BIC;
            case 0xF:
                return THUMB_INSTRUCTION::MVN;
        }
    }
    if ((instruction & 0xFC00) == 0x4400) {
        switch (instruction & 0x0300) {
            case 0x0000:
                return THUMB_INSTRUCTION::ADD;
            case 0x0100:
                return THUMB_INSTRUCTION::ADD;
        }
    }
    return THUMB_INSTRUCTION::NO_MATCH;
}

void CPU::execute_THUMB(halfword instruction) {}

bool CPU::check_flag(CPSR_BIT_FLAG flag) {
    return CPSR & flag;
}

void CPU::arm_branch_exchange(word instruction) {
    word* reg = get_reg(instruction & 0xF);
    PC = *reg;
    if (*reg & 0x1) {
        state = THUMB_CODE;
    }
}

void CPU::arm_branch(word instruction) {
    int offset = (instruction & 0x00FFFFFF) << 2;
    PC += offset;
}

void CPU::arm_branch_link(word instruction) {
    int offset = (instruction & 0x00FFFFFF) << 2;
    *get_reg(14) = PC;
    PC += offset;
}

void CPU::arm_and(word instruction){
    if (is_bit_set(instruction, 25)) {
        
    } else {

    }
}
void CPU::arm_edor(word instruction){

}
void CPU::arm_sub(word instruction){

}
void CPU::arm_reverse_sub(word instruction){

}
void CPU::arm_add(word instruction){

}
void CPU::arm_add_carry(word instruction){

}
void CPU::arm_sub_carry(word instruction){

}
void CPU::arm_reverse_sub_carry(word instruction){

}
void CPU::arm_test_bits(word instruction){

}
void CPU::arm_test_bitwise_equality(word instruction){

}
void CPU::arm_compare(word instruction){

}
void CPU::arm_compare_negative(word instruction){

}
void CPU::arm_or(word instruction){

}
void CPU::arm_mov(word instruction){

}
void CPU::arm_bit_clear(word instruction){

}
void CPU::arm_mov_negative(word instruction){

}
void CPU::arm_mov_psr_reg(word instruction){

}
void CPU::arm_mov_reg_psr(word instruction){

}
void CPU::arm_multiply(word instruction){

}
void CPU::arm_multiply_accumulate(word instruction){

}
void CPU::arm_multiply_long(word instruction){

}
void CPU::arm_multiply_long_accumulate(word instruction){

}
void CPU::arm_store_reg_mem(word instruction){

}
void CPU::arm_load_mem_reg(word instruction){

}
void CPU::arm_store_reg_mem_halfword(word instruction){

}
void CPU::arm_load_mem_reg_halfword(word instruction){

}
void CPU::arm_store_reg_mem_byte(word instruction){

}
void CPU::arm_load_mem_reg_byte(word instruction){

}
void CPU::arm_store_multiple(word instruction){

}
void CPU::arm_load_multiple(word instruction){

}
void CPU::arm_single_data_swap(word instruction){

}
void CPU::arm_software_interrupt(word instruction){

}