
.ORIG x3000      ; 시작 주소 지정
START  ADD R1, R2, R3
       AND R4, R5, #10
       NOT R6, R7
       RET
.END             ; 프로그램 종료
