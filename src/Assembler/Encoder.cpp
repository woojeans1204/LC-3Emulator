// src/Assembler/Encoder.cpp
#include "Assembler/Encoder.h"
#include <iostream>
#include <stdexcept>
#include <cctype>

namespace Assembler
{

    int InstructionSet::getRegisterNumber(const std::string &reg) const
    {
        if (reg.size() != 2 || (reg[0] != 'R' && reg[0] != 'r') || !isdigit(reg[1]))
        {
            throw std::invalid_argument("Invalid register name: " + reg);
        }
        int regNum = reg[1] - '0';
        if (regNum < 0 || regNum > 7)
        {
            throw std::invalid_argument("Register number out of range (R0 ~ R7): " + reg);
        }
        return regNum;
    }

    unsigned short InstructionSet::encodeAddRegister(const std::string &dr, const std::string &sr1, const std::string &sr2)
    {
        // Opcode for ADD: 0001
        unsigned short opcode = 0x1 << 12;

        // DR (Destination Register)
        int drNum = getRegisterNumber(dr);
        opcode |= (drNum & 0x7) << 9;

        // SR1 (Source Register 1)
        int sr1Num = getRegisterNumber(sr1);
        opcode |= (sr1Num & 0x7) << 6;

        // Bit [5:3] = 000 for Register Mode
        opcode |= 0x0 << 3;

        // SR2 (Source Register 2)
        int sr2Num = getRegisterNumber(sr2);
        opcode |= (sr2Num & 0x7);
        return opcode;
    }

    unsigned short InstructionSet::encodeAddImmediate(const std::string &dr, const std::string &sr1, int imm5)
    {
        // Opcode for ADD: 0001
        unsigned short opcode = 0x1 << 12;

        // DR (Destination Register)
        int drNum = getRegisterNumber(dr);
        opcode |= (drNum & 0x7) << 9;

        // SR1 (Source Register 1)
        int sr1Num = getRegisterNumber(sr1);
        opcode |= (sr1Num & 0x7) << 6;

        // Bit [5] = 1 for Immediate Mode
        opcode |= 0x1 << 5;

        // imm5 (Immediate Value, 5 bits)
        if (imm5 < -16 || imm5 > 15)
        {
            throw std::invalid_argument("Immediate value out of range (-16 ~ +15): " + std::to_string(imm5));
        }
        // Convert to 5-bit two's complement
        unsigned short imm5Bits = static_cast<unsigned short>(imm5 & 0x1F);
        opcode |= imm5Bits;

        return opcode;
    }

    unsigned short InstructionSet::encodeAndRegister(const std::string &dr, const std::string &sr1, const std::string &sr2)
    {
        // Opcode for AND: 0101
        unsigned short opcode = 0x5 << 12;

        // DR (Destination Register)
        int drNum = getRegisterNumber(dr);
        opcode |= (drNum & 0x7) << 9;

        // SR1 (Source Register 1)
        int sr1Num = getRegisterNumber(sr1);
        opcode |= (sr1Num & 0x7) << 6;

        // Bit [5:3] = 000 for Register Mode
        opcode |= 0x0 << 3;

        // SR2 (Source Register 2)
        int sr2Num = getRegisterNumber(sr2);
        opcode |= (sr2Num & 0x7);

        return opcode;
    }

    unsigned short InstructionSet::encodeAndImmediate(const std::string &dr, const std::string &sr1, int imm5)
    {
        // Opcode for AND: 0101
        unsigned short opcode = 0x5 << 12;

        // DR (Destination Register)
        int drNum = getRegisterNumber(dr);
        opcode |= (drNum & 0x7) << 9;

        // SR1 (Source Register 1)
        int sr1Num = getRegisterNumber(sr1);
        opcode |= (sr1Num & 0x7) << 6;

        // Bit [5] = 1 for Immediate Mode
        opcode |= 0x1 << 5;

        // imm5 (Immediate Value, 5 bits)
        if (imm5 < -16 || imm5 > 15)
        {
            throw std::invalid_argument("Immediate value out of range (-16 ~ +15): " + std::to_string(imm5));
        }
        // Convert to 5-bit two's complement
        unsigned short imm5Bits = static_cast<unsigned short>(imm5 & 0x1F);
        opcode |= imm5Bits;

        return opcode;
    }

    unsigned short InstructionSet::encodeBr(bool n, bool z, bool p, int pcOffset9)
    {
        // Opcode for BR: 0000
        unsigned short opcode = 0x0 << 12;

        // Condition Codes
        if (n)
            opcode |= (1 << 11);
        if (z)
            opcode |= (1 << 10);
        if (p)
            opcode |= (1 << 9);

        // PCoffset9 (9 bits)
        if (pcOffset9 < -256 || pcOffset9 > 255)
        {
            throw std::invalid_argument("PCoffset9 out of range (-256 ~ +255): " + std::to_string(pcOffset9));
        }
        // Convert to 9-bit two's complement
        unsigned short pcOffset9Bits = static_cast<unsigned short>(pcOffset9 & 0x1FF);
        opcode |= pcOffset9Bits;

        return opcode;
    }

    unsigned short InstructionSet::encodeNot(const std::string &dr, const std::string &sr)
    {
        // Opcode for NOT: 1001
        unsigned short opcode = 0x9 << 12;

        // DR (Destination Register)
        int drNum = getRegisterNumber(dr);
        opcode |= (drNum & 0x7) << 9;

        // SR (Source Register)
        int srNum = getRegisterNumber(sr);
        opcode |= (srNum & 0x7) << 6;

        // Bit [5:0] = 111111 for NOT
        opcode |= 0x3F;

        return opcode;
    }

    unsigned short InstructionSet::encodeRet()
    {
        // Opcode for RET (JMP R7): 1100
        unsigned short opcode = 0xC << 12;

        // Base Register (R7)
        int baseRNum = 7;
        opcode |= (baseRNum & 0x7) << 6;

        // Bits [5:0] = 000000
        opcode |= 0x0;

        return opcode;
    }

    unsigned short InstructionSet::encodeHalt()
    {
        // Opcode for TRAP: 1111
        unsigned short opcode = 0xF << 12;

        // Trap vector for HALT: x25
        unsigned short trapvect8 = 0x25;

        opcode |= trapvect8;

        return opcode;
    }

    unsigned short InstructionSet::encodeJmp(const std::string &baseR)
    {
        // Opcode for JMP: 1100
        unsigned short opcode = 0xC << 12;

        // Base Register
        int baseRNum = getRegisterNumber(baseR);
        opcode |= (baseRNum & 0x7) << 6;

        // Bits [5:0] = 000000
        opcode |= 0x0;

        return opcode;
    }

    unsigned short InstructionSet::encodeLd(const std::string &dr, int pcOffset9)
    {
        // Opcode for LD: 0010
        unsigned short opcode = 0x2 << 12;

        // DR (Destination Register)
        int drNum = getRegisterNumber(dr);
        opcode |= (drNum & 0x7) << 9;

        // PCoffset9 (9 bits)
        if (pcOffset9 < -256 || pcOffset9 > 255)
        {
            throw std::invalid_argument("PCoffset9 out of range (-256 ~ +255): " + std::to_string(pcOffset9));
        }

        unsigned short pcOffset9Bits = static_cast<unsigned short>(pcOffset9 & 0x1FF);
        opcode |= pcOffset9Bits;

        return opcode;
    }

    unsigned short InstructionSet::encodeLdi(const std::string &dr, int pcOffset9)
    {
        // Opcode for LDI: 1010
        unsigned short opcode = 0xA << 12;

        // DR (Destination Register)
        int drNum = getRegisterNumber(dr);
        opcode |= (drNum & 0x7) << 9;

        // PCoffset9 (9 bits)
        if (pcOffset9 < -256 || pcOffset9 > 255)
        {
            throw std::invalid_argument("PCoffset9 out of range (-256 ~ +255): " + std::to_string(pcOffset9));
        }

        unsigned short pcOffset9Bits = static_cast<unsigned short>(pcOffset9 & 0x1FF);
        opcode |= pcOffset9Bits;

        return opcode;
    }

    unsigned short InstructionSet::encodeLdr(const std::string &dr, const std::string &baseR, int offset6)
    {
        // Opcode for LDR: 0110
        unsigned short opcode = 0x6 << 12;

        // DR (Destination Register)
        int drNum = getRegisterNumber(dr);
        opcode |= (drNum & 0x7) << 9;

        // Base Register
        int baseRNum = getRegisterNumber(baseR);
        opcode |= (baseRNum & 0x7) << 6;

        // offset6 (6 bits)
        if (offset6 < -32 || offset6 > 31)
        {
            throw std::invalid_argument("offset6 out of range (-32 ~ +31): " + std::to_string(offset6));
        }

        unsigned short offset6Bits = static_cast<unsigned short>(offset6 & 0x3F);
        opcode |= offset6Bits;

        return opcode;
    }

    unsigned short InstructionSet::encodeLea(const std::string &dr, int pcOffset9)
    {
        // Opcode for LEA: 1110
        unsigned short opcode = 0xE << 12;

        // DR (Destination Register)
        int drNum = getRegisterNumber(dr);
        opcode |= (drNum & 0x7) << 9;

        // PCoffset9 (9 bits)
        if (pcOffset9 < -256 || pcOffset9 > 255)
        {
            throw std::invalid_argument("PCoffset9 out of range (-256 ~ +255): " + std::to_string(pcOffset9));
        }

        unsigned short pcOffset9Bits = static_cast<unsigned short>(pcOffset9 & 0x1FF);
        opcode |= pcOffset9Bits;

        return opcode;
    }

    unsigned short InstructionSet::encodeSt(const std::string &sr, int pcOffset9)
    {
        // Opcode for ST: 0011
        unsigned short opcode = 0x3 << 12;

        // SR (Source Register)
        int srNum = getRegisterNumber(sr);
        opcode |= (srNum & 0x7) << 9;

        // PCoffset9 (9 bits)
        if (pcOffset9 < -256 || pcOffset9 > 255)
        {
            throw std::invalid_argument("PCoffset9 out of range (-256 ~ +255): " + std::to_string(pcOffset9));
        }

        unsigned short pcOffset9Bits = static_cast<unsigned short>(pcOffset9 & 0x1FF);
        opcode |= pcOffset9Bits;

        return opcode;
    }

    unsigned short InstructionSet::encodeSti(const std::string &sr, int pcOffset9)
    {
        // Opcode for STI: 1011
        unsigned short opcode = 0xB << 12;

        // SR (Source Register)
        int srNum = getRegisterNumber(sr);
        opcode |= (srNum & 0x7) << 9;

        // PCoffset9 (9 bits)
        if (pcOffset9 < -256 || pcOffset9 > 255)
        {
            throw std::invalid_argument("PCoffset9 out of range (-256 ~ +255): " + std::to_string(pcOffset9));
        }

        unsigned short pcOffset9Bits = static_cast<unsigned short>(pcOffset9 & 0x1FF);
        opcode |= pcOffset9Bits;

        return opcode;
    }

    unsigned short InstructionSet::encodeStr(const std::string &sr, const std::string &baseR, int offset6)
    {
        // Opcode for STR: 0111
        unsigned short opcode = 0x7 << 12;

        // SR (Source Register)
        int srNum = getRegisterNumber(sr);
        opcode |= (srNum & 0x7) << 9;

        // Base Register
        int baseRNum = getRegisterNumber(baseR);
        opcode |= (baseRNum & 0x7) << 6;

        // offset6 (6 bits)
        if (offset6 < -32 || offset6 > 31)
        {
            throw std::invalid_argument("offset6 out of range (-32 ~ +31): " + std::to_string(offset6));
        }

        unsigned short offset6Bits = static_cast<unsigned short>(offset6 & 0x3F);
        opcode |= offset6Bits;

        return opcode;
    }

    unsigned short InstructionSet::encodeTrap(unsigned char trapvect8)
    {
        // Opcode for TRAP: 1111
        unsigned short opcode = 0xF << 12;

        // trapvect8 (8 bits)
        opcode |= (trapvect8 & 0xFF);

        return opcode;
    }

} // namespace Assembler