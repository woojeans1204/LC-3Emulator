// tests/TestController.cpp
#include <iostream>
#include <fstream>
#include "Controller.h"

int main()
{
    // 어셈블리 소스 파일 경로 및 오브젝트 파일 경로 설정
    std::string sourceFilePath = "tests/debug/Controller/Input.asm"; // array.asm, blur.asm, stack.asm을 Input.asm에 복사해 테스트
    std::string objFilePath = "tests/debug/Controller/test.obj";
    bool debugMode = false; // 디버그 모드 활성화

    // 테스트용 어셈블리 코드 생성
    //     std::ofstream sourceFile(sourceFilePath);
    //     if (!sourceFile.is_open())
    //     {
    //         std::cerr << "Error: Failed to create source file: " << sourceFilePath << std::endl;
    //         return 1;
    //     }

    //     // 간단한 어셈블리 프로그램 작성
    //     sourceFile << R"(
    // )";
    //     sourceFile.close();

    // Controller 객체 생성 및 실행
    Controller controller;
    controller.assembleAndRun(sourceFilePath, objFilePath, debugMode);

    return 0;
}
