// tests/TestAssembler.cpp
#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>
#include <filesystem> // C++17 이상 필요
#include "Assembler/Assembler.h"

int main()
{

    // 테스트 디렉토리 설정
    std::filesystem::path testDir = "tests/Debug";
    std::filesystem::path inputFile = testDir / "input.asm";
    std::filesystem::path outputFile = testDir / "output.obj";

    // 어셈블리 코드 준비
    std::string assemblyCode = R"(
.ORIG x3000      ; 시작 주소 지정
START  ADD R1, R2, R3
       AND R4, R5, #10
       NOT R6, R7
       RET
.END             ; 프로그램 종료
)";

    // 입력 파일 작성
    try
    {
        std::ofstream inputStream(inputFile);
        if (!inputStream.is_open())
        {
            std::cerr << "Failed to create input.asm file at " << inputFile << std::endl;
            return 1;
        }
        inputStream << assemblyCode;
        inputStream.close();
    }
    catch (const std::ofstream::failure &e)
    {
        std::cerr << "Exception writing to input.asm: " << e.what() << std::endl;
        return 1;
    }

    // Assembler 인스턴스 생성 및 어셈블 수행
    Assembler::Assembler assembler;
    try
    {
        assembler.assemble(inputFile.string(), outputFile.string());
    }
    catch (const std::exception &e)
    {
        std::cerr << "Assembly failed: " << e.what() << std::endl;
        return 1;
    }

    // 어셈블 결과 파일 읽기
    std::vector<unsigned short> machineCodes;
    try
    {
        std::ifstream objStream(outputFile, std::ios::binary);
        if (!objStream.is_open())
        {
            std::cerr << "Failed to open output.obj file at " << outputFile << std::endl;
            return 1;
        }

        unsigned short code;
        while (objStream.read(reinterpret_cast<char *>(&code), sizeof(unsigned short)))
        {
            machineCodes.push_back(code);
        }
        objStream.close();
    }
    catch (const std::ifstream::failure &e)
    {
        std::cerr << "Exception reading output.obj: " << e.what() << std::endl;
        return 1;
    }

    // 예상 기계어 코드 (임의의 예시, 실제 인코딩에 따라 수정 필요)
    // 예를 들어:
    // ADD R1, R2, R3 -> opcode: 0001 (ADD), DR: R1, SR1: R2, SR2: R3 -> 0001 001 010 00011 -> 0x1243
    // AND R4, R5, #10 -> opcode: 0101 (AND), DR: R4, SR1: R5, immediate: 01010 -> 0101 100 101 1 01010 -> 0x54A A
    // NOT R6, R7 -> opcode: 1001 (NOT), DR: R6, SR: R7 -> 1001 110 111 111111 -> 0x9CE1
    // RET -> opcode: 1100 00000 111 -> 1100 0000 0010 0101 -> 0xC1C1
    // .ORIG x3000 and .END may not produce machine code entries depending on implementation

    // 실제 기계어 값은 어셈블러의 구현에 따라 다를 수 있으므로, 여기에 예시 값을 제공합니다.
    // 아래 값들은 실제 어셈블러의 인코딩 규칙에 따라 수정되어야 합니다.
    std::vector<unsigned short> expectedMachineCodes = {
        0x3000,
        0x1283, // ADD R1, R2, R3
        0x596A, // AND R4, R5, #10 (실제 값은 어셈블러 구현에 따라 다를 수 있음)
        0x9DFF, // NOT R6, R7
        0xC1C0  // RET
    };

    // 검증: 어셈블 결과와 예상 기계어 코드 비교
    bool testPassed = true;
    if (machineCodes.size() != expectedMachineCodes.size())
    {
        std::cerr << "Mismatch in number of machine codes. Expected: " << expectedMachineCodes.size()
                  << ", Got: " << machineCodes.size() << std::endl;
        testPassed = false;
    }
    else
    {
        for (size_t i = 0; i < expectedMachineCodes.size(); ++i)
        {
            if (machineCodes[i] != expectedMachineCodes[i])
            {
                std::cerr << "Mismatch at word " << i << ": Expected 0x" << std::hex << expectedMachineCodes[i]
                          << ", Got 0x" << machineCodes[i] << std::dec << std::endl;
                testPassed = false;
            }
        }
    }

    if (testPassed)
    {
        std::cout << "Assembler test passed successfully." << std::endl;
    }
    else
    {
        std::cerr << "Assembler test failed." << std::endl;
    }

    // 테스트 파일 정리 (선택 사항)
    // try
    // {
    //     std::filesystem::remove(inputFile);
    //     std::filesystem::remove(outputFile);
    // }
    // catch (const std::filesystem::filesystem_error &e)
    // {
    //     std::cerr << "Filesystem error during cleanup: " << e.what() << std::endl;
    // }

    return testPassed ? 0 : 1;
}
