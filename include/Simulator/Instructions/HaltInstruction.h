// include/Simulator/Instructions/HaltInstruction.h
#pragma once

#include "Instruction.h"

namespace Simulator
{
    namespace Instructions
    {

        /**
         * @brief HALT 명령어를 구현하는 클래스입니다.
         */
        class HaltInstruction : public Instruction
        {
        public:
            HaltInstruction(uint16_t instr);
            ~HaltInstruction() = default;

            void execute(LC3Emulator &emulator) override;

        private:
            /**
             * @brief 명령어 코드는 따로 해석할 필요가 없습니다.
             */
            void decode();
        };

    } // namespace Instructions
} // namespace Simulator
