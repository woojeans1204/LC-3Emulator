// include/Simulator/Instructions/BrInstruction.h
#pragma once

#include "Instruction.h"

namespace Simulator
{
    namespace Instructions
    {

        /**
         * @brief BR (Branch) 명령어를 구현하는 클래스입니다.
         */
        class BrInstruction : public Instruction
        {
        public:
            BrInstruction(uint16_t instr);
            ~BrInstruction() = default;

            void execute(LC3Emulator &emulator) override;

        private:
            uint16_t n;        // Negative 조건 플래그
            uint16_t z;        // Zero 조건 플래그
            uint16_t p;        // Positive 조건 플래그
            int16_t pcOffset9; // PC 오프셋 (부호 확장 필요)

            /**
             * @brief 명령어 코드를 해석하여 멤버 변수를 초기화합니다.
             */
            void decode();
        };

    } // namespace Instructions
} // namespace Simulator
