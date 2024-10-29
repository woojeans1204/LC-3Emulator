// src/Simulator/Instructions/BrInstruction.cpp
#include "Instructions/BrInstruction.h"
#include "Simulator/LC3Emulator.h" // LC3Emulator 클래스 정의 포함

namespace Simulator
{
    namespace Instructions
    {

        BrInstruction::BrInstruction(uint16_t instr) : Instruction(instr)
        {
            decode();
        }

        void BrInstruction::decode()
        {
            n = (instructionCode >> 11) & 0x1;
            z = (instructionCode >> 10) & 0x1;
            p = (instructionCode >> 9) & 0x1;

            pcOffset9 = instructionCode & 0x1FF;
            // 부호 확장
            if (pcOffset9 & 0x100)
            {
                pcOffset9 |= 0xFE00;
            }
        }

        void BrInstruction::execute(LC3Emulator &emulator)
        {
            char cond = emulator.getRegisterFile().getCOND();
            bool shouldBranch = false;

            if ((n && cond == 'N') || (z && cond == 'Z') || (p && cond == 'P'))
            {
                shouldBranch = true;
            }

            if (shouldBranch)
            {
                uint16_t pc = emulator.getRegisterFile().readPC();
                emulator.getRegisterFile().writePC(pc + pcOffset9);
            }
        }

    } // namespace Instructions
} // namespace Simulator
