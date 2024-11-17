// include/Assembler/Assembler.h
#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include "Parser.h"
#include "SymbolTable.h"
#include "Encoder.h"

namespace Assembler
{

    class Assembler
    {
    public:
        Assembler();
        ~Assembler() = default;

        void assemble(const std::string &inputFilePath, const std::string &outputFilePath);
        void firstPass(const std::string &inputFilePath);
        void secondPass(const std::string &outputFilePath);
        void reset();

    private:
        Parser parser;
        SymbolTable symTable;
        InstructionSet instrSet;

        int parseImmediate(const std::string &operand, int bitCount, bool isSigned = true);
        int computePCOffset(int currentAddress, const std::string &label, int bitCount);
    };

} // namespace Assembler