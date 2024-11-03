// include/Simulator/Instructions/JsrrInstruction.h
#pragma once

#include "Instruction.h"
#include "LC3Emulator.h"

namespace Simulator
{
    namespace Instructions
    {
        /**
         * @brief JSRR 명령어를 구현하는 클래스입니다.
         */
        class JsrrInstruction : public Instruction
        {
        public:
            JsrrInstruction(uint16_t instr);
            ~JsrrInstruction() = default;

            void execute(LC3Emulator &emulator) override;

        private:
            uint16_t baseR;

            void decode();
        };
    } // namespace Instructions
} // namespace Simulator