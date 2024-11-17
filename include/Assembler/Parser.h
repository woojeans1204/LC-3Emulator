// include/Assembler/Parser.h
#pragma once

#include <string>
#include <vector>
#include "SymbolTable.h"

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
        Parser(SymbolTable &symTable);
        void reset();
        bool hasNext() const;
        ParsedLine getNext();

        void parseSource(const std::string &sourceCode);

        void initLine();
        ParsedLine searchLine(size_t t);
        void printLines();

    private:
        std::vector<ParsedLine> lines;
        size_t currentLine;
        SymbolTable &symbolTable;
    };

} // namespace Assembler
