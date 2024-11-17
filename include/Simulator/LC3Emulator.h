// include/Simulator/LC3Emulator.h
#pragma once

#include "RegisterFile.h"
#include "Memory.h"
#include "Decoder.h"
#include <vector>
namespace Simulator
{

    class LC3Emulator
    {
    public:
        LC3Emulator();
        ~LC3Emulator() = default;

        RegisterFile &getRegisterFile();
        Memory &getMemory();

        void loadProgram(const std::vector<std::pair<uint16_t, uint16_t>> &program);
        void run();
        void step();
        void reset();
        void halt();
        bool isHalted() const;

    private:
        Decoder decoder;
        RegisterFile regFile;
        Memory memory;
        bool halted;
    };

} // namespace Simulator