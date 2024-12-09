// tests/TestRegisterFile.cpp
#include <iostream>
#include <cassert>
#include <iomanip>
#include "Simulator/RegisterFile.h"

void testBasicOperations()
{
    Simulator::RegisterFile regFile;

    // 기본 읽기/쓰기 테스트
    regFile.writeRegister(0, 0x1234);
    assert(regFile.readRegister(0) == 0x1234 && "Basic write/read failed");

    // 모든 레지스터 테스트
    for (size_t i = 0; i < 8; i++)
    {
        regFile.writeRegister(i, i + 1);
        assert(regFile.readRegister(i) == i + 1 && "Register value mismatch");
    }
}

void testInvalidRegister()
{
    Simulator::RegisterFile regFile;
    bool exceptionThrown = false;

    try
    {
        regFile.readRegister(8);
    }
    catch (const std::out_of_range &)
    {
        exceptionThrown = true;
    }
    assert(exceptionThrown && "Should throw exception for invalid register read");

    exceptionThrown = false;
    try
    {
        regFile.writeRegister(8, 0);
    }
    catch (const std::out_of_range &)
    {
        exceptionThrown = true;
    }
    assert(exceptionThrown && "Should throw exception for invalid register write");
}

void testConditionFlags()
{
    Simulator::RegisterFile regFile;

    // 양수 테스트
    regFile.writeRegister(0, 1);
    regFile.updateCOND(1);
    assert(regFile.getCOND() == 'P' && "COND should be P for positive");

    // 0 테스트
    regFile.updateCOND(0);
    assert(regFile.getCOND() == 'Z' && "COND should be Z for zero");

    // 음수 테스트
    regFile.updateCOND(0xFFFF); // -1 in 2's complement
    assert(regFile.getCOND() == 'N' && "COND should be N for negative");
}

void testPCOperations()
{
    Simulator::RegisterFile regFile;

    // PC 초기값 테스트
    assert(regFile.readPC() == 0x3000 && "PC should be initialized to 0x3000");

    // PC 쓰기/읽기 테스트
    regFile.writePC(0x4000);
    assert(regFile.readPC() == 0x4000 && "PC write/read failed");
}

void testReset()
{
    Simulator::RegisterFile regFile;

    // 몇 개의 레지스터에 값을 쓰기
    regFile.writeRegister(0, 0x1234);
    regFile.writeRegister(7, 0x5678);
    regFile.writePC(0x4000);
    regFile.updateCOND(1);

    // 리셋 수행
    regFile.reset();

    // 리셋 후 상태 검증
    assert(regFile.readPC() == 0x3000 && "PC should be 0x3000 after reset");
    assert(regFile.getCOND() == 'Z' && "COND should be Z after reset");
    for (size_t i = 0; i < 8; i++)
    {
        assert(regFile.readRegister(i) == 0 && "Registers should be 0 after reset");
    }
}

int main()
{
    try
    {
        std::cout << "Running RegisterFile tests..." << std::endl;

        testBasicOperations();
        std::cout << "Basic operations test passed" << std::endl;

        testInvalidRegister();
        std::cout << "Invalid register test passed" << std::endl;

        testConditionFlags();
        std::cout << "Condition flags test passed" << std::endl;

        testPCOperations();
        std::cout << "PC operations test passed" << std::endl;

        testReset();
        std::cout << "Reset operation test passed" << std::endl;

        std::cout << "All tests passed successfully!" << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Test failed: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}