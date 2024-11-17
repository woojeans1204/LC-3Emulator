// src/Controller.cpp
#include "Controller.h"
#include <iostream>
#include <fstream>
#include <sstream>

Controller::Controller()
    : assembler(), emulator(), stepCnt(0), debugMode(false), isSuccess(true)
{
    // 필요한 초기화 코드가 있다면 추가
}

void Controller::assemble(const std::string &sourceFilePath, const std::string &objfilePath)
{
    try
    {
        assembler.assemble(sourceFilePath, objfilePath);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Assembly Error: " << e.what() << std::endl;
        isSuccess = false;
        return;
    }
}

void Controller::assembleRun(const std::string &sourceFilePath, const std::string &objfilePath)
{
    // 1. 어셈블리 소스 파일 읽기
    // (생략 가능, 어셈블러에서 파일 경로를 직접 사용)

    // 2. 어셈블리 소스 코드 어셈블
    assemble(sourceFilePath, objfilePath);

    // 3. 오브젝트 파일을 시뮬레이터에 로드
    loadProgram(objfilePath);

    // 4. 시뮬레이터 실행
    runProgram();
}

void Controller::loadProgram(const std::string &objfilePath)
{
    // 오브젝트 파일 열기
    std::ifstream objFile(objfilePath, std::ios::binary);
    if (!objFile.is_open())
    {
        std::cerr << "Error: Failed to open object file: " << objfilePath << std::endl;
        isSuccess = false;
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
    return;
}

void Controller::runProgram()
{
    stepCnt = 0;
    if (debugMode)
    {
        // 디버그 모드: 스텝 실행 및 상태 출력
        while (!emulator.isHalted())
        {
            stepCnt++;
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
            // std::cout << "Press Enter to continue..." << std::endl;
            // std::cin.get();
            if (stepCnt >= 10000000)
            {
                std::cerr << "Error: Infinite loop(step executed 10,000,000 times)" << std::endl;
                isSuccess = false;
                return;
            }
        }
    }
    else
    {
        // 일반 모드: 전체 실행
        while (!emulator.isHalted())
        {
            stepCnt++;
            emulator.step();

            if (stepCnt >= 10000000)
            {
                std::cerr << "Error: Infinite loop(step executed 10,000,000 times)" << std::endl;
                isSuccess = false;
                return;
            }
        }
        return;
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
}

void Controller::reset()
{
    isSuccess = true;
    stepCnt = 0;
    debugMode = false;
    assembler.reset();
    emulator.reset();
}

void Controller::setRegister(unsigned short n, unsigned short value)
{
    emulator.getRegisterFile().writeRegister(n, value);
}

unsigned short Controller::getRegister(unsigned short n)
{
    return emulator.getRegisterFile().readRegister(n);
}

void Controller::setMemory(unsigned short address, unsigned short value)
{
    emulator.getMemory().write(address, value);
}

unsigned short Controller::getMemory(unsigned short address)
{
    return emulator.getMemory().read(address);
}

unsigned short Controller::getPC()
{
    return emulator.getRegisterFile().readPC();
}

void Controller::setPC(unsigned short address)
{
    emulator.getRegisterFile().writePC(address);
}

void Controller::enableDebugMode()
{
    debugMode = true;
}

void Controller::disableDebugMode()
{
    debugMode = false;
}

void Controller::step()
{
    emulator.step();
}

void Controller::dumpRegisters()
{
    std::cout << "Current Register State:" << std::endl;
    for (int i = 0; i < 8; ++i)
    {
        printf("R%d: x%.4x ", i, getRegister(i));
        if (i % 4 == 3)
            std::cout << std::endl;
    }
}

void Controller::dumpMemory(unsigned short startAddress, unsigned short endAddress)
{
    std::cout << "Current Memory State:" << std::endl;
    for (int i = startAddress; i < endAddress; i++)
    {
        printf("x%.4x: x%.4x ", i, getMemory(i));
        if ((i - startAddress) % 2 == 1)
            std::cout << std::endl;
    }
}
