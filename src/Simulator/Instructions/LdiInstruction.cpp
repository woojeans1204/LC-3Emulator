// src/Simulator/Instructions/LdiInstruction.cpp
#include "Instructions/LdiInstruction.h"
#include "Simulator/LC3Emulator.h" // LC3Emulator 클래스 정의 포함

namespace Simulator
{
    namespace Instructions
    {

        LdiInstruction::LdiInstruction(uint16_t instr) : Instruction(instr), dr(0), pcOffset9(0)
        {
            decode();
        }

        void LdiInstruction::decode()
        {
            dr = (instructionCode >> 9) & 0x7;
            pcOffset9 = instructionCode & 0x1FF;
            // 부호 확장
            if (pcOffset9 & 0x100)
            {
                pcOffset9 |= 0xFE00;
            }
        }

        void LdiInstruction::execute(LC3Emulator &emulator)
        {
            // 간접 주소 계산
            uint16_t address1 = emulator.getRegisterFile().readPC() + pcOffset9;
            uint16_t address2 = emulator.getMemory().read(address1);
            uint16_t value = emulator.getMemory().read(address2);
            emulator.getRegisterFile().writeRegister(dr, value);
            emulator.getRegisterFile().updateCOND(value);
        }

    } // namespace Instructions
} // namespace Simulator