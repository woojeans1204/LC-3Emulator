// src/Simulator/Decoder.cpp
#include "Decoder.h"

#include <stdexcept>
#include <string>

// 개별 명령어 헤더 포함
#include "Instructions/AddInstruction.h"
#include "Instructions/AndInstruction.h"
#include "Instructions/BrInstruction.h"
#include "Instructions/HaltInstruction.h"
#include "Instructions/JmpInstruction.h"
#include "Instructions/JmpInstruction.h"
#include "Instructions/JsrInstruction.h"
#include "Instructions/JsrrInstruction.h"
#include "Instructions/LdiInstruction.h"
#include "Instructions/LdInstruction.h"
#include "Instructions/LdrInstruction.h"
#include "Instructions/LeaInstruction.h"
#include "Instructions/NotInstruction.h"
#include "Instructions/RtiInstruction.h"
#include "Instructions/StiInstruction.h"
#include "Instructions/StInstruction.h"
#include "Instructions/StrInstruction.h"
#include "Instructions/TrapInstruction.h"

namespace Simulator
{
    std::unique_ptr<Instructions::Instruction> Decoder::decode(uint16_t instructionCode)
    {
        uint16_t opcode = (instructionCode >> 12) & 0xF; // 상위 4비트 추출

        switch (opcode)
        {
        case 0x0: // BR
            return std::make_unique<Instructions::BrInstruction>(instructionCode);
        case 0x1: // ADD
            return std::make_unique<Instructions::AddInstruction>(instructionCode);
        case 0x2: // LD
            return std::make_unique<Instructions::LdInstruction>(instructionCode);
        case 0x3: // ST
            return std::make_unique<Instructions::StInstruction>(instructionCode);
        case 0x4: // JSR / JSRR
        {
            bool isJsrr = ((instructionCode >> 11) & 0x1) == 1;
            if (isJsrr)
            {
                return std::make_unique<Instructions::JsrrInstruction>(instructionCode);
            }
            else
            {
                return std::make_unique<Instructions::JsrInstruction>(instructionCode);
            }
        }
        case 0x5: // AND
            return std::make_unique<Instructions::AndInstruction>(instructionCode);
        case 0x6: // LDR
            return std::make_unique<Instructions::LdrInstruction>(instructionCode);
        case 0x7: // STR
            return std::make_unique<Instructions::StrInstruction>(instructionCode);
        case 0x8: // RTI
            return std::make_unique<Instructions::RtiInstruction>(instructionCode);
        case 0x9: // NOT
            return std::make_unique<Instructions::NotInstruction>(instructionCode);
        case 0xA: // LDI
            return std::make_unique<Instructions::LdiInstruction>(instructionCode);
        case 0xB: // STI
            return std::make_unique<Instructions::StiInstruction>(instructionCode);
        case 0xC: // JMP / RET
            return std::make_unique<Instructions::JmpInstruction>(instructionCode);
        case 0xE: // LEA
            return std::make_unique<Instructions::LeaInstruction>(instructionCode);
        case 0xF: // TRAP / HALT
            return std::make_unique<Instructions::TrapInstruction>(instructionCode);
        default:
            throw std::runtime_error("Unsupported opcode: 0x" + std::to_string(opcode));
        }
    }
} // namespace Simulator
