.section ".text.boot"

.globl _start

_start:

  // setup the stack
  mov sp, #0x10000

  // clear out bss.
  ldr r4, =__bss_start // this loads content from memory address __bss_start to r4
  ldr r9, =__bss_end
  mov r5, #0
  mov r6, #0
  mov r7, #0
  mov r8, #0
  b 2f  // branch jump forwards

  1:
    /*Registers are loaded stored and in numerical order,
    with the lowest numbered register at the address initially in Rn.
      The value in Rn is incremented by 4 times the number of registers in reglist.
    */
    stmia r4!, {r5-r8}

  2:
    cmp r4, r9
    /* BLO is a synonym for BCC; it performs a conditional branch
    based on the carry © flag being clear in the PSR,
    which is the case when one unsigned value compares lower (LO) than another.
    */
    blo 1b //jump backwards

    // Call kernel_main

    ldr r3, =kernel_main
    blx r3

    // halt
  halt:

    /*WFE waif for event
      If the Event Register is not set,
      WFE suspends execution until one of the following events occurs:
      an IRQ interrupt, unless masked by the CPSR I-bit
      an FIQ interrupt, unless masked by the CPSR F-bit
      an Imprecise Data abort, unless masked by the CPSR A-bit
      a Debug Entry request, if Debug is enabled
      an Event signaled by another processor using the SEV instruction.
      If the Event Register is set, WFE clears it and returns immediately.
      If WFE is implemented, SEV must also be implemented.
    */

    wfe
    b halt
