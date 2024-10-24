// src/main.cpp
#include <iostream>
#include "Assembler/Parser.h"

int main()
{
    std::string sourceCode = R"(; Fibonacci function
fib
    ADD R6, R6, #-4
    STR R3, R6, #1
    STR R4, R6, #2
    STR R7, R6, #3

    ADD R0, R0, #0
    BRz ed
    ADD R3, R0, #-1
    BRz ed
    ADD R3, R0, #0 ; R3 = n

    ADD R0, R0, #-1 ; R0 = n - 1
    JSR fib
    ADD R4, R0, #0 ; R4 = fib(n-1)
    ADD R0, R3, #-2 ; R0 = n - 2
    JSR fib
    ADD R0, R0, R4 ; R0 = fib(n-1) + fib(n-2)

ed
    LDR R3, R6, #1
    LDR R4, R6, #2
    LDR R7, R6, #3
    ADD R6, R6, #4
    RET
)";

    Assembler::Parser parser(sourceCode);

    while (parser.hasNext())
    {
        Assembler::ParsedLine line = parser.getNext();
        std::cout << "Label: " << line.label << std::endl;
        std::cout << "Opcode: " << line.opcode << std::endl;
        std::cout << "Operands: ";
        for (const auto &operand : line.operands)
        {
            std::cout << operand << " ";
        }
        std::cout << "\n---\n";
    }

    return 0;
}
