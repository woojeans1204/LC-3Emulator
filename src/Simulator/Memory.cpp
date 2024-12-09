// src/Simulator/Memory.cpp
#include "Simulator/Memory.h"
#include <string>

namespace Simulator
{

    Memory::Memory()
    {
        // 메모리를 0으로 초기화
        memory.fill(0);
    }

    void Memory::reset()
    {
        // 메모리를 0으로 초기화
        memory.fill(0);
    }

    uint16_t Memory::read(uint16_t address) const
    {
        return memory[address];
    }

    void Memory::write(uint16_t address, uint16_t value)
    {
        memory[address] = value;
    }

} // namespace Simulator