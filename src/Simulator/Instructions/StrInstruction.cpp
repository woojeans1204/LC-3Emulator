// src/Simulator/Instructions/StrInstruction.cpp
#include "Instructions/StrInstruction.h"
#include "Simulator/LC3Emulator.h" // LC3Emulator 클래스 정의 포함

namespace Simulator
{
    namespace Instructions
    {

        StrInstruction::StrInstruction(uint16_t instr) : Instruction(instr), sr(0), baseR(0), offset6(0)
        {
            decode();
        }

        void StrInstruction::decode()
        {
            sr = (instructionCode >> 9) & 0x7;
            baseR = (instructionCode >> 6) & 0x7;
            offset6 = instructionCode & 0x3F;
            // 부호 확장
            if (offset6 & 0x20)
            {
                offset6 |= 0xFFC0;
            }
        }

        void StrInstruction::execute(LC3Emulator &emulator)
        {
            uint16_t baseVal = emulator.getRegisterFile().readRegister(baseR);
            uint16_t address = baseVal + offset6;
            uint16_t value = emulator.getRegisterFile().readRegister(sr);
            emulator.getMemory().write(address, value);
        }

    } // namespace Instructions
} // namespace Simulator