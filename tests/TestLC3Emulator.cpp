// tests/TestLC3Emulator.cpp
#include <iostream>
#include <cassert>
#include <vector>
#include "Simulator/LC3Emulator.h"

int main()
{
    Simulator::LC3Emulator emulator;

    // 간단한 프로그램 작성
    // 프로그램: R1 = R2 + R3 실행 후 HALT
    // 주소와 명령어의 쌍으로 프로그램 작성
    // 메모리 주소는 x3000부터 시작
    std::vector<std::pair<uint16_t, uint16_t>> program = {
        {0x3000, 0x1283}, // ADD R1, R2, R3
        {0x3001, 0xF025}  // HALT
    };

    // 레지스터 초기화
    emulator.getRegisterFile().writeRegister(2, 5);  // R2 = 5
    emulator.getRegisterFile().writeRegister(3, 10); // R3 = 10

    // 프로그램 로드
    emulator.loadProgram(program);

    // 에뮬레이터 실행
    emulator.run();

    // 결과 확인
    uint16_t r1 = emulator.getRegisterFile().readRegister(1);
    std::cout << "R1 after execution: " << r1 << " (Expected: 15)" << std::endl;
    assert(r1 == 15 && "LC3Emulator test failed");

    std::cout << "LC3Emulator test passed successfully." << std::endl;
    return 0;
}