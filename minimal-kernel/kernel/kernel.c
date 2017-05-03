// stddef has sise_t & NULL
// stdint has intx_t & uintx_t
// these are part of the compiler

//#include <stddef.h>
//#include <stdint.h>
#include <uart.h>
#include <timer.h>

#if defined(__cplusplus)
extern "C" /* Use C linkage for kernel_main. */
#endif
void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags)
{
  // Declare as unused
  (void) r0;
  (void) r1;
  (void) atags;
  
  uart_init();
  //  uint8_t cmd[] = {0xAA, 0x25, 0x41, 0x42, 0x43, 0x0D};
 
  uart_puts("hello!\n");

  
  while (1){
    //uart_puts_asc(cmd, 6);
    //    mmio_write(GPSET1, (1 << 15));  
    //uart_puts_asc(cmd, 6);
    uart_puts("hmmmmm\n");
    wait(250000);
    uart_puts("yeaaaahh\n");
    //mmio_write(GPCLR1, (1 << 15));
    wait(250000);
    
  }
  //uart_puts_asc(cmd, 4);
  
  /*
  //	uart_puts("Hello World!\n> ");
  uart_puts("\nWelcome to the minimal kernel!\n");
  uart_puts("Now the only supported command is 'info'\n> ");
  
  char command[10] = "info";
  char rec[10];
  uint32_t i = 0;
  char cur;
  uint32_t eq = 1;
  while (1){
    cur = uart_getc();
    uart_putc(cur);
    if ((i < 10) && (cur != '\r')){
      rec[i] = cur;
      i++;
    } else{
      
      for (size_t j=0; j<i; j++){
	if (command[j] != rec[j]){
	  eq = 0;
	}
      }
      if (eq == 1){
	uart_puts("\n Raspberry Pi 2 Model B\n> ");
      }else{
	eq = 1;
	uart_puts("\nCommand not found\n> ");
      }
      
      i = 0;
    }
  }
  */
}
