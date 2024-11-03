// include/Simulator/Instructions/TrapInstruction.h
#pragma once

#include "Instruction.h"
#include "LC3Emulator.h"

namespace Simulator
{
    namespace Instructions
    {
        /**
         * @brief TRAP 명령어를 구현하는 클래스입니다.
         */
        class TrapInstruction : public Instruction
        {
        public:
            TrapInstruction(uint16_t instr);
            ~TrapInstruction() = default;

            void execute(LC3Emulator &emulator) override;

        private:
            uint8_t trapvect8;

            void decode();
        };
    } // namespace Instructions
} // namespace Simulator