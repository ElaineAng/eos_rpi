#include <console.h>
#include <mmu.h>
#include <kalloc.h>
#include <monitor.h>
#include <proc.h>
#include <timer.h>

void c_entry(void)
{

  kalloc_init(ALLOCATABLE_MEMORY_START, KERNEL_BASE + INITIAL_MEMORY_START);
  
	      
