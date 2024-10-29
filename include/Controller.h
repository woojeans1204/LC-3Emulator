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
    void assembleAndRun(const std::string &sourceFilePath, const std::string &objfilePath, bool debugMode = false);

private:
    Assembler::Assembler assembler;
    Simulator::LC3Emulator emulator;

    void loadObjectCodeToEmulator(const std::string &objfilePath);

    void runEmulator(bool debugMode);
};