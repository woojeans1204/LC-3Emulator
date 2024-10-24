// include/Assembler/Keyword.h
#pragma once

#include <unordered_set>
#include <string>

namespace Assembler
{

    // 명령어 목록
    extern const std::unordered_set<std::string> INSTRUCTIONS;

    // 지시어 목록
    extern const std::unordered_set<std::string> DIRECTIVES;

    // 레지스터 목록
    extern const std::unordered_set<std::string> REGISTERS;

} // namespace Assembler
