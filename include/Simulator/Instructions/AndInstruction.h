// include/Simulator/Instructions/AndInstruction.h
#pragma once

#include "Instruction.h"

namespace Simulator
{
    namespace Instructions
    {

        /**
         * @brief AND 명령어를 구현하는 클래스입니다.
         */
        class AndInstruction : public Instruction
        {
        public:
            AndInstruction(uint16_t instr);
            ~AndInstruction() = default;

            void execute(LC3Emulator &emulator) override;

        private:
            uint16_t dr;
            uint16_t sr1;
            uint16_t mode;
            uint16_t sr2;
            int16_t imm5;

            void decode();
        };

    } // namespace Instructions
} // namespace Simulator