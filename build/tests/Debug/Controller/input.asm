; Test Program using ADD, AND, NOT
    .ORIG x3000
    ; Initialize R0 to 5
    AND R0, R0, #0     ; R0 = 0
    ADD R0, R0, #5     ; R0 = R0 + 5

    ; Initialize R1 to -1 (All bits set to 1)
    NOT R1, R0         ; R1 = NOT R0
    ADD R1, R1, #0     ; R1 = R1 (No change)

    ; Compute R2 = R0 AND R1 (Should be 0)
    AND R2, R0, R1     ; R2 = R0 AND R1

    HALT
    .END
