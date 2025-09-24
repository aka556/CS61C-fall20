.globl matmul

.text
# =======================================================
# FUNCTION: Matrix Multiplication of 2 integer matrices
# 	d = matmul(m0, m1)
# Arguments:
# 	a0 (int*)  is the pointer to the start of m0 
#	a1 (int)   is the # of rows (height) of m0
#	a2 (int)   is the # of columns (width) of m0
#	a3 (int*)  is the pointer to the start of m1
# 	a4 (int)   is the # of rows (height) of m1
#	a5 (int)   is the # of columns (width) of m1
#	a6 (int*)  is the pointer to the the start of d
# Returns:
#	None (void), sets d = matmul(m0, m1)
# Exceptions:
#   Make sure to check in top to bottom order!
#   - If the dimensions of m0 do not make sense,
#     this function terminates the program with exit code 72.
#   - If the dimensions of m1 do not make sense,
#     this function terminates the program with exit code 73.
#   - If the dimensions of m0 and m1 don't match,
#     this function terminates the program with exit code 74.
# =======================================================
matmul:

    # Error checks
    
    # check m0 (a1, a2) dimensions
    ble a1, zero, exit_72   # if height m0 < 1
    ble a2, zero, exit_72   # if width m0 < 1

    # check m1 (a4, a5) dimensions
    ble a4, zero, exit_73   # if height m1 < 1
    ble a5, zero, exit_73   # if width m1 < 1

    # check if width m0 == height m1
    bne a2, a4, exit_74   # if width m0 != height m1

    # Prologue
    li t0, 0              # i = 0 (row index m0)

outer_loop_start:
    bge t0, a1, outer_loop_end  # if i == height m0, end outer loop

    li t1, 0              # j = 0 (column index m1)



inner_loop_start:
    bge t1, a5, inner_loop_end  # if j == width m1, end inner loop

    mul t2, t0, a2
    slli t2, t2, 2
    add t2, a0, t2      # t2 = &m0[i][0]

    # compute col_ptr = &m1[0][j]
    slli t3, t1, 2
    add t3, a3, t3      # t3 = &m1[0][j]

    mv a0, t2         # a0 = &m0[i][0]
    mv a1, t3         # a1 = &m1[0][j
    mv a2, a2         # a2 = width m0 = height m1
    li a3, 1          # stride m0 = 1
    mv a4, a5         # stride m1 = width m1
    jal dot           # call dot(&m0[i][0], &m1[0][j], width m0, 1, width m1)

    # store result in d[i][j]
    mul t4, t0, a5
    add t4, t4, t1
    slli t4, t4, 2
    add t4, a6, t4      # t4 = &d[i][j]
    sw a0, 0(t4)        # d[i][j] = dot(...)

    addi t1, t1, 1       # j++
    j inner_loop_start


inner_loop_end:
    addi t0, t0, 1       # i++
    j outer_loop_start


outer_loop_end:


    # Epilogue
    
    
    ret

exit_72:
    li a0, 72
    li a7, 93
    ecall

exit_73:
    li a0, 73
    li a7, 93
    ecall

exit_74:
    li a0, 74
    li a7, 93
    ecall