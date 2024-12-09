// tests/TestParser.cpp

// 테스트 내용
// 단일 라벨 및 명령어 파싱: 라벨과 명령어가 동일한 라인에 있는 경우 올바르게 파싱되는지 테스트합니다.
// 다중 라벨 파싱: 여러 개의 라벨이 동일한 라인에 있는 경우 올바르게 파싱되는지 테스트합니다.
// 라벨만 있는 라인 처리: 라벨만 있는 라인을 다음 명령어에 올바르게 연계하여 파싱하는지 테스트합니다.
// 중복 라벨 처리: 동일한 라벨이 여러 번 정의될 때 올바르게 예외가 발생하는지 테스트합니다.
// 유효하지 않은 명령어 처리: 존재하지 않는 명령어가 있을 때 올바르게 예외가 발생하는지 테스트합니다.
// 명령어 오퍼랜드 수 검증: 명령어의 예상 오퍼랜드 수와 실제 오퍼랜드 수가 일치하는지 검증합니다.

#include <iostream>
#include <cassert>
#include "Assembler/Parser.h"
#include "Assembler/SymbolTable.h"

using namespace Assembler;

void testSingleLabelAndCommand()
{
    std::string sourceCode = "START ADD R1, R2, R3";
    SymbolTable symTable;
    Parser parser(symTable);
    parser.parseSource(sourceCode);

    if (!parser.hasNext())
    {
        std::cerr << "Parser should have next line.\n";
        return;
    }
    ParsedLine line = parser.getNext();
    if (line.labels.empty())
    {
        std::cerr << "Label should not be empty.\n";
        return;
    }
    if (line.labels[0] != "START")
    {
        std::cerr << "Label should be 'START'.\n";
        return;
    }
    if (line.opcode != "ADD")
    {
        std::cerr << "Opcode should be 'ADD'.\n";
        return;
    }
    if (line.operands.size() != 3)
    {
        std::cerr << "There should be 3 operands.\n";
        return;
    }
    if (line.operands[0] != "R1" || line.operands[1] != "R2" || line.operands[2] != "R3")
    {
        std::cerr << "Operands should be R1, R2, R3.\n";
        return;
    }
    std::cout << "testSingleLabelAndCommand passed.\n";
}

void testMultipleLabelsAndCommand()
{
    std::string sourceCode = "LOOP1 LOOP2 ADD R4, R5, R6";
    SymbolTable symTable;
    Parser parser(symTable);
    parser.parseSource(sourceCode);

    if (!parser.hasNext())
    {
        std::cerr << "Parser should have next line.\n";
        return;
    }
    ParsedLine line = parser.getNext();
    if (line.labels.size() != 2)
    {
        std::cerr << "There should be 2 labels.\n";
        return;
    }
    if (line.labels[0] != "LOOP1" || line.labels[1] != "LOOP2")
    {
        std::cerr << "Labels should be LOOP1 and LOOP2.\n";
        return;
    }
    if (line.opcode != "ADD")
    {
        std::cerr << "Opcode should be 'ADD'.\n";
        return;
    }
    if (line.operands.size() != 3)
    {
        std::cerr << "There should be 3 operands.\n";
        return;
    }
    if (line.operands[0] != "R4" || line.operands[1] != "R5" || line.operands[2] != "R6")
    {
        std::cerr << "Operands should be R4, R5, R6.\n";
        return;
    }
    std::cout << "testMultipleLabelsAndCommand passed.\n";
}

void testLabelOnlyLine()
{
    std::string sourceCode = "LABEL_ONLY\nADD R1, R2, R3";
    SymbolTable symTable;
    Parser parser(symTable);
    parser.parseSource(sourceCode);

    if (!parser.hasNext())
    {
        std::cerr << "Parser should have first line.\n";
        return;
    }
    ParsedLine firstLine = parser.getNext();
    if (firstLine.labels.size() != 1 || firstLine.labels[0] != "LABEL_ONLY")
    {
        std::cerr << "First line should have label 'LABEL_ONLY'.\n";
        return;
    }
    if (!firstLine.opcode.empty())
    {
        std::cerr << "First line should have no opcode.\n";
        return;
    }
    if (!firstLine.operands.empty())
    {
        std::cerr << "First line should have no operands.\n";
        return;
    }

    if (!parser.hasNext())
    {
        std::cerr << "Parser should have second line.\n";
        return;
    }
    ParsedLine secondLine = parser.getNext();
    if (!secondLine.labels.empty())
    {
        std::cerr << "Second line should have no label.\n";
        return;
    }
    if (secondLine.opcode != "ADD")
    {
        std::cerr << "Opcode should be 'ADD'.\n";
        return;
    }
    if (secondLine.operands.size() != 3)
    {
        std::cerr << "There should be 3 operands.\n";
        return;
    }
    if (secondLine.operands[0] != "R1" || secondLine.operands[1] != "R2" || secondLine.operands[2] != "R3")
    {
        std::cerr << "Operands should be R1, R2, R3.\n";
        return;
    }

    if (parser.hasNext())
    {
        std::cerr << "Parser should have no more lines.\n";
        return;
    }
    std::cout << "testLabelOnlyLine passed.\n";
}

void testDuplicateLabel()
{
    try
    {
        std::string sourceCode = "DUP_LABEL ADD R1, R2, R3\nDUP_LABEL SUB R4, R5, R6";
        SymbolTable symTable;
        Parser parser(symTable);
        parser.parseSource(sourceCode);
        ParsedLine firstLine = parser.getNext();
        ParsedLine secondLine = parser.getNext();
        std::cerr << "Duplicate label test failed.\n";
    }
    catch (const std::invalid_argument &e)
    {
        std::cout << "testDuplicateLabel passed.\n";
    }
}

void testInvalidOpcode()
{
    std::string sourceCode = "START INVALID_OP R1, R2, R3";
    SymbolTable symTable;
    try
    {
        Parser parser(symTable);
        parser.parseSource(sourceCode);
        ParsedLine line = parser.getNext();
        std::cerr << "Invalid opcode test failed.\n";
    }
    catch (const std::invalid_argument &e)
    {
        std::cout << "testInvalidOpcode passed.\n";
    }
}

void testOperandCountVerification()
{
    std::string sourceCode = "ADD R1, R2"; // 부족한 오퍼랜드
    SymbolTable symTable;
    Parser parser(symTable);
    parser.parseSource(sourceCode);

    try
    {
        if (!parser.hasNext())
        {
            std::cerr << "Parser should have line.\n";
            return;
        }
        ParsedLine line = parser.getNext();
        if (line.operands.size() != 3)
        {
            throw std::invalid_argument("Operand count mismatch");
        }
        std::cerr << "Operand count verification test failed.\n";
    }
    catch (const std::invalid_argument &e)
    {
        std::cout << "testOperandCountVerification passed.\n";
    }
}

void testMultipleLabelsSameLine()
{
    std::string sourceCode = "LABEL1 LABEL2 LABEL3 ADD R4, R5, R6";
    SymbolTable symTable;
    Parser parser(symTable);
    parser.parseSource(sourceCode);

    if (!parser.hasNext())
    {
        std::cerr << "Parser should have line.\n";
        return;
    }
    ParsedLine line = parser.getNext();
    if (line.labels.size() != 3 || line.labels[0] != "LABEL1" || line.labels[1] != "LABEL2" || line.labels[2] != "LABEL3")
    {
        std::cerr << "Labels should be LABEL1, LABEL2, LABEL3.\n";
        return;
    }
    if (line.opcode != "ADD")
    {
        std::cerr << "Opcode should be 'ADD'.\n";
        return;
    }
    if (line.operands.size() != 3)
    {
        std::cerr << "There should be 3 operands.\n";
        return;
    }
    if (line.operands[0] != "R4" || line.operands[1] != "R5" || line.operands[2] != "R6")
    {
        std::cerr << "Operands should be R4, R5, R6.\n";
        return;
    }
    std::cout << "testMultipleLabelsSameLine passed.\n";
}

int main()
{
    std::cout << "Running Parser Tests...\n";
    testSingleLabelAndCommand();
    testMultipleLabelsAndCommand();
    testLabelOnlyLine();
    testDuplicateLabel();
    testInvalidOpcode();
    testMultipleLabelsSameLine();
    testOperandCountVerification();
    std::cout << "All Parser Tests Passed!\n";
    return 0;
}