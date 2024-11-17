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
        if (address >= MEMORY_SIZE)
        {
            throw std::out_of_range("Memory read error: Address out of range (0x0000 ~ 0xFFFF). Address: 0x" + std::to_string(address));
        }
        return memory[address];
    }

    void Memory::write(uint16_t address, uint16_t value)
    {
        if (address >= MEMORY_SIZE)
        {
            throw std::out_of_range("Memory write error: Address out of range (0x0000 ~ 0xFFFF). Address: 0x" + std::to_string(address));
        }
        memory[address] = value;
    }

} // namespace Simulator