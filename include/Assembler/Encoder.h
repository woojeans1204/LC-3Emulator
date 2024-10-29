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

        unsigned short encodeHalt();

        unsigned short InstructionSet::encodeJmp(const std::string &baseR);

        unsigned short InstructionSet::encodeLd(const std::string &dr, int pcOffset9);

        unsigned short InstructionSet::encodeLdi(const std::string &dr, int pcOffset9);

        unsigned short InstructionSet::encodeLdr(const std::string &dr, const std::string &baseR, int offset6);

        unsigned short InstructionSet::encodeLea(const std::string &dr, int pcOffset9);

        unsigned short InstructionSet::encodeSt(const std::string &sr, int pcOffset9);

        unsigned short InstructionSet::encodeSti(const std::string &sr, int pcOffset9);

        unsigned short InstructionSet::encodeStr(const std::string &sr, const std::string &baseR, int offset6);

        unsigned short InstructionSet::encodeTrap(unsigned char trapvect8);

        // 추가 명령어 인코딩 함수들 (JMP, JSR, LD, ST, NOT, RET 등)
        // ...

    private:
        int getRegisterNumber(const std::string &reg) const;
    };

} // namespace Assembler