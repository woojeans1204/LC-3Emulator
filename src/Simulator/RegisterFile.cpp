// src/Simulator/RegisterFile.cpp
#include "Simulator/RegisterFile.h"
#include <sstream>

namespace Simulator
{
    RegisterFile::RegisterFile() : COND('Z')
    {
        registers.fill(0);
    }

    void RegisterFile::reset()
    {
        registers.fill(0);
        writePC(0x3000); // default
        updateCOND(0);
    }

    void RegisterFile::validateRegisterNumber(size_t regNumber) const
    {
        if (regNumber >= NUM_REGISTERS)
        {
            std::stringstream ss;
            ss << "Invalid register number: " << regNumber << ". Valid range is 0 to 7.";
            throw std::out_of_range(ss.str());
        }
    }

    uint16_t RegisterFile::readRegister(size_t regNumber) const
    {
        validateRegisterNumber(regNumber);
        return registers[regNumber];
    }

    void RegisterFile::writeRegister(size_t regNumber, uint16_t value)
    {
        validateRegisterNumber(regNumber);
        registers[regNumber] = value;
    }

    char RegisterFile::getCOND() const
    {
        return COND;
    }

    void RegisterFile::updateCOND(uint16_t value)
    {
        int16_t signedValue = static_cast<int16_t>(value);
        if (signedValue == 0)
        {
            COND = 'Z';
        }
        else if (signedValue < 0)
        {
            COND = 'N';
        }
        else
        {
            COND = 'P';
        }
    }

    void RegisterFile::writePC(uint16_t address)
    {
        PC = address;
    }

    uint16_t RegisterFile::readPC(void)
    {
        return PC;
    }

} // namespace Simulator