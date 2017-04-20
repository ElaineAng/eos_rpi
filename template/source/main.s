.section .init
.globl _start
_start:

ldr r0,=0x3F200000
mov r1,#1
lsl r1,#21
str r1,[r0,#0x10]

mov r1,#1
lsl r1,#15

loop$:
  str r1,[r0,#0x20]

  mov r2,#0x3F0000
  wait1$:
    sub r2,#1
    cmp r2,#0
    bne wait1$

  str r1,[r0,#0x2c]

  mov r2,#0x3F0000
  wait2$:
    sub r2,#1
    cmp r2,#0
    bne wait2$

b loop$
