// include/Simulator/Instructions/JsrInstruction.h
#pragma once

#include "Instruction.h"
#include "LC3Emulator.h"

namespace Simulator
{
    namespace Instructions
    {
        /**
         * @brief JSR 명령어를 구현하는 클래스입니다.
         */
        class JsrInstruction : public Instruction
        {
        public:
            JsrInstruction(uint16_t instr);
            ~JsrInstruction() = default;

            void execute(LC3Emulator &emulator) override;

        private:
            int16_t pcOffset11;

            void decode();
        };
    } // namespace Instructions
} // namespace Simulator