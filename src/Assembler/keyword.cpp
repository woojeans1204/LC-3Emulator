// src/Assembler/Keyword.cpp
#include "Assembler/Keyword.h"

namespace Assembler
{

    // 명령어 목록 정의
    const std::unordered_set<std::string> INSTRUCTIONS = {
        "ADD", "AND", "BR", "BRN", "BRZ", "BRP", "BRNZ", "BRNP", "BRZP", "BRNZP",
        "JMP", "JSR", "JSRR", "LD", "LDI", "LDR", "LEA",
        "NOT", "RET", "RTI", "ST", "STI", "STR", "TRAP",
        "GETC", "OUT", "PUTS", "IN", "PUTSP", "HALT"};

    // 지시어 목록 정의
    const std::unordered_set<std::string> DIRECTIVES = {
        ".ORIG", ".END", ".BLKW", ".FILL", ".STRINGZ"};

    // 레지스터 목록 정의
    const std::unordered_set<std::string> REGISTERS = {
        "R0", "R1", "R2", "R3", "R4", "R5", "R6", "R7"};

} // namespace Assembler
