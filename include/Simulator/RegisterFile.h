// include/Simulator/RegisterFile.h
#pragma once

#include <array>
#include <cstdint>
#include <stdexcept>

namespace Simulator
{

    class RegisterFile
    {
    public:
        RegisterFile();
        ~RegisterFile() = default;

        void reset();

        uint16_t readRegister(size_t regNumber) const;

        void writeRegister(size_t regNumber, uint16_t value);

        char getCOND() const;

        void updateCOND(uint16_t value);

        void writePC(uint16_t address);

        uint16_t readPC(void);

    private:
        static const size_t NUM_REGISTERS = 8;         // R0 ~ R7
        std::array<uint16_t, NUM_REGISTERS> registers; // R0 ~ R7
        char COND;                                     // 조건 레지스터 ('N', 'Z', 'P')
        uint16_t PC;

        void validateRegisterNumber(size_t regNumber) const;
    };

} // namespace Simulator