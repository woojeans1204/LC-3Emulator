// include/Simulator/Instructions/NotInstruction.h
#pragma once

#include "Instruction.h"

namespace Simulator
{
    namespace Instructions
    {

        /**
         * @brief NOT 명령어를 구현하는 클래스입니다.
         */
        class NotInstruction : public Instruction
        {
        public:
            NotInstruction(uint16_t instr);
            ~NotInstruction() = default;

            void execute(LC3Emulator &emulator) override;

        private:
            uint16_t dr; // 목적지 레지스터 번호
            uint16_t sr; // 소스 레지스터 번호

            /**
             * @brief 명령어 코드를 해석하여 멤버 변수를 초기화합니다.
             */
            void decode();
        };

    } // namespace Instructions
} // namespace Simulator
