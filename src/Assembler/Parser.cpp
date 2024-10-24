// src/Assembler/Parser.cpp
#include "Assembler/Parser.h"
#include "Assembler/Keyword.h" // 명령어, 지시어, 레지스터 목록 포함

#include <iostream>
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

    Parser::Parser(const std::string &sourceCode, SymbolTable &symTable)
        : currentLine(0), symbolTable(symTable)
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
        std::string pendingLabel = "";
        int currentAddress = 0x3000; // LC-3 메모리 주소 초기화 (예: .ORIG에서 설정될 수 있음)

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
            // 토큰 분해 준비
            ParsedLine parsed;
            size_t index = 0;
            std::vector<std::string> labels;

            // 여러 개의 라벨을 수집
            while (index < tokens.size())
            {
                std::string tokenUpper = toUpper(tokens[index]);
                if (INSTRUCTIONS.find(tokenUpper) == INSTRUCTIONS.end() &&
                    DIRECTIVES.find(tokenUpper) == DIRECTIVES.end() &&
                    REGISTERS.find(tokenUpper) == REGISTERS.end())
                {
                    labels.push_back(tokens[index]);
                    index++;
                }
                else
                {
                    break; // 명령어나 지시어, 레지스터를 만나면 라벨 수집 종료
                }
            }
            // 수집된 라벨을 심볼 테이블에 추가
            for (const auto &lbl : labels)
            {
                if (symbolTable.contains(lbl))
                {
                    throw std::invalid_argument("Duplicate label found: " + lbl);
                }
                symbolTable.addLabel(lbl, currentAddress);
                parsed.labels.push_back(lbl); // 다중 라벨을 벡터에 추가
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

            // 1차 패스: 라벨을 심볼 테이블에 추가 하기 위해 주소를 계산
            // 현재 주소를 증가시킴 (명령어 또는 지시어에 따라 달라질 수 있음)
            // 예를 들어, 명령어는 1 워드, .FILL도 1 워드, .BLKW는 워드 수에 따라 다름
            // 전체 구현에서는 지시어에 따라 주소 계산을 다르게 해야 합니다.

            if (!parsed.opcode.empty())
            {
                if (parsed.opcode == ".FILL" || parsed.opcode == ".ORIG" || parsed.opcode == ".END")
                {
                    // .FILL, .ORIG, .END 등의 지시어는 주소를 증가시키지 않거나 특별히 처리해야 함
                    if (parsed.opcode == ".ORIG")
                    {
                        if (parsed.operands.empty())
                        {
                            throw std::invalid_argument(".ORIG requires an operand");
                        }
                        // .ORIG의 첫 번째 오퍼랜드를 시작 주소로 설정
                        try
                        {
                            currentAddress = std::stoi(parsed.operands[0], nullptr, 16);
                        }
                        catch (...)
                        {
                            throw std::invalid_argument("Invalid operand for .ORIG: " + parsed.operands[0]);
                        }
                    }
                    else if (parsed.opcode == ".END")
                    {
                        // .END는 어셈블링을 종료
                        break;
                    }
                    // .FILL, .BLKW 등 다른 지시어는 추가 구현 필요
                }
                else
                {
                    // 명령어는 1 워드로 가정
                    currentAddress += 1;
                }
            }

            lines.push_back(parsed);
        }

        // 파일 끝에 pending된 라벨이 있다면, 이를 처리할 수 있습니다.
        if (!pendingLabel.empty())
        {
            // 현재는 무시하거나 에러를 던질 수 있습니다.
            // 여기서는 무시하도록 합니다.
        }
    }

} // namespace Assembler
