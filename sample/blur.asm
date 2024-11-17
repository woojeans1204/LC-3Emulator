;-------------------------------------------------------------------------------
; Digital Computer Concepts and Practice                             Spring 2024
;
; Blurring filter
;
; Given is a 5x5 image of gray-scale values (0x00: black - 0xff: white) and
; a 3x3 convolution filter in .8 fixed-point format
;
; Convolute the input image with the filter and place the result in the output
; image
;
; Difficulty level: high
;

.ORIG   x3000
    BR    Main

IMG                   ; 5x5 image data
    .FILL x0011       ;   IMG[0][0]
    .FILL x0012       ;   IMG   [1]
    .FILL x0013       ;   IMG   [2]
    .FILL x0014       ;   IMG   [3]
    .FILL x0015       ;   IMG   [4]
    .FILL x0021       ;   IMG[1][0]
    .FILL x0022       ;   IMG   [1]
    .FILL x0023       ;   ...
    .FILL x0024
    .FILL x0025
    .FILL x0031       ;   IMG[2][0]
    .FILL x0032
    .FILL x0033
    .FILL x0034
    .FILL x0035
    .FILL x0041       ;   IMG[3][0]
    .FILL x0042
    .FILL x0043
    .FILL x0044
    .FILL x0045
    .FILL x0051       ;   IMG[4][0]
    .FILL x0052
    .FILL x0053
    .FILL x0054
    .FILL x0055      ;   IMG[4][4]

FLT .FILL x0010       ; 3x3 convolution filter
    .FILL x0020
    .FILL x0010
    .FILL x0020
    .FILL x0040
    .FILL x0020
    .FILL x0010
    .FILL x0020
    .FILL x0010

BLR                   ; 3x3 blurred image data      correct result:
    .FILL xffff       ;   BLR[0][0]                       x0020
    .FILL xffff       ;   BLR   [1]                       x0020
    .FILL xffff       ;   BLR   [2]                       x0021
    .FILL xffff       ;   BLR[1][0]                       x0030
    .FILL xffff       ;   BLR   [1]                       x0030
    .FILL xffff       ;                                   x0031
    .FILL xffff       ;   BLR[2][0]                       x0040
    .FILL xffff       ;                                   x0040
    .FILL xffff       ;   BLR[2][2]                       x0041


Main
    ;------------------------------------------------------------
    ; add your code below this line
    
    LD R6, STKTOP
    JSR Blur
    

    ; add your code above this line
    ;------------------------------------------------------------

    HALT


;------------------------------------------------------------
; add your data below this line

STKTOP .FILL xfe00 ; inital R6 value
C_-256 .FILL xff00 ; C_-256 = -256, unsigned bitmask context


; storing image idx offset
; 0 1 2
; 5 6 7
; 10 11 12
IDXOFT .FILL x0000  
       .FILL x0001
       .FILL x0002
       .FILL x0005
       .FILL x0006
       .FILL x0007
       .FILL x000a
       .FILL x000b
       .FILL x000c
       
; real image arr 
; approaching image memory with those indexes
IMGIDX .FILL x0000  
       .FILL x0001
       .FILL x0002
       .FILL x0005
       .FILL x0006
       .FILL x0007
       .FILL x000a
       .FILL x000b
       .FILL x000c

;
;   executing blur process
;   results are stored in BLR arrays
Blur
    ADD R6, R6, #-5
    
    STR R3, R6, #1
    STR R4, R6, #2
    STR R5, R6, #3
    STR R7, R6, #4

    AND R3, R3, #0
    ADD R3, R3, #2 ; R3 = 2
    
    AND R4, R4, #0
    ADD R4, R4, #2 ; R4 = 2
    
    LEA R5, BLR
    ADD R5, R5, #8
    
Lbl1

Lbl2
    ADD R1, R3, #0
    ADD R2, R4, #0
    
    
    JSR Alter_idx ; change the image array index that is used in convolute function
    JSR Convolute ; calculate the sum of fpmul's, R0 = result
    STR R0, R5, #0 ; store R0 in BLR
    ADD R5, R5, #-1 ; add BLR index
    
    ADD R4, R4, #-1
    BRzp Lbl2
    
    ADD R4, R4, #3 ; R4 = 2
    ADD R3, R3, #-1  
    BRzp Lbl1


    LDR R3, R6, #1
    LDR R4, R6, #2
    LDR R5, R6, #3
    LDR R7, R6, #4
    RET
    
    
;
;   altering arroft
;   IMGIDX = ARROFT + R1*5+R2
Alter_idx
    ADD R6, R6, #-5
    
    STR R3, R6, #1
    STR R4, R6, #2
    STR R5, R6, #3
    STR R7, R6, #4
    
    
    ADD R0, R1, R1
    ADD R0, R0, R0
    ADD R0, R0, R1 ; R0 = R1*5
    ADD R0, R0, R2 ; R0 = R1*5 + R2
    
    AND R3, R3, #0
    ADD R3, R3, #9 ; R3 = 9
    LEA R4, IDXOFT
    LEA R5, IMGIDX

LL  
    LDR R1, R4, #0
    ADD R2, R1, R0
    STR R2, R5, #0
    
    ADD R4, R4, #1
    ADD R5, R5, #1
    ADD R3, R3, #-1
    BRp LL

    LDR R3, R6, #1
    LDR R4, R6, #2
    LDR R5, R6, #3
    LDR R7, R6, #4
    
    ADD R6, R6, #5
    RET


;
;  convolute image in idx with filter and store in R0
;  R0 = (convoluted value)
Convolute
    ADD R6, R6, #-8
    
    STR R3, R6, #4
    STR R4, R6, #5
    STR R5, R6, #6
    STR R7, R6, #7
    
    AND R0, R0, #0 
    ADD R0, R0, #9 ; R0 = 9
    STR R0, R6, #0 ; R0 -> Frame[0]
    
    LEA R0, IMGIDX
    STR R0, R6, #1 ; IMGIDX-> Frame[1]
    
    AND R3, R3, #0
    LEA R5, FLT
    
Lt 
    LDR R0, R6, #1 ; R0 <- Frame[1] = IMGIDX address
    LDR R1, R0, #0
    ADD R0, R1, #0 ; R0 <- current IMGIDX = idx offset
    
    LEA R4, IMG
    ADD R4, R4, R0 ; R4 += idx offset
    LDR R1, R4, #0
    LDR R2, R5, #0
    
    JSR Fpmul ; R0 = fpmul(R1, R2)
    ADD R3, R3, R0 ; R3 += R0
    
    LDR R0, R6, #1
    ADD R0, R0, #1
    STR R0, R6, #1 ; inc IMGIDX address
    
    ADD R5, R5, #1
    
    LDR R0, R6, #0
    ADD R0, R0, #-1
    STR R0, R6, #0 ; dec counter
    BRp Lt
    
    ADD R0, R3, #0 ; R0 = result
    

    LDR R3, R6, #4
    LDR R4, R6, #5
    LDR R5, R6, #6
    LDR R7, R6, #7
    
    ADD R6, R6, #8
    RET


;  Multiply 2 unsigned integers
;  R0 = R1 * R2
;
Mul
    ADD R6, R6, #-1
    STR R7, R6, #0
    AND R0, R0, #0
    
La  
    ADD R0, R0, R2  ; R0 = R1 * R2
    ADD R1, R1, #-1
    BRp La
    
    LDR R7, R6, #0
    ADD R6, R6, #1
    RET 


;
;  Divide R0 by 256
;  R0 = R0 / 256
Divide_256
    ADD R6, R6, #-4
    
    STR R3, R6, #0
    STR R4, R6, #1
    STR R5, R6, #2
    STR R7, R6, #3
    
    ADD R3, R0, #0
    LD  R4, C_-256
    AND R0, R0, #0
    AND R5, R3, R4
    BRz Sk
Lb  
    ADD R0, R0, #1
    ADD R3, R3, R4
    AND R5, R3, R4 ; R3 >= 256
    BRnp Lb
Sk
    LDR R3, R6, #0
    LDR R4, R6, #1
    LDR R5, R6, #2
    LDR R7, R6, #3
    
    ADD R6, R6, #4
    RET
    

;  
;   multiply two 0.8 fixed point numbers
;   fpmul: R0 = R1 * R2
Fpmul
    ADD R6, R6, #-1
    STR R7, R6, #0
    JSR Mul
    JSR Divide_256
    LDR R7, R6, #0
    ADD R6, R6, #1
    RET
    
.END
