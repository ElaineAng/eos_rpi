#include <mmu.h>

.global set_domain_access_control
set_domain_access_control:

	/* MRC p15, 0, <Rt>, c3, c0, 0     ;Write Rt to DACR.
	 * MRC p15, 0, <Rt>, c3, c0, 0 	   ;Read DACR into Rt
	 * DACR defines the access permission for each of the sixteen memory domains.
	 * 32 bit register, 2 bit per domain.
	 * The permission values are:
	 * 0b00: No access. Any access to the domain generates a Domain fault.
	 * 0b01: Client. Accesses are checked against the permission bits in the translation tables.
	 * 0b10: Reserved, effect is UNPREDICTABLE.
	 * 0b11: Manager. Accesses are not checked against the permission bits in the translation tables.
	 */

	mcr p15, 0, r0, cr3, cr0, 0
	mov pc, lr
	
.global set_translation_table_base
set_translation_table_base:

	/* Translation Table Base Register 0 (TTBR0) */

	mcr p15, 0, r0, cr2, cr0, 0
	mov pc, lr

.global read_control_register	
read_control_register:

	/* System Control Register (SCTLR)
	 * The lowest bit is the address translation enable bit.
	 * It is a global enable bit for the MMU stage 1 address translation
	 * 0 not enable, 1 enable.
	 */
	
	mrc p15, 0, r0, cr1, cr0, 0
	mov pc, lr

.global set_control_register
set_control_register:
	mcr p15, 0, r0, cr1, cr0, 0
	mov pc, lr

.global jump_to_high_mem
jump_to_high_mem:
	add lr, #KERNEL_BASE
	mov pc, lr
