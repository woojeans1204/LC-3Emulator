// include/Simulator/Instructions/Instruction.h
#pragma once

#include <cstdint>
#include "Simulator/LC3Emulator.h"

namespace Simulator
{
    namespace Instructions
    {

        /**
         * @brief 모든 명령어 클래스의 기본이 되는 추상 클래스입니다.
         */
        class Instruction
        {
        public:
            Instruction(uint16_t instr) : instructionCode(instr) {}
            virtual ~Instruction() = default;

            /**
             * @brief 명령어를 실행합니다.
             *
             * @param emulator LC3 에뮬레이터 객체
             */
            virtual void execute(LC3Emulator &emulator) = 0;

        protected:
            uint16_t instructionCode; // 기계어 명령어 코드
        };

    } // namespace Instructions
} // namespace Simulator

// 개별 명령어 헤더 포함
#include "AddInstruction.h"
#include "AndInstruction.h"
#include "NotInstruction.h"
#include "BrInstruction.h"
#include "JmpInstruction.h"
#include "HaltInstruction.h"