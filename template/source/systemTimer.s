.globl GetSystemTimerBase
GetSystemTimerBase:
	ldr r0, =0x3F003000
	mov pc, lr

.globl GetTimeStamp
GetTimeStamp:
	push {lr}
	bl GetSystemTimerBase
	ldrd r0, r1, [r0, #4]	// This loads lower 4 bytes of memory content at [r0+#4] into r0
				// and higher 4 bytes into r1
	pop {pc}

/*
* Notice that this only supports 4 byte wait value since we are not using the higher 4 bytes in r1
*/
.globl Wait
Wait:	
	delay .req r2
	mov delay, r0		// r0 is the first argument, which is the delay value
	push {lr}
	bl GetTimeStamp
	start .req r3
	mov start, r0

	loop$:
		bl GetTimeStamp
		elapsed .req r1
		sub elapsed, r0, start
		cmp elapsed, delay
		.unreq elapsed
		bls loop$	// if elapsed <= delay

	.unreq delay
	.unreq start
	pop {pc}
