// include/Simulator/Instructions/LdInstruction.h
#pragma once

#include "Instruction.h"
#include "LC3Emulator.h"

namespace Simulator
{
    namespace Instructions
    {
        /**
         * @brief LD 명령어를 구현하는 클래스입니다.
         */
        class LdInstruction : public Instruction
        {
        public:
            LdInstruction(uint16_t instr);
            ~LdInstruction() = default;

            void execute(LC3Emulator &emulator) override;

        private:
            uint16_t dr;
            int16_t pcOffset9;

            void decode();
        };
    } // namespace Instructions
} // namespace Simulator