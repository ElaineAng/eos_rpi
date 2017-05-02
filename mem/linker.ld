start_addr = 0x8000;

ENTRY(entry)

SECTIONS
{
  . = 0x80000000 + start_addr;

  /* .text is loaded at start_addr. */
  .text : AT(start_addr) { *(.text) }
  
  /* use this address if etext is not explicitly defined in the program. */
  PROVIDE(etext = .);

  .stab : {

    PROVIDE(STAB_BEGIN = .);
    *(.stab);
    PROVIDE(STAB_END = .);

    /* store a byte 0 at the end of this section. */
    BYTE(0);
  }
  
  .stabstr : {
    PROVIDE(STABSTR_BEGIN = .);
    *(.stabstr);
    PROVIDE(STABSTR_END = .);
    BYTE(0);
  }

  .data : { *(.data) }
  PROVIDE(edata = .);

  .bss : { *(.bss) *(COMMON) }
  . = ALIGN(8);

  . = . + 0x1000; /* 4KB of stack memory, size of one page */
  kernel_stack_start = .;
  PROVIDE(ekernel_stack_start = .);

  . = . + 0x1000;
  irq_stack_start = .;

  PROVIDE(kernel_end = .);
}
