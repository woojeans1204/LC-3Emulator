// src/Assembler/Assembler.cpp
#include "Assembler/Assembler.h"
#include <iostream>
#include <stdexcept>
#include <sstream>
#include <iomanip>
#include <cctype> // isdigit

namespace Assembler
{

    Assembler::Assembler() : symTable(), instrSet(), parser(symTable)
    {
    }

    void Assembler::assemble(const std::string &inputFilePath, const std::string &outputFilePath)
    {
        // 1차 패스: 라벨 수집
        firstPass(inputFilePath);

        // 2차 패스: 명령어 인코딩 및 출력 파일 생성
        secondPass(outputFilePath);
    }

    void Assembler::firstPass(const std::string &inputFilePath)
    {
        std::ifstream inputFile(inputFilePath);
        if (!inputFile.is_open())
        {
            throw std::runtime_error("Failed to open input file: " + inputFilePath);
        }

        std::string asmcode, line;
        while (std::getline(inputFile, line))
        {
            asmcode += line;
            asmcode += '\n';
        }
        inputFile.close();

        parser.parseSource(asmcode);
    }

    // 항상 firstPass가 실행됨을 전제로 한다
    // symTable, parser의 정보를 가지고 소스코드를 기계어로 번역
    void Assembler::secondPass(const std::string &outputFilePath)
    {
        std::ofstream outputFile(outputFilePath, std::ios::binary);
        if (!outputFile.is_open())
        {
            throw std::runtime_error("Failed to open output file: " + outputFilePath);
        }

        int currentAddress = 0;
        ParsedLine currentParsed;
        parser.initLine();
        bool hasAddress = false;
        unsigned short machineCode;

        while (parser.hasNext())
        {
            currentParsed = parser.getNext();

            // ORIG 처리
            if (currentParsed.opcode == ".ORIG")
            {
                if (hasAddress)
                    throw std::runtime_error(".END should come before another .ORIG");
                if (currentParsed.operands.empty())
                    throw std::invalid_argument(".ORIG requires an operand.");
                std::string operand = currentParsed.operands[0];
                hasAddress = true;
                currentAddress = parseImmediate(operand, 16); // 주소는 16비트
                continue;                                     // .ORIG은 주소 설정만 하고 넘어감
            }

            if (!hasAddress)
                throw std::runtime_error("Cannot find starting address (.ORIG directive is missing).");

            // END 처리
            if (currentParsed.opcode == ".END")
            {
                hasAddress = false;
                continue; // .END는 처리할 내용 없음
            }

            // 주소 출력
            machineCode = static_cast<unsigned short>(currentAddress & 0xFFFF);
            outputFile.write(reinterpret_cast<char *>(&machineCode), sizeof(unsigned short));

            // 라벨 처리 (기계어 코드에는 포함되지 않음)
            if (!currentParsed.labels.empty())
            {
                // 라벨은 이미 firstPass에서 처리되었으므로 별도 작업 불필요
            }

            // 디렉티브 및 명령어 처리
            if (currentParsed.opcode.empty())
            {
                // opcode가 없는 경우 (빈 줄 등)
                continue;
            }
            else if (currentParsed.opcode == ".FILL")
            {
                if (currentParsed.operands.size() != 1)
                {
                    throw std::invalid_argument("'.FILL' requires exactly one operand.");
                }
                std::string operand = currentParsed.operands[0];
                int value;

                if (symTable.contains(operand))
                {
                    value = symTable.getAddress(operand);
                }
                else
                {
                    value = parseImmediate(operand, 16);
                }

                machineCode = static_cast<unsigned short>(value & 0xFFFF);
                outputFile.write(reinterpret_cast<char *>(&machineCode), sizeof(unsigned short));
                currentAddress += 1;
            }
            else if (currentParsed.opcode == ".BLKW")
            {
                if (currentParsed.operands.size() != 1)
                {
                    throw std::invalid_argument("'.BLKW' requires exactly one operand.");
                }
                std::string operand = currentParsed.operands[0];
                int numWords = parseImmediate(operand, 16);

                for (int i = 0; i < numWords; ++i)
                {
                    // 주소와 0으로 초기화된 기계어 코드 출력
                    machineCode = static_cast<unsigned short>((currentAddress + i) & 0xFFFF);
                    outputFile.write(reinterpret_cast<char *>(&machineCode), sizeof(unsigned short));
                    machineCode = 0;
                    outputFile.write(reinterpret_cast<char *>(&machineCode), sizeof(unsigned short));
                }
                currentAddress += numWords;
            }
            else if (currentParsed.opcode == ".STRINGZ")
            {
                if (currentParsed.operands.size() != 1)
                {
                    throw std::invalid_argument("'.STRINGZ' requires exactly one operand.");
                }
                std::string str = currentParsed.operands[0];
                // 문자열에서 양쪽 따옴표 제거
                if (str.front() == '"' && str.back() == '"')
                {
                    str = str.substr(1, str.length() - 2);
                }
                for (char c : str)
                {
                    // 주소와 문자 코드 출력
                    machineCode = static_cast<unsigned short>(currentAddress & 0xFFFF);
                    outputFile.write(reinterpret_cast<char *>(&machineCode), sizeof(unsigned short));
                    machineCode = static_cast<unsigned short>(c);
                    outputFile.write(reinterpret_cast<char *>(&machineCode), sizeof(unsigned short));
                    currentAddress++;
                }
                // Null terminator
                machineCode = static_cast<unsigned short>(currentAddress & 0xFFFF);
                outputFile.write(reinterpret_cast<char *>(&machineCode), sizeof(unsigned short));
                unsigned short nullTerm = 0;
                outputFile.write(reinterpret_cast<char *>(&nullTerm), sizeof(unsigned short));
                currentAddress++;
            }
            else
            {
                // 명령어 인코딩
                std::string opcode = currentParsed.opcode;
                if (opcode == "ADD")
                {
                    if (currentParsed.operands.size() != 3)
                    {
                        throw std::invalid_argument("ADD requires exactly 3 operands.");
                    }
                    std::string dr = currentParsed.operands[0];
                    std::string sr1 = currentParsed.operands[1];
                    std::string operand3 = currentParsed.operands[2];
                    // Check if operand3 is a register or immediate
                    if (operand3[0] == 'R' || operand3[0] == 'r')
                    {
                        std::string sr2 = operand3;
                        machineCode = instrSet.encodeAddRegister(dr, sr1, sr2);
                    }
                    else
                    {
                        int imm5 = parseImmediate(operand3, 5);
                        machineCode = instrSet.encodeAddImmediate(dr, sr1, imm5);
                    }
                }
                else if (opcode == "AND")
                {
                    if (currentParsed.operands.size() != 3)
                    {
                        throw std::invalid_argument("AND requires exactly 3 operands.");
                    }
                    std::string dr = currentParsed.operands[0];
                    std::string sr1 = currentParsed.operands[1];
                    std::string operand3 = currentParsed.operands[2];
                    if (operand3[0] == 'R' || operand3[0] == 'r')
                    {
                        std::string sr2 = operand3;
                        machineCode = instrSet.encodeAndRegister(dr, sr1, sr2);
                    }
                    else
                    {
                        int imm5 = parseImmediate(operand3, 5);
                        machineCode = instrSet.encodeAndImmediate(dr, sr1, imm5);
                    }
                }
                else if (opcode == "NOT")
                {
                    if (currentParsed.operands.size() != 2)
                    {
                        throw std::invalid_argument("NOT requires exactly 2 operands.");
                    }
                    std::string dr = currentParsed.operands[0];
                    std::string sr = currentParsed.operands[1];
                    machineCode = instrSet.encodeNot(dr, sr);
                }
                else if (opcode == "RET")
                {
                    machineCode = instrSet.encodeRet();
                }
                else if (opcode == "HALT")
                {
                    machineCode = instrSet.encodeHalt();
                }
                else
                {
                    throw std::invalid_argument("Unsupported opcode: " + opcode);
                }

                // 기계어 코드 출력
                outputFile.write(reinterpret_cast<char *>(&machineCode), sizeof(unsigned short));
                currentAddress += 1;
            }
        }

        outputFile.close();
    }

    int Assembler::parseImmediate(const std::string &operand, int bitCount)
    {
        int value;
        std::string numStr = operand;

        // 접두사 처리
        if (operand[0] == '#')
        {
            numStr = operand.substr(1);
            value = std::stoi(numStr, nullptr, 10);
        }
        else if (operand[0] == 'x' || operand[0] == 'X')
        {
            numStr = operand.substr(1);
            value = std::stoi(numStr, nullptr, 16);
        }
        else if (operand[0] == 'b' || operand[0] == 'B')
        {
            numStr = operand.substr(1);
            value = std::stoi(numStr, nullptr, 2);
        }
        else if (operand[0] == '-' || isdigit(operand[0]))
        {
            value = std::stoi(numStr, nullptr, 10);
        }
        else
        {
            throw std::invalid_argument("Invalid immediate value: " + operand);
        }

        // 범위 체크
        int minValue = -(1 << (bitCount - 1));
        int maxValue = (1 << (bitCount - 1)) - 1;
        if (value < minValue || value > maxValue)
        {
            std::ostringstream oss;
            oss << "Immediate value out of range (" << minValue << " ~ " << maxValue << "): " << value;
            throw std::invalid_argument(oss.str());
        }

        return value;
    }

    int Assembler::computePCOffset(int currentAddress, const std::string &label, int bitCount)
    {
        if (!symTable.contains(label))
        {
            throw std::invalid_argument("Undefined label: " + label);
        }
        int labelAddress = symTable.getAddress(label);
        int offset = labelAddress - (currentAddress + 1); // PC-relative offset
        int minOffset = -(1 << (bitCount - 1));
        int maxOffset = (1 << (bitCount - 1)) - 1;
        if (offset < minOffset || offset > maxOffset)
        {
            std::ostringstream oss;
            oss << "PCOffset" << bitCount << " out of range (" << minOffset << " ~ " << maxOffset << "): " << offset;
            throw std::invalid_argument(oss.str());
        }
        return offset;
    }

} // namespace Assembler
