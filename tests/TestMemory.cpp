// tests/TestMemory.cpp
#include <iostream>
#include <iomanip>
#include <cassert>
#include "Simulator/Memory.h"

int main()
{
    Simulator::Memory memory;

    // 메모리에 값 쓰기
    memory.write(0x3000, 0x1234);

    // 메모리에서 값 읽기
    uint16_t value = memory.read(0x3000);
    std::cout << "Value at 0x3000: 0x" << std::hex << std::setw(4) << std::setfill('0') << value << std::endl;

    // 값 검증
    assert(value == 0x1234 && "Memory read/write test failed.");

    std::cout << "All Memory tests passed successfully." << std::endl;

    return 0;
}