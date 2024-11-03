// include/Simulator/Instructions/LdrInstruction.h
#pragma once

#include "Instruction.h"
#include "LC3Emulator.h"

namespace Simulator
{
    namespace Instructions
    {
        /**
         * @brief LDR 명령어를 구현하는 클래스입니다.
         */
        class LdrInstruction : public Instruction
        {
        public:
            LdrInstruction(uint16_t instr);
            ~LdrInstruction() = default;

            void execute(LC3Emulator &emulator) override;

        private:
            uint16_t dr;
            uint16_t baseR;
            int16_t offset6;

            void decode();
        };
    } // namespace Instructions
} // namespace Simulator