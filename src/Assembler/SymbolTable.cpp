// src/Assembler/SymbolTable.cpp
#include "Assembler/SymbolTable.h"
#include <iostream>

namespace Assembler
{

    bool SymbolTable::addLabel(const std::string &label, int address)
    {
        // 이미 존재하는 라벨인지 확인
        if (table.find(label) != table.end())
        {
            return false; // 이미 존재함
        }
        // 라벨 추가
        table[label] = address;
        return true;
    }

    bool SymbolTable::contains(const std::string &label) const
    {
        return table.find(label) != table.end();
    }

    int SymbolTable::getAddress(const std::string &label) const
    {
        auto it = table.find(label);
        if (it == table.end())
        {
            throw std::invalid_argument("Label not found: " + label);
        }
        return it->second;
    }

    void SymbolTable::printTable() const
    {
        std::cout << "Symbol Table:\n";
        std::cout << "-----------------------\n";
        for (const auto &pair : table)
        {
            std::cout << "Label: " << pair.first << " -> Address: " << pair.second << "\n";
        }
        std::cout << "-----------------------\n";
    }

    void SymbolTable::reset()
    {
        table.clear();
    }
} // namespace Assembler
