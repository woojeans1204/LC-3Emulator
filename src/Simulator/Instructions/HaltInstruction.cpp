// src/Simulator/Instructions/HaltInstruction.cpp
#include "Instructions/HaltInstruction.h"

namespace Simulator
{
    namespace Instructions
    {

        HaltInstruction::HaltInstruction(uint16_t instr) : Instruction(instr)
        {
        }

        void HaltInstruction::decode()
        {
        }

        void HaltInstruction::execute(LC3Emulator &emulator)
        {
            // 에뮬레이터를 종료합니다.
            emulator.halt();
        }

    } // namespace Instructions
} // namespace Simulator
