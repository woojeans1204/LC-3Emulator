// src/Controller.cpp
#include "Controller.h"
#include <iostream>
#include <fstream>
#include <sstream>

Controller::Controller()
    : assembler(), emulator()
{
    // 필요한 초기화 코드가 있다면 추가
}

void Controller::assembleAndRun(const std::string &sourceFilePath, const std::string &objfilePath, bool debugMode)
{
    // 1. 어셈블리 소스 파일 읽기
    // (생략 가능, 어셈블러에서 파일 경로를 직접 사용)

    // 2. 어셈블리 소스 코드 어셈블
    try
    {
        assembler.assemble(sourceFilePath, objfilePath);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Assembly Error: " << e.what() << std::endl;
        return;
    }

    // 3. 오브젝트 파일을 시뮬레이터에 로드
    loadObjectCodeToEmulator(objfilePath);

    // 4. 시뮬레이터 실행
    runEmulator(debugMode);
}

void Controller::loadObjectCodeToEmulator(const std::string &objfilePath)
{
    // 오브젝트 파일 열기
    std::ifstream objFile(objfilePath, std::ios::binary);
    if (!objFile.is_open())
    {
        std::cerr << "Error: Failed to open object file: " << objfilePath << std::endl;
        return;
    }

    // 오브젝트 파일에서 데이터 읽기
    std::vector<std::pair<uint16_t, uint16_t>> objectCode;

    uint16_t instruction;
    uint16_t address;
    bool isAddress = true;

    while (objFile.read(reinterpret_cast<char *>(&instruction), sizeof(uint16_t)))
    {
        // 엔디안 변환 불필요
        if (isAddress)
        {
            address = instruction;
            isAddress = false;
        }
        else
        {
            objectCode.emplace_back(address, instruction);
            isAddress = true;
        }
    }

    objFile.close();

    // 에뮬레이터에 프로그램 로드
    emulator.loadProgram(objectCode);
}

unsigned short Controller::readMemory(unsigned short n)
{
    return emulator.getMemory().read(n);
}

unsigned short Controller::readRegister(unsigned short n)
{
    return emulator.getRegisterFile().readRegister(n);
}

void Controller::runEmulator(bool debugMode)
{
    if (debugMode)
    {
        // 디버그 모드: 스텝 실행 및 상태 출력
        while (!emulator.isHalted())
        {
            emulator.step(); // 한 명령어 실행
            // 레지스터 상태 출력
            std::cout << "PC: 0x" << std::hex << emulator.getRegisterFile().readPC() << std::endl;
            for (int i = 0; i < 8; ++i)
            {
                std::cout << "R" << i << ": 0x" << std::hex << emulator.getRegisterFile().readRegister(i) << " ";
                if (i % 4 == 3)
                    std::cout << std::endl;
            }
            std::cout << "COND: " << emulator.getRegisterFile().getCOND() << std::endl;
            std::cout << "-----------------------------" << std::endl;
            // 사용자 입력 대기
            std::cout << "Press Enter to continue..." << std::endl;
            std::cin.get();
        }
    }
    else
    {
        // 일반 모드: 전체 실행
        emulator.run();
    }

    std::cout << "Program execution finished." << std::endl;
    // 최종 레지스터 상태 출력
    std::cout << "Final Register State:" << std::endl;
    for (int i = 0; i < 8; ++i)
    {
        printf("R%d: x%.4x ", i, emulator.getRegisterFile().readRegister(i));
        if (i % 4 == 3)
            std::cout << std::endl;
    }
    std::cout << "COND: " << emulator.getRegisterFile().getCOND() << "\n\n";

    if (debugMode)
    {
        unsigned short readStart, readEnd;
        std::string input;
        while (true)
        {
            std::cout << "Reading memory address in hex from start to end (e.g., 3000 300A): ";
            std::cin >> std::hex >> readStart >> readEnd;

            // 메모리 주소 범위 출력
            for (unsigned short i = readStart; i <= readEnd; i++)
                printf("x%.4x: x%.4x\n", i, readMemory(i));

            // 루프를 계속할지 종료할지 물어보기
            std::cout << "Press Enter to continue or type anything else to quit: ";
            std::cin.ignore(); // 이전 입력의 개행 문자 제거
            std::getline(std::cin, input);

            if (!input.empty())
                break;
        }
    }
}
