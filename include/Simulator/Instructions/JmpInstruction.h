// include/Simulator/Instructions/JmpInstruction.h
#pragma once

#include "Instruction.h"

namespace Simulator
{
    namespace Instructions
    {

        /**
         * @brief Jmp 명령어를 구현하는 클래스입니다.
         */
        class JmpInstruction : public Instruction
        {
        public:
            JmpInstruction(uint16_t instr);
            ~JmpInstruction() = default;

            void execute(LC3Emulator &emulator) override;

        private:
            uint16_t baseR; // 베이스 레지스터 번호

            /**
             * @brief 명령어 코드를 해석하여 멤버 변수를 초기화합니다.
             */
            void decode();
        };

    } // namespace Instructions
} // namespace Simulator
