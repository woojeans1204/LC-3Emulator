// src/Simulator/Instructions/JsrrInstruction.cpp
#include "Instructions/JsrrInstruction.h"
#include "Simulator/LC3Emulator.h" // LC3Emulator 클래스 정의 포함

namespace Simulator
{
    namespace Instructions
    {

        JsrrInstruction::JsrrInstruction(uint16_t instr) : Instruction(instr), baseR(0)
        {
            decode();
        }

        void JsrrInstruction::decode()
        {
            baseR = (instructionCode >> 6) & 0x7;
        }

        void JsrrInstruction::execute(LC3Emulator &emulator)
        {
            // 현재 PC를 R7에 저장
            emulator.getRegisterFile().writeRegister(7, emulator.getRegisterFile().readPC());

            // PC 업데이트
            uint16_t newPC = emulator.getRegisterFile().readRegister(baseR);
            emulator.getRegisterFile().writePC(newPC);
        }

    } // namespace Instructions
} // namespace Simulator