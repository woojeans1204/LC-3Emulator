// include/Simulator/Instructions/LdiInstruction.h
#pragma once

#include "Instruction.h"
#include "LC3Emulator.h"

namespace Simulator
{
    namespace Instructions
    {
        /**
         * @brief LDI 명령어를 구현하는 클래스입니다.
         */
        class LdiInstruction : public Instruction
        {
        public:
            LdiInstruction(uint16_t instr);
            ~LdiInstruction() = default;

            void execute(LC3Emulator &emulator) override;

        private:
            uint16_t dr;
            int16_t pcOffset9;

            void decode();
        };
    } // namespace Instructions
} // namespace Simulator