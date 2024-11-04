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

        unsigned short encodeJmp(const std::string &baseR);

        unsigned short encodeLd(const std::string &dr, int pcOffset9);

        unsigned short encodeLdi(const std::string &dr, int pcOffset9);

        unsigned short encodeLdr(const std::string &dr, const std::string &baseR, int offset6);

        unsigned short encodeLea(const std::string &dr, int pcOffset9);

        unsigned short encodeSt(const std::string &sr, int pcOffset9);

        unsigned short encodeSti(const std::string &sr, int pcOffset9);

        unsigned short encodeStr(const std::string &sr, const std::string &baseR, int offset6);

        unsigned short encodeTrap(unsigned char trapvect8);

        unsigned short encodeJsr(int pcOffset11);

        unsigned short encodeJsrr(const std::string &baseR);

        unsigned short encodeRti();

        unsigned short encodeGetc();

        unsigned short encodeOut();

        unsigned short encodePuts();

        unsigned short encodeIn();

        unsigned short encodePutsp();

        unsigned short encodeHalt();

    private:
        int getRegisterNumber(const std::string &reg) const;
    };

} // namespace Assembler