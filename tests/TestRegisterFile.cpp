// tests/TestRegisterFile.cpp
#include <iostream>
#include <cassert>
#include <iomanip>
#include "Simulator/RegisterFile.h"

int main()
{
    Simulator::RegisterFile regFile;

    // 레지스터에 값 쓰기
    regFile.writeRegister(1, 0xFFFF); // R1에 -1 저장

    // 레지스터 읽기 및 COND 업데이트
    uint16_t r1 = regFile.readRegister(1);
    regFile.updateCOND(r1);
    std::cout << "R1: 0x" << std::hex << r1 << ", COND: " << regFile.getCOND() << std::endl;

    // 결과 확인
    assert(r1 == 0xFFFF && "R1 value mismatch");
    assert(regFile.getCOND() == 'N' && "COND should be 'N'");

    std::cout << "All tests passed successfully." << std::endl;
    return 0;
}