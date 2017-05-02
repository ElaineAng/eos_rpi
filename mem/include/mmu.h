#ifndef MEMORY_H
#define MEMORY_H

#define KERNEL_BASE 0x80000000

#ifndef __ASSEMBLER__

#include <types.h>
#include <hardware.h>

#define P2V(x) (((uint32_t) x) + KERNEL_BASE)
#define V2P(X) (((uint32_t) x) - KERNEL_BASE)

#define PGSIZE 0x1000

#define ROUND_DOWN(x) ((x) & ~(PGSIZE-1))
#define ROUND_UP(x) (((x)+PGSIZE-1) & ~(PGSIZE-1))

extern char kernel_end[];

/* Memory Layout */
#define KERNEL_STACK_BOTTOM (KERNEL_BASE - 2 * PGSIZE)
#define USER_STACK_BOTTOM (KERNEL_BASE - 3 * PGSIZE)
#define KERNEL_SECTION_TABLE ((uint32_t) kernel_end) // ??
#define MMIO_BASE (KERNEL_SECTION_TABLE + 0x200000) // ??
#define ALLOCATABLE_MEMORY_START (KERNEL_BASE + 0x4000000)

#define MMIO_P2V(x) ((volatile uint_32 *) (MMIO_BASE + (x - MMIO_BASE_PHYSICAL)))

#define SECTION_TABLE_ALIGNMENT 0x4000
