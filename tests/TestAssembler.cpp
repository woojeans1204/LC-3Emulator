// tests/TestAssembler.cpp
#include <iostream>
#include <fstream>
#include <cassert>
#include "Assembler/Assembler.h"

int main()
{
    Assembler::Assembler assembler;

    // 어셈블리 코드 준비
    std::string assemblyCode = R"(
; Simple program
START  ADD R1, R2, R3
END    HALT
)";

    // 입력 파일 작성
    std::string inputFile = "tests/Debug/input.asm";
    std::ofstream inputStream(inputFile);
    if (!inputStream.is_open())
    {
        std::cerr << "Failed to create input.asm file." << std::endl;
        return 1;
    }
    inputStream << assemblyCode;
    inputStream.close();

    // 출력 파일 경로
    std::string outputFile = "output.obj";

    // 어셈블링 수행
    try
    {
        assembler.firstPass(inputFile);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Assembly failed: " << e.what() << std::endl;
        return 1;
    }

    std::cout << "All Assembler tests passed successfully." << std::endl;

    return 0;
}