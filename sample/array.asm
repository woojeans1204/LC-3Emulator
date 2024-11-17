;-------------------------------------------------------------------------------
; Digital Computer Concepts and Practice                             Spring 2024
;
; Data structures: Array traversal
;
; Given is an array of int16 values. The symbol 'Array' indicates the beginning
; of the array. The data value 'ArrayLen' contains the length of the array.
;
; Write a program that traverses the entire array and prints out each element
; on a separate line.
;
; Notes:
; - initialize the stack before calling any functions using the provided
;   initStack() function
; - functions to print a decimal value, printInt(int16 x), and to advance the
;   output to the next line, 'printLn()', are provided and can be used as-is.
; - follow the CSAP LC-3 calling convention in your code
; - The symbols 'Array' and 'ArrayLen' are too far away to load directly.
;   Refer to code samples provided in the lecture to understand how to access
;   such symbols.


;-------------------------------------------------------------------------------
; Code section
;
.ORIG   x3000

; main
;
; Program entry point
main
        ;----- ADD YOUR CODE/DATA AFTER THIS LINE  -----
        JSR initStack

        LD  R3, Arradr
        LDI  R4, Arrlen
        
Lmm     LDR R0, R3, #0 
        JSR printInt
        JSR printLn
        ADD R3, R3, #1
        ADD R4, R4, #-1
        BRp Lmm
        
        Halt
   
;------------------------------------
Arrlen .FILL ArrayLen
Arradr .FILL Array

        ;----- ADD YOUR CODE/DATA BEFORE THIS LINE -----





;-------------------------------------------------------------------------------
; Helper functions - DO NOT MODIFY
;
; printInt(int16 x) - output x as a decimal value
; printLn()         - output a newline
; initStack()       - initialize the stack
;
; Note: do not call printDigit(int16 x) directly

; printInt(int16 x)
;
; Prints parameter x as a decimal value.
; Just for fun, we implement this with two (inefficient) recursive functions
;
; Input:
;   int16 x     value to print
;
printInt
        ADD   R6, R6, #-1
        STR   R7, R6, #0

        ADD   R1, R0, #0          ; move R0 into R1
        BRz   piZero              ; if R1 == 0, print a zero and exit
        BRp   piPrint             ; if R1 > 0, skip negating R1

        NOT   R1, R1              ; negate R1
        ADD   R1, R1, #1

        LD    R0, minus           ; print '-' character
        OUT

    piPrint
        ADD   R0, R1, #0          ; move |R1| back into R0

        JSR   printDigit          ; call the recursive printDigit function

    piDone
        LDR   R7, R6, #0
        ADD   R6, R6, #1
        RET

    piZero
        LD    R0, zero
        OUT
        BR    piDone

zero    .FILL x0030               ; '0' character
minus   .FILL x002D               ; '-' character


; printDigit(int16 x)
;
; Prints a positive integer x (recursiv implementation)
;
; Input
;   int16 x     value to print (>0)
printDigit
        ADD   R1, R0, #0          ; R1 = x
        AND   R0, R0, #0          ; R0 = 0

pdLoop  ; divide R1 by 10
        ADD   R2, R1, #-10        ; subtract 10, result in R2
        BRn   pdDone              ; if negative, we're done
        ADD   R0, R0, #1          ; increase quotient
        ADD   R1, R2, #0          ; move R2 back into R1
        BRp   pdLoop              ; repeat as long as R1 is positive

    pdDone
        ; R0 = R1 / 10, R1 = R1 % 10
        ADD   R0, R0, #0          ; if the quotient is 0...
        BRz   pdSkip              ;   x was <10, so no further recursion

        ; x >= 10 -> call printDigit recursively
        ADD   R6, R6, #-2         ; make room on stack
        STR   R7, R6, #1          ; save R7
        STR   R1, R6, #0          ; save R1 (digit to print)

        JSR   printDigit

        LDR   R1, R6, #0          ; restore R1
        LDR   R7, R6, #1          ; restore R7
        ADD   R6, R6, #2          ; clean up stack

    pdSkip
        ; R1: digit to print
        LD    R0, zero            ; ASCII code of '0'
        ADD   R0, R0, R1          ; add value of digit
        OUT                       ; and print

        RET


; printLn()
;
; Print a newline character
printLn
        AND   R0, R0, #0
        ADD   R0, R0, #10         ; 0x0a is the newline character
        OUT

        RET


; initStack()
;
; Initialize the stack pointer (R6)
;
; This function demonstrates a common trick used on architectures with limited
; LD/LEA ranges: Since the symbol USPHIGH is too far away to be loaded with LEA,
; we have the assembler generate a nearby data word that contains the address
; of the desired symbol and the simply load that data word into the register 
; with LD
initStack
        LD    R6, sp
        RET  
sp      .FILL USPHIGH

.END



;-------------------------------------------------------------------------------
; Data section
;
.ORIG   x4000

; int16 Array[ArrayLen]
; For testing purposes, you can modify
; * the elements of the array
; * the length of the array (ArrayLen)
;   If you make the array longer, do not forget to add more data values.
;   If value in ArrayLen is smaller than the actual number of data, no harm
;   is done.

ArrayLen .FILL 10                 ; length of array
Array   .FILL 7                   ; array data
        .FILL 3
        .FILL 75
        .FILL 35
        .FILL 5000
        .FILL -22
        .FILL -5
        .FILL -1975
        .FILL 169
        .FILL 81

.END



;-------------------------------------------------------------------------------
; Stack area - DO NOT MODIFY
;
; The symbol USPLOW is located at address xf000, USPHIGH at xfe000.
; The stack is assumed to grow towards smaller addresses, hence, R6 should be
; initialized to USPHIGH with
;       LEA   R6, USPHIGH
;
; If stack range checks are performed, the following must hold
; * after growing the stack USPLOW <= R6
; * after cleaning the stack R6 <= USPHIGH
;
.ORIG   xf000
USPLOW  .BLKW xe00
USPHIGH

.END
