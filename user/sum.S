.section .data
    .align 2
buf:    .space  MAX_BUF_SIZE
fmt:    .string "%d\n"
errMsg: .string "Incorrect data format\n"

.section .text
.globl main
main:
    addi    sp, sp, -32
    sw      ra, 28(sp)
    la      a0, buf
    li      a1, MAX_BUF_SIZE
    jal     gets

    mv      t0, a0
    mv      a0, t0
    jal     atoi
    move    s0, a0

    li      s2, 0
    li      s1, 0
    li      t0, 0
loop:
    lbu     t1, 0(t0)
    beqz    t1, check_format
    bltz    t1, error
    bgez    t1, is_digit
    beq     t1, zero, is_space
    j       increment
is_digit:
    addi    s1, s1, 1
    j       increment
is_space:
    addi    s2, s2, 1
increment:
    addi    t0, t0, 1
    j       loop
check_format:
    sub     t2, t0, 1
    beqz    t2, continue_check
    bltz    t2, error
    lb      t3, -1(t2)
    bgez    t3, error
    beqz    s1, continue_check
    bgez    s1, continue_check
error:
    la      a0, errMsg
    li      a7, 4
    ecall
    li      a7, 10
    li      a0, 1
    ecall
continue_check:
    mv      t0, s0
    li      t1, 1
    move    s3, zero
rank_loop:
    divu    t0, t1
    remu    t2, t0, t1
    beqz    t0, calc_num2
    mul     t1, t1, 10
    addi    s3, s3, 1
    j       rank_loop
calc_num2:
    addi    t4, t3, 1
    bnez    s0, num1_not_zero
    addi    t4, t4, 1
num1_not_zero:
    la      a0, buf
    add     a0, a0, t4
    jal     atoi
    move    s4, a0

    move    a0, s0
    move    a1, s4
    jal     add_numbers

    move    a0, a0
    la      a1, fmt
    li      a7, 0
    ecall

    li      a7, 10
    ecall

add_numbers:
    add     a0, a0, a1
    jr      ra