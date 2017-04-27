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

ldr r0,=0x3F200000

/*
* There are 54 GPIO pins, seperated into 6 groups, and there are 4 bytes for each group
* So there are 6*4 = 24 bytes for all GPIO pins
* Within a group, there are 3 bits for a pin, more specifically:
* 000: Input; 	001: Output;
* 100: Alternate Function 0;
* 101: A-F 1; 	110: A-F 2; 	111: A-F 3; 	011: A-F 4; 	010: A-F 5;
*/
mov r1,#1
lsl r1,#21 	// Pin 47 occupies the 7th pin within this groupd -> 7(th) * 3 (bits) = 21 (lower)
str r1,[r0,#0x10] // Pin 47 belongs to 40-49 -> GPFSEL4, the 5th set of 4 bytes

/*
* The output set registers are used to set a GPIO pin
* 0x1C is GPSET0 (responsible for pin 0-31); 0x20 is GPSET1 (responsible for pin 32-53)
* For any bits in these two registers:
	0 = No effect, 1 = Set GPIO Pin n 
*/
mov r1,#1
lsl r1,#15 // along with the first line in loop, write a 1 to GPSET1 at offset 15 

loop$:
  str r1,[r0,#0x20] // turn LED on

  mov r2,#0x3F0000
  wait1$:
    sub r2,#1
    cmp r2,#0
    bne wait1$

  str r1,[r0,#0x2c] // 0x2c is GPCLR1, clear the previous written 1 at offset 15, turn LED off

  mov r2,#0x3F0000
  wait2$:
    sub r2,#1
    cmp r2,#0
    bne wait2$

b loop$
