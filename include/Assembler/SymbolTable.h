// include/Assembler/SymbolTable.h
#pragma once

#include <unordered_map>
#include <string>

namespace Assembler
{

    class SymbolTable
    {
    public:
        SymbolTable() = default;
        ~SymbolTable() = default;
        bool addLabel(const std::string &label, int address);

        bool contains(const std::string &label) const;
        int getAddress(const std::string &label) const;
        void printTable() const;
        void reset();

    private:
        std::unordered_map<std::string, int> table;
    };
} // namespace Assembler
