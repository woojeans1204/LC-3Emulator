// include/Assembler/Encoder.h
#pragma once

#include <string>
#include <unordered_map>
#include <stdexcept>

namespace Assembler
{
    class InstructionSet
    {
    public:
        InstructionSet() = default;
        ~InstructionSet() = default;

        unsigned short encodeAddRegister(const std::string &dr, const std::string &sr1, const std::string &sr2);

        unsigned short encodeAddImmediate(const std::string &dr, const std::string &sr1, int imm5);

        unsigned short encodeAndRegister(const std::string &dr, const std::string &sr1, const std::string &sr2);

        unsigned short encodeAndImmediate(const std::string &dr, const std::string &sr1, int imm5);

        unsigned short encodeBr(bool n, bool z, bool p, int pcOffset9);

        unsigned short encodeNot(const std::string &dr, const std::string &sr);

        unsigned short encodeRet();

        // 추가 명령어 인코딩 함수들 (JMP, JSR, LD, ST, NOT, RET 등)
        // ...

    private:
        int getRegisterNumber(const std::string &reg) const;
    };

} // namespace Assembler