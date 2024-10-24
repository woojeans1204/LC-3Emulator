// include/Assembler/Parser.h
#pragma once

#include <string>
#include <vector>

namespace Assembler
{

    struct ParsedLine
    {
        std::string label;
        std::string opcode;
        std::vector<std::string> operands;
    };

    class Parser
    {
    public:
        Parser(const std::string &sourceCode);
        bool hasNext() const;
        ParsedLine getNext();

    private:
        std::vector<ParsedLine> lines;
        size_t currentLine;
        void parseSource(const std::string &sourceCode);
    };

} // namespace Assembler
