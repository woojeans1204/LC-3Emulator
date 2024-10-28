// src/Simulator/Instructions/NotInstruction.cpp
#include "Instructions/NotInstruction.h"

namespace Simulator
{
    namespace Instructions
    {

        NotInstruction::NotInstruction(uint16_t instr) : Instruction(instr)
        {
            decode();
        }

        void NotInstruction::decode()
        {
            dr = (instructionCode >> 9) & 0x7;
            sr = (instructionCode >> 6) & 0x7;
            // 나머지 비트는 1이어야 함 (명령어 형식상)
        }

        void NotInstruction::execute(LC3Emulator &emulator)
        {
            uint16_t val = emulator.getRegisterFile().readRegister(sr);
            uint16_t result = ~val;

            emulator.getRegisterFile().writeRegister(dr, result);
            emulator.getRegisterFile().updateCOND(result);
        }

    } // namespace Instructions
} // namespace Simulator
