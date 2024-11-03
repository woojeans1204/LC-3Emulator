// src/Simulator/Instructions/JsrInstruction.cpp
#include "Instructions/JsrInstruction.h"
#include "Simulator/LC3Emulator.h" // LC3Emulator 클래스 정의 포함

namespace Simulator
{
    namespace Instructions
    {

        JsrInstruction::JsrInstruction(uint16_t instr) : Instruction(instr), pcOffset11(0)
        {
            decode();
        }

        void JsrInstruction::decode()
        {
            pcOffset11 = instructionCode & 0x07FF;
            // 부호 확장
            if (pcOffset11 & 0x0400)
            {
                pcOffset11 |= 0xF800;
            }
        }

        void JsrInstruction::execute(LC3Emulator &emulator)
        {
            // 현재 PC를 R7에 저장
            emulator.getRegisterFile().writeRegister(7, emulator.getRegisterFile().readPC());

            // PC 업데이트
            uint16_t newPC = emulator.getRegisterFile().readPC() + pcOffset11;
            emulator.getRegisterFile().writePC(newPC);
        }

    } // namespace Instructions
} // namespace Simulator