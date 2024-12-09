#include <iostream>
#include <cassert>
#include <vector>
#include "Simulator/LC3Emulator.h"

void testBasicOperations(Simulator::LC3Emulator &emulator)
{
    // ADD 명령어 테스트
    std::vector<std::pair<uint16_t, uint16_t>> addProgram = {
        {0x3000, 0x1642}, // ADD R3, R1, R2
        {0x3001, 0xF025}  // HALT
    };

    emulator.reset();
    emulator.getRegisterFile().writeRegister(1, 5);
    emulator.getRegisterFile().writeRegister(2, 10);
    emulator.loadProgram(addProgram);
    emulator.run();

    assert(emulator.getRegisterFile().readRegister(3) == 15 && "ADD instruction failed");
}

void testMemoryOperations(Simulator::LC3Emulator &emulator)
{
    // ST와 LD 테스트
    std::vector<std::pair<uint16_t, uint16_t>> memoryProgram = {
        {0x3000, 0x3201}, // ST R1, LABEL
        {0x3001, 0x2401}, // LD R2, LABEL
        {0x3002, 0xF025}, // HALT
        {0x3003, 0x0000}  // LABEL
    };

    emulator.reset();
    emulator.getRegisterFile().writeRegister(1, 0x1234);
    emulator.loadProgram(memoryProgram);
    emulator.run();

    assert(emulator.getRegisterFile().readRegister(2) == 0x1234 && "Memory operations failed");
}

void testBranching(Simulator::LC3Emulator &emulator)
{
    // 조건부 분기 테스트
    std::vector<std::pair<uint16_t, uint16_t>> branchProgram = {
        {0x3000, 0x1020}, // ADD R0, R0, 0    ; sets CC to Z
        {0x3001, 0x0801}, // BRnz NEXT        ; should branch
        {0x3002, 0x1220}, // ADD R1, R0, 0    ; should be skipped
        {0x3003, 0xF025}, // NEXT: HALT
    };

    emulator.reset();
    emulator.loadProgram(branchProgram);
    emulator.run();

    assert(emulator.getRegisterFile().readRegister(1) == 0 && "Branch instruction failed");
}

void testErrorCases(Simulator::LC3Emulator &emulator)
{
    // 잘못된 메모리 접근 테스트
    try
    {
        emulator.getMemory().write(0xFFFF + 1, 0);
        assert(false && "Should throw out_of_range exception");
    }
    catch (const std::out_of_range &)
    {
        // 예상된 동작
    }

    // 잘못된 레지스터 접근 테스트
    try
    {
        emulator.getRegisterFile().writeRegister(8, 0);
        assert(false && "Should throw out_of_range exception");
    }
    catch (const std::out_of_range &)
    {
        // 예상된 동작
    }
}

int main()
{
    Simulator::LC3Emulator emulator;

    std::cout << "Starting LC3Emulator tests...\n";

    testBasicOperations(emulator);
    std::cout << "Basic operations test passed\n";

    testMemoryOperations(emulator);
    std::cout << "Memory operations test passed\n";

    testBranching(emulator);
    std::cout << "Branching test passed\n";

    testErrorCases(emulator);
    std::cout << "Error cases test passed\n";

    std::cout << "All tests passed successfully.\n";
    return 0;
}