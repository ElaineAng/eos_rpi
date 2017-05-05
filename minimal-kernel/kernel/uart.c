#include <uart.h>

// Memory mapped I/O output
void mmio_write(unsigned int reg, unsigned int data){

  // volatile indicates that the variable reg may change at any time --
  // without any action being taken by the code nearby
  *(volatile uint32_t * )reg = data;
}

uint32_t mmio_read(uint32_t reg){
  return *(volatile uint32_t*) reg;
}

// Loop <delay> times in a way that the compiler won't optimize away
void delay(int32_t count)
{
  __asm__ volatile("__delay_%=: subs %[count], %[count], #1; bne __delay_%=\n"
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
  
  /* Set integer & fractional part of baud rate.
   * Divider = UART_CLOCK/(16 * Baud)
   * Fraction part register = (Fractional part * 64) + 0.5
   * UART_CLOCK = 50,000,000; Baud for LCD: 9600
   */

  // Divider = 50,000,000 / (16 * 9600) = 325.52 ~ 325
  mmio_write(UART0_IBRD, 325);
  // Fractional part register = (.52 * 64) + 0.5 = 33.833 ~ 33
  mmio_write(UART0_FBRD, 33);

  // Enable FIFO & 8 bit data transmissio (1 stop bit, no parity).
  mmio_write(UART0_LCRH, (1 << 4) | (1 << 5) | (1 << 6));
  
  /* Mask all interrupts.
   * mmio_write(UART0_IMSC, (1 << 1) | (1 << 4) | (1 << 5) | (1 << 6) |
   	     (1 << 7) | (1 << 8) | (1 << 9) | (1 << 10));
   */

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
    uart_putc((unsigned char) str[i]);

}

void uart_put_ascii(uint8_t asc)
{
  while ( mmio_read(UART0_FR) & (1 << 5) ){}
  mmio_write(UART0_DR, asc);
}

void uart_puts_cmd(const uint8_t* na, size_t size)
{
  for (size_t i = 0; i<size; i++){
    uart_put_ascii(na[i]);
  }
}

void uart_write_to_lcd(const char * str)
{
  uart_put_ascii(0xAA);
  uart_put_ascii(0x25);
  uart_puts(str);
  uart_put_ascii(0x0D);
}

void uart_write_char_to_lcd(uint8_t c)
{
  uart_put_ascii(0xAA);
  uart_put_ascii(0x25);
  uart_putc(c);
  uart_put_ascii(0x0D);
}
