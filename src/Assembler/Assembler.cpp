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
        secondPass(inputFilePath, outputFilePath);
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
    void Assembler::secondPass(const std::string &inputFilePath, const std::string &outputFilePath)
    {
        std::ifstream inputFile(inputFilePath);
        if (!inputFile.is_open())
        {
            throw std::runtime_error("Failed to open input file: " + inputFilePath);
        }
        std::string line;
        int currentAddress = 0;

        std::ofstream outputFile(outputFilePath, std::ios::binary);
        if (!outputFile.is_open())
        {
            throw std::runtime_error("Failed to open output file: " + outputFilePath);
        }

        ParsedLine currentParsedLine;
        size_t currentLine = 0;
        parser.initLine();
        while (parser.hasNext())
        {
            currentParsedLine = parser.getNext();
            if (!parsed.opcode.empty())
            {
                if (parsed.opcode == ".ORIG")
                {
                    std::string operand = parsed.operands[0];
                    int origAddress = std::stoi(operand.substr(1), nullptr, 16);
                    currentAddress = origAddress;
                }
                else if (parsed.opcode == ".END")
                {
                    break; // 어셈블링 종료
                }
                else if (parsed.opcode == ".FILL")
                {
                    if (parsed.operands.size() != 1)
                    {
                        throw std::invalid_argument("'.FILL' requires exactly one operand.");
                    }
                    std::string operand = parsed.operands[0];
                    int value;
                    if (operand.size() >= 2 && (operand[0] == 'x' || operand[0] == 'X'))
                    {
                        value = std::stoi(operand.substr(1), nullptr, 16);
                    }
                    else
                    {
                        value = std::stoi(operand, nullptr, 10);
                    }
                    unsigned short machineCode = static_cast<unsigned short>(value & 0xFFFF);
                    outputFile.write(reinterpret_cast<char *>(&machineCode), sizeof(unsigned short));
                    currentAddress += 1;
                }
                else if (parsed.opcode == ".BLKW")
                {
                    if (parsed.operands.size() != 1)
                    {
                        throw std::invalid_argument("'.BLKW' requires exactly one operand.");
                    }
                    int numWords = std::stoi(parsed.operands[0]);
                    for (int i = 0; i < numWords; ++i)
                    {
                        unsigned short machineCode = 0;
                        outputFile.write(reinterpret_cast<char *>(&machineCode), sizeof(unsigned short));
                    }
                    currentAddress += numWords;
                }
                else if (parsed.opcode == ".STRINGZ")
                {
                    if (parsed.operands.size() != 1)
                    {
                        throw std::invalid_argument("'.STRINGZ' requires exactly one operand.");
                    }
                    std::string str = parsed.operands[0];
                    for (char c : str)
                    {
                        unsigned short machineCode = static_cast<unsigned short>(c);
                        outputFile.write(reinterpret_cast<char *>(&machineCode), sizeof(unsigned short));
                    }
                    // Null terminator
                    unsigned short nullTerm = 0;
                    outputFile.write(reinterpret_cast<char *>(&nullTerm), sizeof(unsigned short));
                    currentAddress += str.size() + 1;
                }
                else
                {
                    // 명령어 인코딩
                    unsigned short machineCode = 0;
                    if (parsed.opcode == "HALT")
                    {
                        // HALT은 TRAP x25로 인코딩
                        // TRAP x25 = 1111 0000 0010 0101 = 0xF025
                        machineCode = 0xF025;
                    }
                    else
                    {
                        // Instruction encoding
                        // 예를 들어, ADD R1, R2, R3
                        // 이 부분은 Opcode에 따라 다르게 처리
                        // 간단히 명령어 이름으로 구분
                        std::string opcode = parsed.opcode;
                        if (opcode == "ADD")
                        {
                            if (parsed.operands.size() != 3)
                            {
                                throw std::invalid_argument("ADD requires exactly 3 operands.");
                            }
                            std::string dr = parsed.operands[0];
                            std::string sr1 = parsed.operands[1];
                            std::string operand3 = parsed.operands[2];
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
                            if (parsed.operands.size() != 3)
                            {
                                throw std::invalid_argument("AND requires exactly 3 operands.");
                            }
                            std::string dr = parsed.operands[0];
                            std::string sr1 = parsed.operands[1];
                            std::string operand3 = parsed.operands[2];
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
                        else if (opcode == "BR")
                        {
                            if (parsed.operands.size() != 4)
                            {
                                throw std::invalid_argument("BR requires 4 operands (n z p offset).");
                            }
                            bool n = (parsed.operands[0] == "n" || parsed.operands[0] == "N");
                            bool z = (parsed.operands[1] == "z" || parsed.operands[1] == "Z");
                            bool p = (parsed.operands[2] == "p" || parsed.operands[2] == "P");
                            std::string offsetStr = parsed.operands[3];
                            int pcOffset9;
                            if (offsetStr.find("x") == 0 || offsetStr.find("X") == 0)
                            {
                                pcOffset9 = std::stoi(offsetStr.substr(1), nullptr, 16);
                            }
                            else
                            {
                                pcOffset9 = std::stoi(offsetStr, nullptr, 10);
                            }
                            machineCode = instrSet.encodeBr(n, z, p, pcOffset9);
                        }
                        else if (opcode == "NOT")
                        {
                            if (parsed.operands.size() != 2)
                            {
                                throw std::invalid_argument("NOT requires exactly 2 operands.");
                            }
                            std::string dr = parsed.operands[0];
                            std::string sr = parsed.operands[1];
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
                    }

                    // 기계어 코드 출력
                    outputFile.write(reinterpret_cast<char *>(&machineCode), sizeof(unsigned short));
                    currentAddress += 1;
                }
            }
        }

        inputFile.close();
        outputFile.close();
    }

} // namespace Assembler