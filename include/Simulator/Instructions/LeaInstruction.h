// include/Simulator/Instructions/LeaInstruction.h
#pragma once

#include "Instruction.h"
#include "LC3Emulator.h"

namespace Simulator
{
    namespace Instructions
    {
        /**
         * @brief LEA 명령어를 구현하는 클래스입니다.
         */
        class LeaInstruction : public Instruction
        {
        public:
            LeaInstruction(uint16_t instr);
            ~LeaInstruction() = default;

            void execute(LC3Emulator &emulator) override;

        private:
            uint16_t dr;
            int16_t pcOffset9;

            void decode();
        };
    } // namespace Instructions
} // namespace Simulator