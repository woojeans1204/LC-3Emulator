// src/Simulator/Instructions/JmpInstruction.cpp
#include "Instructions/JmpInstruction.h"
#include "Simulator/LC3Emulator.h" // LC3Emulator 클래스 정의 포함

namespace Simulator
{
    namespace Instructions
    {

        JmpInstruction::JmpInstruction(uint16_t instr) : Instruction(instr)
        {
            decode();
        }

        void JmpInstruction::decode()
        {
            baseR = (instructionCode >> 6) & 0x7;
            // 나머지 비트는 사용되지 않음
        }

        void JmpInstruction::execute(LC3Emulator &emulator)
        {
            uint16_t address = emulator.getRegisterFile().readRegister(baseR);
            emulator.getRegisterFile().writePC(address);
        }

    } // namespace Instructions
} // namespace Simulator
