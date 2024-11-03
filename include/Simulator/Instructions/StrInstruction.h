// include/Simulator/Instructions/StrInstruction.h
#pragma once

#include "Instruction.h"
#include "LC3Emulator.h"

namespace Simulator
{
    namespace Instructions
    {
        /**
         * @brief STR 명령어를 구현하는 클래스입니다.
         */
        class StrInstruction : public Instruction
        {
        public:
            StrInstruction(uint16_t instr);
            ~StrInstruction() = default;

            void execute(LC3Emulator &emulator) override;

        private:
            uint16_t sr;
            uint16_t baseR;
            int16_t offset6;

            void decode();
        };
    } // namespace Instructions
} // namespace Simulator