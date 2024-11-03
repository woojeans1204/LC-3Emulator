// src/Simulator/Instructions/LeaInstruction.cpp
#include "Instructions/LeaInstruction.h"
#include "Simulator/LC3Emulator.h" // LC3Emulator 클래스 정의 포함

namespace Simulator
{
    namespace Instructions
    {

        LeaInstruction::LeaInstruction(uint16_t instr) : Instruction(instr), dr(0), pcOffset9(0)
        {
            decode();
        }

        void LeaInstruction::decode()
        {
            dr = (instructionCode >> 9) & 0x7;
            pcOffset9 = instructionCode & 0x1FF;
            // 부호 확장
            if (pcOffset9 & 0x100)
            {
                pcOffset9 |= 0xFE00;
            }
        }

        void LeaInstruction::execute(LC3Emulator &emulator)
        {
            uint16_t address = emulator.getRegisterFile().readPC() + pcOffset9;
            emulator.getRegisterFile().writeRegister(dr, address);
            emulator.getRegisterFile().updateCOND(dr);
        }

    } // namespace Instructions
} // namespace Simulator