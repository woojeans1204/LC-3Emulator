// include/Assembler/Parser.h
#pragma once

#include <string>
#include <vector>
#include "Assembler/SymbolTable.h"

namespace Assembler
{

    struct ParsedLine
    {
        std::vector<std::string> labels; // 다중 라벨을 저장
        std::string opcode;
        std::vector<std::string> operands;
    };

    class Parser
    {
    public:
        Parser(const std::string &sourceCode, SymbolTable &symTable);
        bool hasNext() const;
        ParsedLine getNext();

    private:
        std::vector<ParsedLine> lines;
        size_t currentLine;
        SymbolTable &symbolTable;
        void parseSource(const std::string &sourceCode);
    };

} // namespace Assembler
