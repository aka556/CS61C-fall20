.globl factorial

.data
n: .word 8

.text
main:
    la t0, n # t0是n的寄存器
    lw a0, 0(t0)
    jal ra, factorial

    addi a1, a0, 0
    addi a0, x0, 1
    ecall # Print Result

    addi a1, x0, '\n'
    addi a0, x0, 11
    ecall # Print newline

    addi a0, x0, 10
    ecall # Exit

factorial:
    # YOUR CODE HERE
    # 基础情况, n = 0, n = 1
    addi t0, x0, 1 # t0 = 1
    beq a0, x0, return_one
    beq a0, t0, return_one

    addi sp, sp, -8 # 初始化寄存器，分配栈空间
    sw ra, 0(sp) # 保存返回地址
    sw a0, 4(sp) # 保存当前n的值

    addi a0, a0, -1
    jal ra, factorial # 递归调用

    lw t1, 4(sp)
    lw ra, 0(sp)
    addi sp, sp, 8

    mul a0, a0, t1
    jr ra

return_one:
    addi a0, x0, 1
    jr ra