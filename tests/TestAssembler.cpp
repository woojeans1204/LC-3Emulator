// tests/TestAssembler.cpp
#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>
#include "Assembler/Assembler.h"

int main()
{
    // 테스트 디렉토리 설정
    std::string testDir = "tests/Debug";
    std::string inputFile = testDir + "/input.asm";
    std::string outputFile = testDir + "/output.obj";

    // 어셈블리 코드 준비 (더 많은 명령어 포함)
    std::string assemblyCode = R"(
.ORIG x3000      ; 시작 주소 지정

; 테스트를 위한 레이블 정의
LABEL1  .FILL x1234
LABEL2  .BLKW 2
LABEL3  .STRINGZ "Hello"

; 데이터 이동 명령어 테스트
        LEA R0, LABEL3
        LD R1, LABEL1
        LDI R2, LABEL1
        LDR R3, R1, #0
        ST R1, LABEL2
        STI R2, LABEL2
        STR R3, R1, #1

; 연산 명령어 테스트
        ADD R4, R4, #10
        ADD R5, R5, R6
        AND R7, R7, #15
        NOT R0, R0

; 제어 흐름 명령어 테스트
        BRnzp LABEL1
        JSR SUBROUTINE
        JMP R0
SUBROUTINE
        RET

; TRAP 서비스 루틴 테스트
        GETC
        OUT
        PUTS
        IN
        PUTSP
        HALT

.END             ; 프로그램 종료
)";

    // 입력 파일 작성
    try
    {
        std::ofstream inputStream(inputFile);
        if (!inputStream.is_open())
        {
            std::cerr << "Failed to create input.asm file at " << inputFile << std::endl;
            return 1;
        }
        inputStream << assemblyCode;
        inputStream.close();
    }
    catch (const std::ofstream::failure &e)
    {
        std::cerr << "Exception writing to input.asm: " << e.what() << std::endl;
        return 1;
    }

    // Assembler 인스턴스 생성 및 어셈블 수행
    Assembler::Assembler assembler;
    try
    {
        assembler.assemble(inputFile, outputFile);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Assembly failed: " << e.what() << std::endl;
        return 1;
    }

    // 어셈블 결과 파일 읽기
    std::vector<unsigned short> machineCodes;
    try
    {
        std::ifstream objStream(outputFile, std::ios::binary);
        if (!objStream.is_open())
        {
            std::cerr << "Failed to open output.obj file at " << outputFile << std::endl;
            return 1;
        }

        unsigned short code;
        while (objStream.read(reinterpret_cast<char *>(&code), sizeof(unsigned short)))
        {
            machineCodes.push_back(code);
        }
        objStream.close();
    }
    catch (const std::ifstream::failure &e)
    {
        std::cerr << "Exception reading output.obj: " << e.what() << std::endl;
        return 1;
    }

    // 예상 기계어 코드 (어셈블러 구현에 따라 정확한 값으로 수정 필요)
    std::vector<unsigned short> expectedMachineCodes = {
        0x3000, // 주소
        // LABEL1
        0x1234, // .FILL x1234
        // LABEL2
        0x3001, // 주소
        0x0000, // .BLKW 2 (첫 번째 워드)
        0x3002, // 주소
        0x0000, // .BLKW 2 (두 번째 워드)
        // LABEL3
        0x3003, // 주소
        'H',    // 'H'
        0x3004,
        'e', // 'e'
        0x3005,
        'l', // 'l'
        0x3006,
        'l', // 'l'
        0x3007,
        'o', // 'o'
        0x3008,
        0x0000, // Null terminator

        // LEA R0, LABEL3
        0x3009,                                                // 주소
        0xE000 | (0 << 9) | ((0x3003 - (0x3009 + 1)) & 0x1FF), // LEA R0, LABEL3

        // LD R1, LABEL1
        0x300A,
        0x2000 | (1 << 9) | ((0x3000 - (0x300A + 1)) & 0x1FF), // LD R1, LABEL1

        // LDI R2, LABEL1
        0x300B,
        0xA000 | (2 << 9) | ((0x3000 - (0x300B + 1)) & 0x1FF), // LDI R2, LABEL1

        // LDR R3, R1, #0
        0x300C,
        0x6000 | (3 << 9) | (1 << 6) | (0 & 0x3F), // LDR R3, R1, #0

        // ST R1, LABEL2
        0x300D,
        0x3000 | (1 << 9) | ((0x3001 - 0x300D - 1) & 0x1FF), // ST R1, LABEL2

        // STI R2, LABEL2
        0x300E,
        0xB000 | (2 << 9) | ((0x3001 - 0x300E - 1) & 0x1FF), // STI R2, LABEL2

        // STR R3, R1, #1
        0x300F,
        0x7000 | (3 << 9) | (1 << 6) | (1 & 0x3F), // STR R3, R1, #1

        // ADD R4, R4, #10
        0x3010,
        0x1000 | (4 << 9) | (4 << 6) | (1 << 5) | (10 & 0x1F), // ADD R4, R4, #10

        // ADD R5, R5, R6
        0x3011,
        0x1000 | (5 << 9) | (5 << 6) | (0 << 5) | (6 & 0x7), // ADD R5, R5, R6

        // AND R7, R7, #15
        0x3012,
        0x5000 | (7 << 9) | (7 << 6) | (1 << 5) | (15 & 0x1F), // AND R7, R7, #15

        // NOT R0, R0
        0x3013,
        0x9000 | (0 << 9) | (0 << 6) | 0x3F, // NOT R0, R0

        // BRnzp LABEL1
        0x3014,
        0x0E00 | ((0x3000 - 0x3014 - 1) & 0x1FF), // BRnzp LABEL1

        // JSR SUBROUTINE
        0x3015,
        0x4801 | ((0x3016 - 0x3015 - 1) & 0x7FF), // JSR SUBROUTINE

        // JMP R0
        0x3016,
        0xC000 | (0 << 6), // JMP R0

        // SUBROUTINE
        0x3017,
        // RET
        0xC1C0, // RET (JMP R7)

        // GETC
        0x3018,
        0xF020, // TRAP x20

        // OUT
        0x3019,
        0xF021, // TRAP x21

        // PUTS
        0x301A,
        0xF022, // TRAP x22

        // IN
        0x301B,
        0xF023, // TRAP x23

        // PUTSP
        0x301C,
        0xF024, // TRAP x24

        // HALT
        0x301D,
        0xF025, // TRAP x25
    };

    // 검증: 어셈블 결과와 예상 기계어 코드 비교
    bool testPassed = true;

    // 예상되는 기계어 코드와 실제 결과의 크기가 동일한지 확인
    if (machineCodes.size() != expectedMachineCodes.size())
    {
        std::cerr << "Mismatch in number of machine codes. Expected: " << expectedMachineCodes.size()
                  << ", Got: " << machineCodes.size() << std::endl;
        testPassed = false;
    }
    else
    {
        // 각 워드별로 비교
        for (size_t i = 0; i < expectedMachineCodes.size(); ++i)
        {
            if (machineCodes[i] != expectedMachineCodes[i])
            {
                std::cerr << "Mismatch at word " << i << ": Expected 0x" << std::hex << expectedMachineCodes[i]
                          << ", Got 0x" << machineCodes[i] << std::dec << std::endl;
                testPassed = false;
            }
        }
    }

    if (testPassed)
    {
        std::cout << "Assembler test passed successfully." << std::endl;
    }
    else
    {
        std::cerr << "Assembler test failed." << std::endl;
    }

    // 테스트 파일 정리 (선택 사항)
    // try
    // {
    //     std::filesystem::remove(inputFile);
    //     std::filesystem::remove(outputFile);
    // }
    // catch (const std::filesystem::filesystem_error &e)
    // {
    //     std::cerr << "Filesystem error during cleanup: " << e.what() << std::endl;
    // }

    return testPassed ? 0 : 1;
}
