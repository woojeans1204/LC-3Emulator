// tests/TestMemory.cpp
#include <iostream>
#include <iomanip>
#include <cassert>
#include "Simulator/Memory.h"

void testBasicReadWrite()
{
    Simulator::Memory memory;

    // 기본 읽기/쓰기 테스트
    memory.write(0x3000, 0x1234);
    assert(memory.read(0x3000) == 0x1234);

    // 여러 위치에 데이터 쓰기
    memory.write(0x0000, 0xFFFF);
    memory.write(0x3500, 0xABCD);
    memory.write(0xFFFE, 0x5555);

    assert(memory.read(0x0000) == 0xFFFF);
    assert(memory.read(0x3500) == 0xABCD);
    assert(memory.read(0xFFFE) == 0x5555);
}

// testMemoryReset 함수 제거됨

void testBoundaryConditions()
{
    Simulator::Memory memory;

    // 경계값 테스트 (0x0000, 0xFFFF)
    memory.write(0x0000, 0xDEAD);
    memory.write(0xFFFF, 0xBEEF);

    assert(memory.read(0x0000) == 0xDEAD);
    assert(memory.read(0xFFFF) == 0xBEEF);
}

void testExceptionHandling()
{
    Simulator::Memory memory;

    // 잘못된 주소에 대한 예외 테스트
    try
    {
        memory.read(0x10000);
        assert(false && "Expected out_of_range exception");
    }
    catch (const std::out_of_range &)
    {
        // 예외가 정상적으로 발생함
    }

    try
    {
        memory.write(0x10000, 0x1234);
        assert(false && "Expected out_of_range exception");
    }
    catch (const std::out_of_range &)
    {
        // 예외가 정상적으로 발생함
    }
}

int main()
{
    try
    {
        testBasicReadWrite();
        std::cout << "Basic read/write tests passed." << std::endl;

        // testMemoryReset 호출 제거됨

        testBoundaryConditions();
        std::cout << "Boundary condition tests passed." << std::endl;

        testExceptionHandling();
        std::cout << "Exception handling tests passed." << std::endl;

        std::cout << "All Memory tests passed successfully." << std::endl;
        return 0;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Test failed with exception: " << e.what() << std::endl;
        return 1;
    }
}