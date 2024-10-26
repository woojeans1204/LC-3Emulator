// src/Assembler/Assembler.cpp
#include "Assembler/Assembler.h"
#include <iostream>
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
        size_t currentLine = 0;
        parser.initLine();
        bool hasAddress = false;

        while (parser.hasNext())
        {
            currentParsed = parser.getNext();
            if (!currentParsed.opcode.empty())
            {
                if (!hasAddress && currentParsed.opcode != ".ORIG")
                    throw std::runtime_error("cannot found address");
                if (currentParsed.opcode == ".ORIG")
                {
                    if (hasAddress)
                        throw std::runtime_error(".END should come first");
                    std::string operand = currentParsed.operands[0];
                    hasAddress = true;
                    if (operand[0] == 'x' || operand[0] == 'X')
                        currentAddress = std::stoi(operand.substr(1), nullptr, 16);
                    else
                        currentAddress = std::stoi(operand);
                    unsigned short machineCode = static_cast<unsigned short>(currentAddress & 0xFFFF);
                    outputFile.write(reinterpret_cast<char *>(&machineCode), sizeof(unsigned short));
                }
                else if (currentParsed.opcode == ".END")
                {
                    hasAddress = false;
                }
                else if (currentParsed.opcode == ".FILL")
                {
                    if (currentParsed.operands.size() != 1)
                    {
                        throw std::invalid_argument("'.FILL' requires exactly one operand.");
                    }
                    std::string operand = currentParsed.operands[0];
                    int value;
                    if (operand[0] == 'x' || operand[0] == 'X')
                        value = std::stoi(operand.substr(1), nullptr, 16);
                    else
                        value = std::stoi(operand);

                    unsigned short machineCode = static_cast<unsigned short>(value & 0xFFFF);
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
                    int numWords;
                    if (operand[0] == 'x' || operand[0] == 'X')
                        numWords = std::stoi(operand.substr(1), nullptr, 16);
                    else
                        numWords = std::stoi(operand);

                    for (int i = 0; i < numWords; ++i)
                    {
                        unsigned short machineCode = 0;
                        outputFile.write(reinterpret_cast<char *>(&machineCode), sizeof(unsigned short));
                        currentAddress++;
                    }
                }
                else if (currentParsed.opcode == ".STRINGZ")
                {
                    if (currentParsed.operands.size() != 1)
                    {
                        throw std::invalid_argument("'.STRINGZ' requires exactly one operand.");
                    }
                    std::string str = currentParsed.operands[0];
                    for (char c : str)
                    {
                        unsigned short machineCode = static_cast<unsigned short>(c);
                        outputFile.write(reinterpret_cast<char *>(&machineCode), sizeof(unsigned short));
                        currentAddress++;
                    }
                    // Null terminator
                    unsigned short nullTerm = 0;
                    outputFile.write(reinterpret_cast<char *>(&nullTerm), sizeof(unsigned short));
                    currentAddress++;
                }
                else
                {
                    // 명령어 인코딩
                    unsigned short machineCode;

                    // Instruction encoding
                    // 예를 들어, ADD R1, R2, R3
                    // 이 부분은 Opcode에 따라 다르게 처리
                    // 간단히 명령어 이름으로 구분
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
                        else if (operand3[0] == '#')
                        {
                            int imm5 = std::stoi(operand3.substr(1));
                            machineCode = instrSet.encodeAddImmediate(dr, sr1, imm5);
                        }
                        else
                        {
                            throw std::invalid_argument("Invalid operand for ADD: " + operand3);
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
                        else if (operand3[0] == '#')
                        {
                            int imm5 = std::stoi(operand3.substr(1));
                            machineCode = instrSet.encodeAndImmediate(dr, sr1, imm5);
                        }
                        else
                        {
                            throw std::invalid_argument("Invalid operand for AND: " + operand3);
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
                    else
                    {
                        throw std::invalid_argument("Unsupported opcode: " + opcode);
                    }
                    // 기계어 코드 출력
                    outputFile.write(reinterpret_cast<char *>(&machineCode), sizeof(unsigned short));
                    currentAddress++;
                }
            }
        }

        outputFile.close();
    }

} // namespace Assembler