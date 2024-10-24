// src/Assembler/Parser.cpp
#include "Assembler/Parser.h"
#include "Assembler/keyword.h" // 명령어, 지시어, 레지스터 목록 포함

#include <stdexcept>

namespace Assembler
{

    // 공백 문자인지 확인하는 함수
    bool isSpace(char ch)
    {
        return ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r';
    }

    // 문자열의 좌우 공백을 제거하는 함수
    std::string trim(const std::string &str)
    {
        size_t start = 0;
        while (start < str.size() && isSpace(str[start]))
        {
            ++start;
        }
        size_t end = str.size();
        while (end > start && isSpace(str[end - 1]))
        {
            --end;
        }
        return str.substr(start, end - start);
    }

    // 문자열을 공백이나 쉼표로 분리하는 함수
    std::vector<std::string> split(const std::string &str)
    {
        std::vector<std::string> tokens;
        std::string token;
        for (size_t i = 0; i <= str.size(); ++i)
        {
            if (i == str.size() || isSpace(str[i]) || str[i] == ',')
            {
                if (!token.empty())
                {
                    tokens.push_back(token);
                    token.clear();
                }
            }
            else
            {
                token += str[i];
            }
        }
        return tokens;
    }

    // 문자열을 대문자로 변환하는 함수
    std::string toUpper(const std::string &str)
    {
        std::string result = str;
        for (auto &ch : result)
        {
            if ('a' <= ch && ch <= 'z')
            {
                ch = ch - 'a' + 'A';
            }
        }
        return result;
    }

    Parser::Parser(const std::string &sourceCode) : currentLine(0)
    {
        parseSource(sourceCode);
    }

    bool Parser::hasNext() const
    {
        return currentLine < lines.size();
    }

    ParsedLine Parser::getNext()
    {
        if (!hasNext())
        {
            throw std::out_of_range("No more lines to parse.");
        }
        return lines[currentLine++];
    }

    void Parser::parseSource(const std::string &sourceCode)
    {
        size_t pos = 0;
        while (pos < sourceCode.size())
        {
            // 한 줄 읽기
            size_t endPos = sourceCode.find('\n', pos);
            if (endPos == std::string::npos)
            {
                endPos = sourceCode.size();
            }
            std::string line = sourceCode.substr(pos, endPos - pos);
            pos = endPos + 1;

            // 주석 제거
            size_t commentPos = line.find(';');
            if (commentPos != std::string::npos)
            {
                line = line.substr(0, commentPos);
            }

            // 좌우 공백 제거
            line = trim(line);

            if (line.empty())
            {
                continue; // 빈 줄이면 다음으로 넘어감
            }

            // 토큰 분리
            std::vector<std::string> tokens = split(line);

            if (tokens.empty())
            {
                continue; // 토큰이 없으면 다음으로 넘어감
            }

            ParsedLine parsed;
            size_t index = 0;

            // 첫 번째 토큰을 대문자로 변환하여 검사
            std::string firstTokenUpper = toUpper(tokens[0]);

            // 첫 번째 토큰이 명령어나 지시어, 레지스터에 포함되지 않으면 라벨로 간주
            if (INSTRUCTIONS.find(firstTokenUpper) == INSTRUCTIONS.end() &&
                DIRECTIVES.find(firstTokenUpper) == DIRECTIVES.end() &&
                REGISTERS.find(firstTokenUpper) == REGISTERS.end())
            {
                parsed.label = tokens[index++];
            }

            // 명령어 또는 지시어 처리
            if (index < tokens.size())
            {
                std::string opcodeUpper = toUpper(tokens[index]);
                if (INSTRUCTIONS.find(opcodeUpper) != INSTRUCTIONS.end() ||
                    DIRECTIVES.find(opcodeUpper) != DIRECTIVES.end())
                {
                    parsed.opcode = opcodeUpper;
                    index++;
                }
                else
                {
                    // 유효하지 않은 명령어/지시어
                    throw std::invalid_argument("Invalid opcode: " + tokens[index]);
                }
            }

            // 오퍼랜드 처리
            while (index < tokens.size())
            {
                parsed.operands.push_back(tokens[index++]);
            }

            lines.push_back(parsed);
        }
    }

} // namespace Assembler
