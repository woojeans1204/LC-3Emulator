// src/Simulator/Instructions/TrapInstruction.cpp
#include "Instructions/TrapInstruction.h"
#include <iostream>
#include "Simulator/LC3Emulator.h" // LC3Emulator 클래스 정의 포함

namespace Simulator
{
    namespace Instructions
    {

        TrapInstruction::TrapInstruction(uint16_t instr) : Instruction(instr), trapvect8(0)
        {
            decode();
        }

        void TrapInstruction::decode()
        {
            trapvect8 = instructionCode & 0x00FF;
        }

        void TrapInstruction::execute(LC3Emulator &emulator)
        {
            // 현재 PC를 R7에 저장
            emulator.getRegisterFile().writeRegister(7, emulator.getRegisterFile().readPC());

            switch (trapvect8)
            {
            case 0x20: // GETC
            {
                char inputChar = std::cin.get();
                emulator.getRegisterFile().writeRegister(0, static_cast<uint16_t>(inputChar));
                emulator.getRegisterFile().updateCOND(0);
                break;
            }
            case 0x21: // OUT
            {
                char outputChar = static_cast<char>(emulator.getRegisterFile().readRegister(0) & 0xFF);
                std::cout << outputChar;
                break;
            }
            case 0x22: // PUTS
            {
                uint16_t address = emulator.getRegisterFile().readRegister(0);
                char outputChar = static_cast<char>(emulator.getMemory().read(address) & 0xFF);
                while (outputChar != '\0')
                {
                    std::cout << outputChar;
                    address++;
                    outputChar = static_cast<char>(emulator.getMemory().read(address) & 0xFF);
                }
                break;
            }
            case 0x23: // IN
            {
                std::cout << "Input a character: ";
                char inputChar = std::cin.get();
                emulator.getRegisterFile().writeRegister(0, static_cast<uint16_t>(inputChar));
                std::cout << inputChar;
                emulator.getRegisterFile().updateCOND(0);
                break;
            }
            case 0x24: // PUTSP
            {
                uint16_t address = emulator.getRegisterFile().readRegister(0);
                uint16_t word = emulator.getMemory().read(address);
                while (word != 0)
                {
                    char char1 = static_cast<char>(word & 0xFF);
                    std::cout << char1;
                    char char2 = static_cast<char>((word >> 8) & 0xFF);
                    if (char2 != '\0')
                    {
                        std::cout << char2;
                    }
                    address++;
                    word = emulator.getMemory().read(address);
                }
                break;
            }
            case 0x25: // HALT
            {
                std::cout << "HALT" << std::endl;
                emulator.halt();
                break;
            }
            default:
            {
                std::cerr << "Unknown TRAP vector: 0x" << std::hex << static_cast<int>(trapvect8) << std::endl;
                emulator.halt();
                break;
            }
            }

            // PC 업데이트
            emulator.getRegisterFile().writePC(emulator.getRegisterFile().readRegister(7) + 1); // TRAP 이후 PC는 R7 + 1
        }

    } // namespace Instructions
} // namespace Simulator