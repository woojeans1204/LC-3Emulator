// include/Controller.h
#pragma once

#include <string>
#include "Assembler/Assembler.h"
#include "Simulator/LC3Emulator.h"

class Controller
{
public:
    Controller();
    ~Controller() = default;
    void assemble(const std::string &sourceFilePath, const std::string &objfilePath);

    void assembleRun(const std::string &sourceFilePath, const std::string &objFilePath);

    void loadProgram(const std::string &objFilePath);

    void runProgram();

    void reset();

    void setRegister(unsigned short n, unsigned short value);

    unsigned short getRegister(unsigned short n);

    void setMemory(unsigned short address, unsigned short value);

    unsigned short getMemory(unsigned short address);

    void setPC(unsigned short address);

    unsigned short getPC();

    void enableDebugMode();
    void disableDebugMode();

    void step();

    void dumpRegisters();
    void dumpMemory(unsigned short startAddress, unsigned short endAddress);
    bool checkSuccess()
    {
        return isSuccess;
    }

private:
    Assembler::Assembler assembler;
    Simulator::LC3Emulator emulator;
    bool isSuccess;
    bool debugMode;
    int stepCnt;
};