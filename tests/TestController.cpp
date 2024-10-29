// tests/TestController.cpp
#include <iostream>
#include <fstream>
#include "Controller.h"

int main()
{
    // 어셈블리 소스 파일 경로 및 오브젝트 파일 경로 설정
    std::string sourceFilePath = "tests/debug/Controller/input.asm";
    std::string objFilePath = "tests/debug/Controller/test.obj";
    bool debugMode = true; // 디버그 모드 활성화

    // 테스트용 어셈블리 코드 생성
    std::ofstream sourceFile(sourceFilePath);
    if (!sourceFile.is_open())
    {
        std::cerr << "Error: Failed to create source file: " << sourceFilePath << std::endl;
        return 1;
    }

    // 간단한 어셈블리 프로그램 작성
    sourceFile << R"(; Test Program using ADD, AND, NOT
    .ORIG x3000
    ; Initialize R0 to 5
    AND R0, R0, #0     ; R0 = 0
    ADD R0, R0, #5     ; R0 = R0 + 5

    ; Initialize R1 to -1 (All bits set to 1)
    NOT R1, R0         ; R1 = NOT R0
    ADD R1, R1, #0     ; R1 = R1 (No change)

    ; Compute R2 = R0 AND R1 (Should be 0)
    AND R2, R0, R1     ; R2 = R0 AND R1

    HALT
    .END
)";
    sourceFile.close();

    // Controller 객체 생성 및 실행
    Controller controller;
    controller.assembleAndRun(sourceFilePath, objFilePath, debugMode);

    return 0;
}