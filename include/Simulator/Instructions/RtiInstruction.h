// include/Simulator/Instructions/RtiInstruction.h
#pragma once

#include "Instruction.h"
#include "LC3Emulator.h"

namespace Simulator
{
    namespace Instructions
    {
        /**
         * @brief RTI 명령어를 구현하는 클래스입니다.
         */
        class RtiInstruction : public Instruction
        {
        public:
            RtiInstruction(uint16_t instr);
            ~RtiInstruction() = default;

            void execute(LC3Emulator &emulator) override;

        private:
            void decode();
        };
    } // namespace Instructions
} // namespace Simulator