// tests/TestSymbolTable.cpp

// 테스트내용
// 라벨 추가: 새로운 라벨을 심볼 테이블에 추가할 수 있는지 테스트합니다.
// 중복 라벨 처리: 이미 존재하는 라벨을 다시 추가하려고 할 때 올바르게 처리되는지 테스트합니다.
// 라벨 존재 확인: 특정 라벨이 심볼 테이블에 존재하는지 확인할 수 있는지 테스트합니다.
// 주소 조회: 특정 라벨에 매핑된 주소를 올바르게 반환하는지 테스트합니다.
// 예외 처리: 존재하지 않는 라벨의 주소를 조회할 때 예외가 발생하는지 테스트합니다.

#include <iostream>
#include <cassert>
#include "Assembler/SymbolTable.h"

using namespace Assembler;

void testAddLabel()
{
    SymbolTable symTable;
    bool added = symTable.addLabel("LOOP", 0x3000);
    assert(added && "Failed to add a new label.");
    added = symTable.addLabel("LOOP", 0x3001);
    assert(!added && "Duplicate label was added.");
    std::cout << "testAddLabel passed.\n";
}

void testContains()
{
    SymbolTable symTable;
    symTable.addLabel("START", 0x3000);
    assert(symTable.contains("START") && "SymbolTable should contain 'START'.");
    assert(!symTable.contains("END") && "SymbolTable should not contain 'END'.");
    std::cout << "testContains passed.\n";
}

void testGetAddress()
{
    SymbolTable symTable;
    symTable.addLabel("INIT", 0x3000);
    symTable.addLabel("END", 0x3001);
    int address = symTable.getAddress("INIT");
    assert(address == 0x3000 && "Incorrect address for 'INIT'.");
    address = symTable.getAddress("END");
    assert(address == 0x3001 && "Incorrect address for 'END'.");
    std::cout << "testGetAddress passed.\n";
}

void testGetAddressException()
{
    SymbolTable symTable;
    symTable.addLabel("START", 0x3000);
    try
    {
        symTable.getAddress("MIDDLE");
        assert(false && "Expected exception for non-existent label.");
    }
    catch (const std::invalid_argument &e)
    {
        std::cout << "testGetAddressException passed.\n";
    }
}

int main()
{
    std::cout << "Running SymbolTable Tests...\n";
    testAddLabel();
    testContains();
    testGetAddress();
    testGetAddressException();
    std::cout << "All SymbolTable Tests Passed!\n";
    return 0;
}