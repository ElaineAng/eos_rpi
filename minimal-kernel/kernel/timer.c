#include <timer.h>
#include <uart.h>

enum
{
  SYS_TIMER = 0x3F003004
};

inline uint32_t get_sys_timer()
{
  return  *(volatile uint32_t*) SYS_TIMER;
}

void wait(uint32_t count)
{
  uint32_t init_val = get_sys_timer();
  uint32_t elapsed = 0;

  //  uart_putc((unsigned char) init_val);
  // uint8_t cmd[] = {0x41, 0x42, 0x7F};
  //uart_puts_asc(cmd, 3);
  while (elapsed <= count) {
    elapsed = get_sys_timer() - init_val;
    //   uart_puts("wooo\n");
  }
}
