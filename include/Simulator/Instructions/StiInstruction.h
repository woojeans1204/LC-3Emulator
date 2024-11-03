// include/Simulator/Instructions/StiInstruction.h
#pragma once

#include "Instruction.h"
#include "LC3Emulator.h"

namespace Simulator
{
    namespace Instructions
    {
        /**
         * @brief STI 명령어를 구현하는 클래스입니다.
         */
        class StiInstruction : public Instruction
        {
        public:
            StiInstruction(uint16_t instr);
            ~StiInstruction() = default;

            void execute(LC3Emulator &emulator) override;

        private:
            uint16_t sr;
            int16_t pcOffset9;

            void decode();
        };
    } // namespace Instructions
} // namespace Simulator