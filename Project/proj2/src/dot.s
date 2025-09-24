.globl dot

.text
# =======================================================
# FUNCTION: Dot product of 2 int vectors
# Arguments:
#   a0 (int*) is the pointer to the start of v0
#   a1 (int*) is the pointer to the start of v1
#   a2 (int)  is the length of the vectors
#   a3 (int)  is the stride of v0
#   a4 (int)  is the stride of v1
# Returns:
#   a0 (int)  is the dot product of v0 and v1
# Exceptions:
# - If the length of the vector is less than 1,
#   this function terminates the program with error code 75.
# - If the stride of either vector is less than 1,
#   this function terminates the program with error code 76.
# =======================================================
dot:

    # Prologue

    # check a2 (length) < 1
    ble a2, zero, exit_75

    # check a3, a4 (stride v0) < 1
    ble a3, zero, exit_76
    ble a4, zero, exit_76

    # main case: loop through arrays
    li t0, 0              # index i = 0
    li t1, 0              # sum = 0
    mv t3, a0             # t3 = ptr v0
    mv t4, a1             # t4 = ptr v1

loop_start:
    bge t0, a2, loop_end

    lw t5, 0(t3)         # t5 = v0[i]
    lw t6, 0(t4)         # t6 = v1[i]
    mul t5, t5, t6       # t5 = v0[i] * v1[i]
    add t1, t1, t5       # sum += v0[i] * v1[i]
    addi t0, t0, 1       # i++

    slli t2, a3, 2      # t3 = length * 4 (byte offset)
    add t3, t3, t2      # ptr v0 += stride v0
    slli t2, a4, 2      # t4 = length * 4 (byte offset)
    add t4, t4, t2      # ptr v1 += stride v1
    j loop_start


loop_end:


    # Epilogue
    mv a0, t1             # return sum in a0
    
    ret

exit_75:
    li a0, 75
    li a7, 93
    ecall

exit_76:
    li a0, 76
    li a7, 93
    ecall