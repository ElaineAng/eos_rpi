#include <timer.h>
#include <uart.h>


static inline uint32_t get_sys_timer()
{
  uint32_t sys_timer = 0x3F003000;
  return  *(volatile uint32_t*) sys_timer;
}

void wait(uint32_t count)
{
  uint32_t init_val = get_sys_timer();
  uint32_t elapsed = 0;
  uart_putc((unsigned char) init_val);
  uart_putc((unsigned int) 40);
  while (elapsed <= count) {
    elapsed = get_sys_timer() - init_val;
    //   uart_puts("wooo\n");
  }
}
