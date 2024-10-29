// src/Simulator/LC3Emulator.cpp
#include "Simulator/LC3Emulator.h"

namespace Simulator
{

    LC3Emulator::LC3Emulator() : halted(false)
    {
        regFile.writePC(0x3000); // x3000으로 고정
    }

    RegisterFile &LC3Emulator::getRegisterFile()
    {
        return regFile;
    }

    Memory &LC3Emulator::getMemory()
    {
        return memory;
    }

    void LC3Emulator::run()
    {

        uint16_t pc = regFile.readPC();
        uint16_t instructionCode = memory.read(pc);
        regFile.writePC(pc + 1); // PC 증가

        auto instruction = decoder.decode(instructionCode);
        instruction->execute(*this);
    }

    void LC3Emulator::step()
    {

        uint16_t pc = regFile.readPC();
        uint16_t instructionCode = memory.read(pc);
        regFile.writePC(pc + 1); // PC 증가

        auto instruction = decoder.decode(instructionCode);
        instruction->execute(*this);
    }

    void LC3Emulator::loadProgram(const std::vector<std::pair<uint16_t, uint16_t>> &program)
    {
        for (const auto &instr : program)
        {
            memory.write(instr.first, instr.second);
        }
    }

    void LC3Emulator::halt()
    {
        halted = true;
    }

    bool LC3Emulator::isHalted() const
    {
        return halted;
    }

} // namespace Simulator