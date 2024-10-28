// src/Simulator/Instructions/AndInstruction.cpp
#include "Instructions/AndInstruction.h"

namespace Simulator
{
    namespace Instructions
    {

        AndInstruction::AndInstruction(uint16_t instr) : Instruction(instr)
        {
            decode();
        }

        void AndInstruction::decode()
        {
            dr = (instructionCode >> 9) & 0x7;
            sr1 = (instructionCode >> 6) & 0x7;
            mode = (instructionCode >> 5) & 0x1;

            if (mode == 0)
            {
                // 레지스터 모드
                sr2 = instructionCode & 0x7;
            }
            else
            {
                // 즉시값 모드
                imm5 = instructionCode & 0x1F;
                // 부호 확장
                if (imm5 & 0x10)
                {
                    imm5 |= 0xFFE0;
                }
            }
        }

        void AndInstruction::execute(LC3Emulator &emulator)
        {
            uint16_t val1 = emulator.getRegisterFile().readRegister(sr1);
            uint16_t result = 0;

            if (mode == 0)
            {
                // 레지스터 모드
                uint16_t val2 = emulator.getRegisterFile().readRegister(sr2);
                result = val1 & val2;
            }
            else
            {
                // 즉시값 모드
                result = val1 & imm5;
            }

            emulator.getRegisterFile().writeRegister(dr, result);
            emulator.getRegisterFile().updateCOND(result);
        }

    } // namespace Instructions
} // namespace Simulator