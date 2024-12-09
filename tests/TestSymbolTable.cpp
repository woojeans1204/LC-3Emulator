#include <iostream>
#include <cassert>
#include "Assembler/SymbolTable.h"

using namespace Assembler;

/**
 * SymbolTable 클래스의 단위 테스트
 * 테스트 항목:
 * 1. 라벨 추가 및 중복 처리 테스트
 * 2. 라벨 존재 여부 확인 테스트
 * 3. 주소값 조회 테스트
 * 4. 예외 상황 처리 테스트
 * 5. 테이블 초기화 테스트
 * 6. 다중 라벨 관리 테스트
 */

namespace SymbolTableTest
{
    void testAddLabel()
    {
        SymbolTable symTable;
        bool added = symTable.addLabel("LOOP", 0x3000);
        assert(added && "라벨 추가 실패");

        added = symTable.addLabel("LOOP", 0x3001);
        assert(!added && "중복 라벨 추가됨");
        std::cout << "라벨 추가 테스트 통과\n";
    }

    void testContains()
    {
        SymbolTable symTable;
        symTable.addLabel("START", 0x3000);
        assert(symTable.contains("START") && "START 라벨이 없음");
        assert(!symTable.contains("END") && "존재하지 않는 END 라벨이 있음");
        std::cout << "라벨 존재 확인 테스트 통과\n";
    }

    void testGetAddress()
    {
        SymbolTable symTable;
        symTable.addLabel("INIT", 0x3000);
        symTable.addLabel("END", 0x3001);
        int address = symTable.getAddress("INIT");
        assert(address == 0x3000 && "INIT 라벨의 주소가 올바르지 않음");
        address = symTable.getAddress("END");
        assert(address == 0x3001 && "END 라벨의 주소가 올바르지 않음");
        std::cout << "주소값 조회 테스트 통과\n";
    }

    void testGetAddressException()
    {
        SymbolTable symTable;
        symTable.addLabel("START", 0x3000);
        try
        {
            symTable.getAddress("MIDDLE");
            assert(false && "존재하지 않는 라벨에 대한 예외가 발생해야 함");
        }
        catch (const std::invalid_argument &e)
        {
            std::cout << "예외 상황 처리 테스트 통과\n";
        }
    }

    void testResetTable()
    {
        SymbolTable symTable;
        symTable.addLabel("TEST", 0x3000);
        assert(symTable.contains("TEST") && "리셋 전 라벨이 존재해야 함");

        symTable.reset();
        assert(!symTable.contains("TEST") && "리셋 후 라벨이 존재하지 않아야 함");
        std::cout << "테이블 초기화 테스트 통과\n";
    }

    void testMultipleLabels()
    {
        SymbolTable symTable;
        symTable.addLabel("START", 0x3000);
        symTable.addLabel("MIDDLE", 0x3005);
        symTable.addLabel("END", 0x300A);

        assert(symTable.getAddress("START") == 0x3000 && "START 라벨의 주소가 올바르지 않음");
        assert(symTable.getAddress("MIDDLE") == 0x3005 && "MIDDLE 라벨의 주소가 올바르지 않음");
        assert(symTable.getAddress("END") == 0x300A && "END 라벨의 주소가 올바르지 않음");
        std::cout << "다중 라벨 관리 테스트 통과\n";
    }

    void runAllTests()
    {
        std::cout << "SymbolTable 단위 테스트 시작...\n";
        testAddLabel();
        testContains();
        testGetAddress();
        testGetAddressException();
        testResetTable();
        testMultipleLabels();
        std::cout << "모든 SymbolTable 테스트 통과!\n";
    }
}

int main()
{
    SymbolTableTest::runAllTests();
    return 0;
}