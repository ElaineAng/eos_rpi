// stddef has sise_t & NULL
// stdint has intx_t & uintx_t
// these are part of the compiler

#include <stddef.h>
//#include <stdint.h>
#include "uart.h"

// Memory mapped I/O output

static inline void mmio_write(unsigned int reg, unsigned int data){

  // volatile indicates that the variable reg may change at any time --
  // without any action being taken by the code nearby
  *(volatile uint32_t * )reg = data;
}

static inline uint32_t mmio_read(uint32_t reg){
  return *(volatile uint32_t*) reg;
}

// Loop <delay> times in a way that the compiler won't optimize away
static inline void delay(int32_t count)
{
  asm volatile("__delay_%=: subs %[count], %[count], #1; bne __delay_%=\n"
	       : "=r"(count)
	       : [count]"0"(count) 
	       : "cc");
}

void uart_init()
{
  // Disable UART0.
  mmio_write(UART0_CR, 0x00000000);

  // Setup the GPIO pin 14 && 15.
  mmio_write(GPFSEL1, (1 << 14) | (1<<17)); 
  mmio_write(GPFSEL4, (1<<21)); 

  // Disable pull up/down for all GPIO pins & delay for 150 cycles.
  mmio_write(GPPUD, 0x00000000);
  delay(150);
  
  // Disable pull up/down for pin 14,15 & delay for 150 cycles.
  mmio_write(GPPUDCLK0, (1 << 14) | (1 << 15));
  delay(150);
 
  // Write 0 to GPPUDCLK0 to make it take effect.
  mmio_write(GPPUDCLK0, 0x00000000);

  // Clear pending interrupts.
  mmio_write(UART0_ICR, 0x7FF);
  
  // Set integer & fractional part of baud rate.
  // Divider = UART_CLOCK/(16 * Baud)
  // Fraction part register = (Fractional part * 64) + 0.5
  // UART_CLOCK = 3000000; Baud = 115200. Baud for LCD: 9600
  
  // Divider = 3000000 / (16 * 115200) = 1.627 = ~1.
  //	mmio_write(UART0_IBRD, 1);
  mmio_write(UART0_IBRD, 19);
  // Fractional part register = (.627 * 64) + 0.5 = 40.6 = ~40.
  mmio_write(UART0_FBRD, 34);

  // Enable FIFO & 8 bit data transmissio (1 stop bit, no parity).
  mmio_write(UART0_LCRH, (1 << 4) | (1 << 5) | (1 << 6));
  
  // Mask all interrupts.
  //mmio_write(UART0_IMSC, (1 << 1) | (1 << 4) | (1 << 5) | (1 << 6) |
  //	     (1 << 7) | (1 << 8) | (1 << 9) | (1 << 10));

  // Enable UART0, receive & transfer part of UART.
  mmio_write(UART0_CR, (1 << 0) | (1 << 8) | (1 << 9));
 
}

void uart_putc(unsigned char c)
{
  /* Wait for UART to become ready to transmit.
   * The lower 5th bit in UART0_FR is TXFF (transmit FIFO Full)
   * If the FIFO is enabled, the TXFF bit is set when the transmit FIFO is full
   * If the FIFO is disabled, this bit is set when the transmit holding register is full
   */
  while ( mmio_read(UART0_FR) & (1 << 5) ) { }
  mmio_write(UART0_DR, c);
}

unsigned char uart_getc()
{
  /* Wait for UART to have received something.
   * The lower 4th bit in UART0_FR is RXFE (receive FIFO Empty)
   * If the FIFO is disabled, this bit is set when the receive holding register is empty
   * If the FIFO is enabled, this bit is set when the receive FIFO is empty    
   */
  while ( mmio_read(UART0_FR) & (1 << 4) ) { }
  return mmio_read(UART0_DR);
}

void uart_puts(const char* str)
{

  for (size_t i = 0; str[i] != '\0'; i ++)
    uart_putc((unsigned char)str[i]);
   
}

void uart_putAscii(uint8_t asc)
{
  while ( mmio_read(UART0_FR) & (1 << 5) ){}
  mmio_write(UART0_DR, asc);
}

void uart_puts_asc(const uint8_t* na, size_t size)
{
  for (size_t i = 0; i<size; i++)
    uart_putAscii(na[i]);

}

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
  uint8_t cmd[] = {0xAA, 0x25, 0x41, 0x42, 0x43, 0x0D};
 
  // uart_puts("hello!\n");

  
  while (1){
    uart_puts_asc(cmd, 6);
    mmio_write(GPSET1, (1 << 15));  
    delay(0x3F0000);
    uart_puts_asc(cmd, 6);
    delay(0x3F0000);
    mmio_write(GPCLR1, (1 << 15));
    delay(0x3F0000);
    
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
