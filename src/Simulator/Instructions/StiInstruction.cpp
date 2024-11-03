// src/Simulator/Instructions/StiInstruction.cpp
#include "Instructions/StiInstruction.h"
#include "Simulator/LC3Emulator.h" // LC3Emulator 클래스 정의 포함

namespace Simulator
{
    namespace Instructions
    {

        StiInstruction::StiInstruction(uint16_t instr) : Instruction(instr), sr(0), pcOffset9(0)
        {
            decode();
        }

        void StiInstruction::decode()
        {
            sr = (instructionCode >> 9) & 0x7;
            pcOffset9 = instructionCode & 0x1FF;
            // 부호 확장
            if (pcOffset9 & 0x100)
            {
                pcOffset9 |= 0xFE00;
            }
        }

        void StiInstruction::execute(LC3Emulator &emulator)
        {
            uint16_t address1 = emulator.getRegisterFile().readPC() + pcOffset9;
            uint16_t address2 = emulator.getMemory().read(address1);
            uint16_t value = emulator.getRegisterFile().readRegister(sr);
            emulator.getMemory().write(address2, value);
        }

    } // namespace Instructions
} // namespace Simulator