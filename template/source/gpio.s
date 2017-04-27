.globl GetGpioAddress
GetGpioAddress:
	ldr r0, =0x3F200000
	mov pc,lr		// Return

	
.globl SetGpioFunction
SetGpioFunction:
	cmp r0, #53
	cmpls r1, #7		// compare lower or same, function code vary from 0-7
	movhi pc, lr		// move if (strictly) higher

	push {lr}
	mov r2, r0		// r2 is not used in GetGpioAddress

	bl GetGpioAddress	// bl causes a branch to label,
				// and copies the address of the next instruction into LR
	 
	/* When the call to GetGpioAddress returns,
	* we now know that r0 contains the GPIO address,
	* r1 contains the function code and r2 contains the GPIO pin number.
	*/

	functionLoop$:
		cmp r2, #9
		subhi r2, #10
		addhi r0, #4
		bhi functionLoop$

	/*
	* r2 will contain a number from 0-9
	* r0 will contain the address in the GPIO controller of this pin's function setting
	*/
	add r2, r2, lsl #1	// r2 = r2 + r2 * 2 = 3 * r2
	lsl r1, r2

	mask .req r3
	mov mask, #7
	lsl mask, r2

	mvn mask, mask
	oldFunc .req r2
	ldr oldFunc, [r0]
	and oldFunc, mask
	.unreq mask

	orr r1, oldFunc
	.unreq oldFunc
	
	str r1, [r0]
	pop {pc}

.globl SetGpio
SetGpio:
	pinNum .req r0
	pinVal .req r1		// pinVal now become the alias of r1

	cmp pinNum, #53
	movhi pc, lr
	push {lr}

	mov r2, pinNum
	.unreq pinNum		// .unreq remove the alias from r0
	pinNum .req r2

	bl GetGpioAddress
	gpioAddr .req r0

	pinBank .req r3
	lsr pinBank, pinNum, #5	// pinBank = pinNum / 32, get if the pin is in group 0 or 1
	lsl pinBank, #2
	add gpioAddr, pinBank
	.unreq pinBank

	and pinNum, #31		// Get the remainder of pinNUM / 31 to determine which pin to set
	setBit .req r3
	mov setBit, #1
	lsl setBit, pinNum
	.unreq pinNum

	teq pinVal, #0		// Test the value in pinVal against #0
				// Updates the conditional flags on the result without placing it in any reg
	.unreq pinVal
	strne setBit, [gpioAddr, #0x1c] // If pinVal is not 0, set a bit for the pin, which turn on LED
	streq setBit, [gpioAddr, #0x28] // If pinVal is 0, clear the bit for the pin, which turn off LED

	.unreq setBit
	.unreq gpioAddr
	pop {pc}
