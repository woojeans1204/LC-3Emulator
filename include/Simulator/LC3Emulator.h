// include/Simulator/LC3Emulator.h
#pragma once

#include "RegisterFile.h"
#include "Memory.h"
#include <vector>
namespace Simulator
{

    class LC3Emulator
    {
    public:
        LC3Emulator();
        ~LC3Emulator() = default;

        /**
         * @brief 레지스터 파일에 대한 참조를 반환합니다.
         *
         * @return RegisterFile& 레지스터 파일 참조
         */
        RegisterFile &getRegisterFile();

        /**
         * @brief 메모리에 대한 참조를 반환합니다.
         *
         * @return Memory& 메모리 참조
         */
        Memory &getMemory();

        /**
         * @brief 시뮬레이터를 실행합니다.
         */
        void run();

        /**
         * @brief 프로그램을 메모리에 로드합니다.
         *
         * @param program 기계어 코드의 벡터
         * @param startAddress 로드할 시작 주소
         */
        void LC3Emulator::loadProgram(const std::vector<std::pair<uint16_t, uint16_t>> &program);

        /**
         * @brief 실행을 중지합니다.
         */
        void halt();

        /**
         * @brief 시뮬레이터가 중지되었는지 확인합니다.
         *
         * @return true 중지됨
         * @return false 실행 중
         */
        bool isHalted() const;

    private:
        RegisterFile regFile;
        Memory memory;
        bool halted;
    };

} // namespace Simulator