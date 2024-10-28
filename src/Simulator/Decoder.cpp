// src/Simulator/Decoder.cpp
#include "Simulator/Decoder.h"
#include <stdexcept>
#include <string>

namespace Simulator
{

    std::unique_ptr<Instructions::Instruction> Decoder::decode(uint16_t instructionCode)
    {
        uint16_t opcode = (instructionCode >> 12) & 0xF; // 상위 4비트 추출

        switch (opcode)
        {
        case 0x1: // ADD
            return std::make_unique<Instructions::AddInstruction>(instructionCode);
        case 0x5: // AND
            return std::make_unique<Instructions::AndInstruction>(instructionCode);
        case 0x9: // NOT
            return std::make_unique<Instructions::NotInstruction>(instructionCode);
        case 0x0: // BR
            return std::make_unique<Instructions::BrInstruction>(instructionCode);
        case 0xC: // JMP/RET
            return std::make_unique<Instructions::JmpInstruction>(instructionCode);
        case 0xF: // TRAP/HALT
            return std::make_unique<Instructions::HaltInstruction>(instructionCode);

        // 다른 명령어 케이스들 추가
        default:
            throw std::runtime_error("Unsupported opcode: 0x" + std::to_string(opcode));
        }
    }

} // namespace Simulator