
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
