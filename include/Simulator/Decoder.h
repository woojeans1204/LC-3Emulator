// include/Simulator/Decoder.h
#pragma once

#include <cstdint>
#include <memory>
#include "Instructions/Instruction.h"

namespace Simulator
{

    /**
     * @brief 기계어 명령어 코드를 해석하여 해당 명령어 객체를 생성하는 디코더 클래스입니다.
     */
    class Decoder
    {
    public:
        Decoder() = default;
        ~Decoder() = default;

        /**
         * @brief 기계어 명령어 코드를 해석하여 해당 명령어 객체를 생성합니다.
         *
         * @param instructionCode 16비트 기계어 명령어 코드
         * @return std::unique_ptr<Instructions::Instruction> 생성된 명령어 객체
         */
        std::unique_ptr<Instructions::Instruction> decode(uint16_t instructionCode);
    };

} // namespace Simulator