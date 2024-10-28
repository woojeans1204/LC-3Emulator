// include/Simulator/Instructions/AddInstruction.h
#pragma once

#include "Instruction.h"

namespace Simulator
{
    namespace Instructions
    {

        class AddInstruction : public Instruction
        {
        public:
            AddInstruction(uint16_t instr);
            ~AddInstruction() = default;

            void execute(LC3Emulator &emulator) override;

        private:
            uint16_t dr;   // 목적지 레지스터 번호
            uint16_t sr1;  // 소스 레지스터 1 번호
            uint16_t mode; // 모드 (0: 레지스터 모드, 1: 즉시값 모드)
            uint16_t sr2;  // 소스 레지스터 2 번호 (레지스터 모드에서 사용)
            int16_t imm5;  // 즉시값 (즉시값 모드에서 사용)

            void decode();
        };

    } // namespace Instructions
} // namespace Simulator