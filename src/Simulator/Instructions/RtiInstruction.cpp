// src/Simulator/Instructions/RtiInstruction.cpp
#include "Instructions/RtiInstruction.h"
#include <iostream>
#include "Simulator/LC3Emulator.h" // LC3Emulator 클래스 정의 포함

namespace Simulator
{
    namespace Instructions
    {

        RtiInstruction::RtiInstruction(uint16_t instr) : Instruction(instr)
        {
            decode();
        }

        void RtiInstruction::decode()
        {
            // RTI는 명령어의 특정 필드를 사용하지 않으므로 별도의 디코드가 필요 없습니다.
        }

        void RtiInstruction::execute(LC3Emulator &emulator)
        {
            // RTI 명령어는 인터럽트 상태에서 복귀할 때 사용됩니다.
            // 현재 인터럽트 처리 로직이 없다면, RTI는 특별한 동작을 하지 않습니다.

            std::cerr << "RTI instruction encountered but interrupts are not implemented.\n";
            emulator.halt();
        }

    } // namespace Instructions
} // namespace Simulator