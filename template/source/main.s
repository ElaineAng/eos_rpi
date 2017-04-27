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

	bl SetGpioFunction

	.unreq pinNum
	.unreq pinFunc


	loop$:
	/*
	* Turn the LED on
	*/
	pinNum .req r0
	pinVal .req r1
	
	mov pinNum, #47
	mov pinVal, #0
	bl SetGpio
	
	.unreq pinNum
	.unreq pinVal

	/*
	* Wait for sometime (for 0x3F0000 to decrease to 0)
	*/
	decr .req r2
	mov decr, #0x3F0000

	wait1$:
		sub decr, #1
		teq decr, #0	// teq does not affect V or C flag but cmp do
		bne wait1$
	.unreq decr
	
	/*
	* Turn the LED off
	*/
	pinNum .req r0
	pinVal .req r1

	mov pinNum, #47
	mov pinVal, #1
	bl SetGpio

	.unreq pinNum
	.unreq pinVal


	/*
	* Wait for sometime
	*/
	decr .req r2
	mov decr, #0x3F0000
	wait2$:
		sub decr, #1
		teq decr, #0
		bne wait2$
	.unreq decr

	b loop$
