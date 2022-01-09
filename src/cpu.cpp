#include "cpu.h"
#include <sys/types.h>

#include <bitset>
#include <iostream>
#include <functional>

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


CPU::CPU(Memory& _mem)
    : mem(_mem) {
    PC = PAK_ROM_WAIT_STATE_0_START;
    state         = ARM_CODE;
}

CPU::~CPU() {
}

void CPU::run() {
    word arm_instruction;
    halfword thumb_instruction;
    ARM_OP arm_op;
    THUMB_OP thumb_op;
    switch (state) {
    case ARM_CODE:
        arm_instruction = mem.get_word(PC);
        arm_op = decode_arm_instruction(arm_instruction);
        std::invoke(arm_op, this, arm_instruction);
        break;
    case THUMB_CODE:
        thumb_instruction = mem.get_halfword(PC);
        thumb_op = decode_thumb_instruction(thumb_instruction);
        std::invoke(thumb_op, this, thumb_instruction);
        break;
    }
}

word* CPU::get_reg(int r) {
    return reg[mode][r];
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
/*
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
*/
CPU::ARM_OP CPU::decode_arm_instruction(word instruction) {
    if ((instruction & 0x0F000000) == 0x0F000000) return &CPU::arm_software_interrupt;
    if ((instruction & 0x0F000000) == 0x0B000000) return &CPU::arm_branch_link;
    if ((instruction & 0x0F000000) == 0x0A000000) return &CPU::arm_branch;
    if ((instruction & 0x0E100000) == 0x08100000) return &CPU::arm_load_multiple;
    // implement undefined instruction trap maybe
    if ((instruction & 0x0C100000) == 0x04100000) return &CPU::arm_load_mem_reg;
    if ((instruction & 0x0C100000) == 0x04000000) return &CPU::arm_store_reg_mem;
    // skipped halfword data transfers as documentation is lacking
    if ((instruction & 0x0FFFFFF0) == 0x012FFF10) return &CPU::arm_branch_exchange;
    if ((instruction & 0x0FB00FF0) == 0x01000090) return &CPU::arm_single_data_swap;
    // if ((instruction & 0x0FA000F0) == 0x00A00090) return MLAL; // maybe add MLAL to enum of ARM Instructions
    // if ((instruction & 0x0FA000F0) == 0x00800090) return MULL; // maybe add MLAL to enum of ARM Instructions
    if ((instruction & 0x0FE000F0) == 0x00200090) return &CPU::arm_multiply_accumulate;
    if ((instruction & 0x0FE000F0) == 0x00000090) return &CPU::arm_multiply;
    if ((instruction & 0x0A000000) == 0x00000000) return &CPU::arm_data_processing; // data processing
    if ((instruction & 0x0FBF0FFF) == 0x010F0000) return &CPU::arm_mov_psr_reg;
    if ((instruction & 0x0FBFFFF0) == 0x0129F000) return &CPU::arm_mov_reg_psr;
    if ((instruction & 0x0DBFF000) == 0x0128F000) return &CPU::arm_mov_reg_psr;
#ifdef COPROC_SUPPORT
    if ((instruction & 0x0F100010) == 0x0E100010) return ARM_INSTRUCTION::MRC;
    if ((instruction & 0x0F100010) == 0x0E000010) return ARM_INSTRUCTION::MCR;
    if ((instruction & 0x0F000010) == 0x0E000000) return ARM_INSTRUCTION::CDP;
    if ((instruction & 0x0E100000) == 0x0C100000) return ARM_INSTRUCTION::LDC;
    if ((instruction & 0x0E100000) == 0x0C000000) return ARM_INSTRUCTION::STC;
#endif
    log_error("Could not match ARM instruction");
    return nullptr;
}

CPU::THUMB_OP CPU::decode_thumb_instruction(word instruction) {
    return nullptr;
    /*
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
    */
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

void CPU::arm_data_processing(word instruction) {
    word operand_1, operand_2, result;
    word* dest_reg = get_reg(instruction >> 12);
    bool set_overflow_flag, set_carry_flag, set_zero_flag, set_sign_flag;
    operand_1 = *get_reg(instruction >> 16);
    if (is_bit_set(instruction, 25)) {
        // Immediate value operand
        operand_2 = rotate_right(instruction & 0xFF, instruction >> 8 & 0xF);
    } else {
        operand_2 = *get_reg(instruction & 0xF);
        operand_2 <<= ((instruction & 0xFF0) >> 4);
        // Register Operand
    }
    switch (instruction >> 21 & 0xF) {
        case 0x0:   // AND
        result = operand_1 & operand_2;
        *dest_reg = result;
        break;
        case 0x1: // EOR
        result = operand_1 ^ operand_2;
        *dest_reg = result;
        break;
        case 0x2: // SUB
        result = operand_1 - operand_2;
        *dest_reg = result;
        break;
        case 0x3: // RSB
        result = operand_2 - operand_1;
        *dest_reg = result;
        break;
        case 0x4: // ADD
        result = operand_1 + operand_2;
        *dest_reg = result;
        break;
        case 0x5: // ADC
        result = operand_1 + operand_2 + is_bit_set(CPSR, 29);
        *dest_reg = result;
        break;
        case 0x6: // SBC
        result = operand_1 - operand_2 + is_bit_set(CPSR, 29) - 1;
        *dest_reg = result;
        break;
        case 0x7: // RSC
        result = operand_2 - operand_1 + is_bit_set(CPSR, 29) - 1;
        *dest_reg = result;
        break;
        case 0x8: // TST
        result = operand_1 & operand_2;
        break;
        case 0x9: // TEQ
        result = operand_1 ^ operand_2;
        break;
        case 0xA: // CMP
        result = operand_1 - operand_2;
        break;
        case 0xB: // CMN
        result = operand_1 + operand_2;
        break;
        case 0xC: // ORR
        result = operand_1 | operand_2;
        *dest_reg = result;
        break;
        case 0xD: // MOV
        result = operand_2;
        *dest_reg = result;
        break;
        case 0xE: // BIC
        result = operand_1 & (!operand_2);
        *dest_reg = result;
        break;
        case 0xF: // MVN
        result = !operand_2;
        *dest_reg = result;
        break;
    }
    if (is_bit_set(instruction, 20)) {
        if (dest_reg == &PC) {
            CPSR = *PSR[mode];
        } else {

        if (set_overflow_flag) 
            CPSR |= CPSR_BIT_FLAG::OVERFLOW_FLAG;
        if (set_carry_flag)
            CPSR |= CPSR_BIT_FLAG::CARRY_FLAG;
        if (set_zero_flag)
            CPSR |= CPSR_BIT_FLAG::ZERO_FLAG;
        if (set_sign_flag)
            CPSR |= CPSR_BIT_FLAG::SIGN_FLAG;
        }
    }
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