// tests/TestEncoder.cpp
#include <iostream>
#include <cassert>
#include "Assembler/Encoder.h"

int main()
{
    Assembler::InstructionSet instrSet;

    // Test ADD Register Mode
    unsigned short addReg = instrSet.encodeAddRegister("R1", "R2", "R3");
    // Expected: 0001 001 010 000 011 => 0x1283
    assert(addReg == 0x1283 && "ADD Register Mode encoding failed.");

    // Test ADD Immediate Mode
    unsigned short addImm = instrSet.encodeAddImmediate("R1", "R2", -4);
    // Expected: 0001 001 010 1 11100 => 0x12BC
    assert(addImm == 0x12BC && "ADD Immediate Mode encoding failed.");

    // Test AND Register Mode
    unsigned short andReg = instrSet.encodeAndRegister("R1", "R2", "R3");
    // Expected: 0101 001 010 000 011 => 0x5283
    assert(andReg == 0x5283 && "AND Register Mode encoding failed.");

    // Test AND Immediate Mode
    unsigned short andImm = instrSet.encodeAndImmediate("R1", "R2", 5);
    // Expected: 0101 001 010 1 00101 => 0x52A5
    assert(andImm == 0x52A5 && "AND Immediate Mode encoding failed.");

    // Test BR (BRzp)
    unsigned short brzp = instrSet.encodeBr(false, true, true, 10);
    // Expected: 0000 011 000001010 => 0x060A
    assert(brzp == 0x060A && "BRzp encoding failed.");

    // Test NOT
    unsigned short notInstr = instrSet.encodeNot("R1", "R2");
    // Expected: 1001 001 010 111111 => 0x92BF
    assert(notInstr == 0x92BF && "NOT encoding failed.");

    // Test RET (JMP R7)
    unsigned short retInstr = instrSet.encodeRet();
    // Expected: 1100 000 111 000000 => 0xC1C0 (Actually, JMP R7 is 1100 111 000000 => 0xC1C0)
    assert(retInstr == 0xC1C0 && "RET encoding failed.");

    std::cout << "All InstructionSet tests passed successfully." << std::endl;

    return 0;
}