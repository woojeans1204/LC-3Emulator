// include/Simulator/Instructions/StInstruction.h
#pragma once

#include "Instruction.h"
#include "LC3Emulator.h"

namespace Simulator
{
    namespace Instructions
    {
        /**
         * @brief ST 명령어를 구현하는 클래스입니다.
         */
        class StInstruction : public Instruction
        {
        public:
            StInstruction(uint16_t instr);
            ~StInstruction() = default;

            void execute(LC3Emulator &emulator) override;

        private:
            uint16_t sr;
            int16_t pcOffset9;

            void decode();
        };
    } // namespace Instructions
} // namespace Simulator