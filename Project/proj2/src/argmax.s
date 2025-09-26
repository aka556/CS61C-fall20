.globl argmax

.text
# =================================================================
# FUNCTION: Given a int vector, return the index of the largest
#	element. If there are multiple, return the one
#	with the smallest index.
# Arguments:
# 	a0 (int*) is the pointer to the start of the vector
#	a1 (int)  is the # of elements in the vector
# Returns:
#	a0 (int)  is the first index of the largest element
# Exceptions:
# - If the length of the vector is less than 1,
#   this function terminates the program with error code 77.
# =================================================================
argmax:

    # Prologue

    # base case: check if length < 1
    ble a1, zero, exit_77

    # length = 1, return index 0
    li t0, 1
    beq a1, t0, return_zero

    # main case: loop througth array
    li t0, 0              # index i = 0
    li t1, 0              # max_index = 0
    lw t2, 0(a0)          # max_value = array[0]
    slli t3, a1, 2        # t3 = length * 4 (byte offset)
    addi t0, t0, 4        # i = 1

loop_start:
    beq t0, t3, loop_end
    add t4, a0, t0       # t4 = &array[i]
    lw t5, 0(t4)         # t5 = array[i]
    ble t5, t2, loop_continue # if array[i] <= max_value
    mv t2, t5
    srai t1, t0, 2       # max_index = i

loop_continue:
    addi t0, t0, 4       # i++
    j loop_start


loop_end:
    mv a0, t1             # return max_index in a0
    # Epilogue


    ret

return_zero:
    li a0, 0
    ret
    
exit_77:
    li a0, 77
    jal exit2