.section .init

/*
* Difference with Pi 1:
* 1. Base physical address for SoC perpherals has moved from 0X20000000 to 0X3F000000
* 2. GPIO pin for OK/ACT LED moved from 16 to 47
* 3. Logic of 0 to enable and 1 to disable the LED is inverted (??)

* Our register use is as follows:
* r0=0x3F200000 the address of the GPIO region.
* r1=1 << 21    a number with bits 21-23 set to 001 to put into the GPFSEL4 NEW
*               function select to enable output to GPIO 47. NEW
* then
* r1=1 << 15    a number with bit 15 high, so we can communicate with GPIO 47. NEW
* r2=0x003F0000 a number that will take a noticeable duration for the processor
*               to decrement to 0, allowing us to create a delay.
*/
	
.globl _start
_start:
	b main
	.section .text

main:
	mov sp, #0x8000
	
	/*
	* Set the pin 47 (the LED Pin) as output (function code 1)
	*/
	pinNum .req r0
	pinFunc .req r1

	mov pinNum, #47
	mov pinFunc, #1

	bl SetGpioFunction	// bl: branch with link, it copies the addr of next instruction into lr (r14)

	.unreq pinNum
	.unreq pinFunc


	ptrn .req r4
	ldr ptrn, =pattern	// put the value of pattern (addr of the .int?) in ptrn
	ldr ptrn, [ptrn]	// load whatever in that addr into ptrn
	seq .req r5
	mov seq, #0
	
loop$:
	/*
	* Turn the LED on
	*/
	pinNum .req r0
	pinVal .req r1
	
	mov pinNum, #47
	mov pinVal, #1
	lsl pinVal, seq
	and pinVal, ptrn // for seq n, if ptrn == 1, send a non-zero to SetGpio, turn on LED
	bl SetGpio
	
	.unreq pinNum
	.unreq pinVal

	/*
	* Wait for sometime (using the Wait function in systemTimer)
	*/
	ldr r0, =250000
	bl Wait
	
	add seq, #1		// increament the counter;
	and seq, #0b11111	// effectively seq mod 32
	
	b loop$

	
.section .data
.align 2			// IMPORTANT! ldr only works at addresses that are multiples of 4
				// .align num ensures the address of the next line is a multiple of 2^num
pattern:	
	.int 0b00000000010101011101110111010101
