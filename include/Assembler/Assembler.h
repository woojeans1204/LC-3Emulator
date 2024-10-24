// include/Assembler/Assembler.h
#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include "Parser.h"
#include "SymbolTable.h"
#include "InstructionSet.h"

namespace Assembler
{

    class Assembler
    {
    public:
        Assembler();
        ~Assembler() = default;

        void assemble(const std::string &inputFilePath, const std::string &outputFilePath);
        void firstPass(const std::string &inputFilePath);

    private:
        Parser parser;
        SymbolTable symTable;
        InstructionSet instrSet;

        void secondPass(const std::string &inputFilePath, const std::string &outputFilePath);
    };

} // namespace Assembler