;-------------------------------------------------------------------------------
; Digital Computer Concepts and Practice                             Spring 2024
;
; Data structures: Stack
;
; Given is an LC-3 string (a sequence of characters where each character is
; stored in an int16). The string is terminated with the x0000 value.
;
; Write a program that uses a stack data structure to print the string in
; reverse order.
;
; Implement your program with two functions, push(int16) and pop() -> int16 that
; push and pop a value to/from the stack. Use the provided stack area 'STACK'
; to store your data. Unlike the function stack, our stack data structure grows
; towards higher addresses. If the stack over- or underflows, you can abort
; the program.
;
; Example input: "I love SNU!" --> output: "!UNS evol I"
;
; Notes:
; - initialize the stack before calling any functions using the provided
;   initStack() function
; - follow the CSAP LC-3 calling convention in your code


;-------------------------------------------------------------------------------
;                                Sample solution
;-------------------------------------------------------------------------------


;-------------------------------------------------------------------------------
; Code section
;
.ORIG   x3000


;----- ADD YOUR CODE/DATA AFTER THIS LINE  -----


; main
;
; Program entry point
main

        JSR   initStack           ; initialize stack before we do anything

        ;
        ; TODO
        ;
        LD  R4, Msg
        AND R5, R5, #0
        
ml      
        LDR R0, R4, #0
        BRz me ; end if null character
        
        JSR push
        ADD R5, R5, #1
        ADD R4, R4, #1
        BR ml
me  

        ADD R5, R5, #0
        BRz mee

mll 
        JSR pop
        OUT
        ADD R5, R5, #-1
        BRp mll
mee
        HALT


; push(int16 x)
;
; pushes the value in R0 on top of the data stack.
; Aborts the program if the stack is full with the message
;   "Stack overflow; halting program."
push
        ;
        ; TODO
        ;
        LD R1, STKPTR
        ADD R1, R1, #1
        
        LEA R2, STACKHIGH
        NOT R2, R2
        ADD R2, R2, #1
        ADD R2, R2, R1 ; R2 = STKPTR - STKHIGH
        BRn pua
        
        LEA R0, OVERMSG
        PUTS
        HALT
pua     
        STR R0, R1, #0 ; store R0
        ST  R1, STKPTR
        RET


; pop(int16 x)
;
; pops and returns the value on top of the data stack.
; Aborts the program if the stack is empty with the message
;   "Stack underflow; halting program."
pop
        ;
        ; TODO
        ;
        LD R1, STKPTR
        
        LEA R2, STACKLOW
        NOT R2, R2
        ADD R2, R2, #1
        ADD R2, R2, R1 ; R2 = STKPTR - STKLOW
        BRzp poa
        
        LEA R0, UNDERMSG
        PUTS
        HALT
poa     
        LDR R0, R1, #0 ; load R0
        ADD R1, R1, #-1
        ST  R1, STKPTR
        RET


OVERMSG .STRINGz "Stack overflow; halting program."
UNDERMSG .STRINGz "Stack underflow; halting program."
STKPTR  .FILL STLCHK
Msg .FILL Input
;----- ADD YOUR CODE/DATA BEFORE THIS LINE -----





;-------------------------------------------------------------------------------
; Helper functions - DO NOT MODIFY
;

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
; Stack data section - DO NOT MODIFY
;
.ORIG   x3080

STLCHK  .FILL xdead               ; lower stack sentinel
STACKLOW                          ; lower limit of stack area
STACK   .BLKW x20                 ; stack data
STACKHIGH                         ; upper limit of stack area
STHCHK  .FILL xbabe               ; higher stack sentinel

.END



;-------------------------------------------------------------------------------
; Data section
;
.ORIG   x4000

; Input string
; For testing purposes, you can modify the contents of the string. Always think
; of corner cases and test whether your algorithm works properly in these cases
; as well.

Input   .STRINGZ "I love SNU!"

.END



;-------------------------------------------------------------------------------
; Stack area - DO NOT MODIFY
;
; The symbol USPLOW is located at address xf000, USPHIGH at xfe00.
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
