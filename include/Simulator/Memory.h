// include/Simulator/Memory.h
#pragma once

#include <array>
#include <cstdint>
#include <stdexcept>

namespace Simulator
{
    class Memory
    {
    public:
        Memory();
        ~Memory() = default;

        uint16_t read(uint16_t address) const;
        void write(uint16_t address, uint16_t value);
        void reset();

    private:
        static const size_t MEMORY_SIZE = 65536; // 2^16
        std::array<uint16_t, MEMORY_SIZE> memory;
    };

} // namespace Simulator