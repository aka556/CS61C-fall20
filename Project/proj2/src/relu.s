.globl relu

.text
# ==============================================================================
# FUNCTION: Performs an inplace element-wise ReLU on an array of ints
# Arguments:
# 	a0 (int*) is the pointer to the array
#	a1 (int)  is the # of elements in the array
# Returns:
#	None
# Exceptions:
# - If the length of the vector is less than 1,
#   this function terminates the program with error code 78.
# ==============================================================================
relu:
    # Prologue
    
    # base case: check if length < 1
    ble a1, zero, exit_78

    # main case: loop through array
    li t0, 0              # index i = 0
    slli t1, a1, 2        # t1 = length * 4 (byte offset)

loop_start:
    beq t0, t1, loop_end # if i == length * 4, end loop
    add t2, a0, t0       # t2 = &array[i]
    lw t3, 0(t2)         # t3 = array[i]
    bge t3, zero, loop_continue # if array[i] >= 0,
    
    # set array[i] to 0
    sw zero, 0(t2)       # array[i] = 0
    j loop_continue



loop_continue:
    addi, t0, t0, 4      # i++
    j loop_start


loop_end:


    # Epilogue

    
	ret

exit_78:
    li a0, 78
    li a7, 93
    ecall
