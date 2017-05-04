#include <uart.h>
#include <timer.h>
#include <types.h>

#if defined(__cplusplus)
extern "C" /* Use C linkage for kernel_main. */
#endif

struct proc
{
  uint8_t id;
  uint32_t exp;
  uint32_t cur;
  uint8_t content;
  uint8_t init_c;
  uint8_t fin;
  uint8_t run;
};


void clr_lcd()
{
  uint8_t clr[] = {0xAA, 0x10};
  uart_puts_cmd (clr, 2);
}

void set_cur_1(){
  uint8_t rtn[] = {0xAA, 0x20, 0, 0};
  uart_puts_cmd(rtn, 4);
}

void set_cur_2(){
  uint8_t rtn[] = {0xAA, 0x20, 1, 0};
  uart_puts_cmd(rtn, 4);
}

void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags)
{
  // Declare as unused
  (void) r0;
  (void) r1;
  (void) atags;
  
  uart_init();

  uint16_t cc = 0;
  int cur = 0;  // default starting from 0
  int fin_num = 0;
  uint32_t cycle = 20;

  struct proc p1, p2, p3;
  int proc_num = 3;

  /* Initialize */
  p1.id = 0x31; 
  p1.exp = cycle * 1;
  p1.cur = 0;
  p1.content = 0x61; // from a to j
  p1.init_c = 0x61;
  p1.fin = 0;
  p1.run = 1;

  p2.id = 0x32;
  p2.exp = cycle * 2;
  p2.cur = 0;
  p2.content = 0x41; // from A to J 
  p2.init_c = 0x41; 
  p2.fin = 0;
  p2.run = 1;

  p3.id = 0x33;
  p3.exp = cycle * 6;
  p3.cur = 0;
  p3.content = 0x30; // from 0 to 9
  p3.init_c = 0x30;
  p3.fin = 0;
  p3.run = 1;

  struct proc all_procs[] = {p1, p2, p3};
  struct proc * cp;
  /* Displaying Starting info */
  uart_write_to_lcd("Hello World!");
  delay(0x3F0000);
  set_cur_2();
  uart_write_to_lcd("...from kernel");
  delay(0x3F0000);
  clr_lcd();

  /*
  uart_write_char_to_lcd(all_procs[0].content+0x1);
  uart_write_char_to_lcd(all_procs[1].content+0x1);
  uart_write_char_to_lcd(all_procs[2].content+0x1);
  */
 
  /* Main loop */

  while (1) {
    if (cur >= proc_num){
      if (fin_num >= proc_num){
	clr_lcd();
	break;
      } else {
	cur = 0;
      }
    }

    cp = & (all_procs[cur]);
    if (! cp -> fin){
      cp->run = 1;    
      
      //           uart_putc(cp->id);
      //      uart_putc(cp->content);
      //	    uart_putc('\n');
      // Clear screen and display info
      clr_lcd();
      uart_write_char_to_lcd(cp->id);
      uart_write_to_lcd(": ");
      uart_write_char_to_lcd(cp->content);
      delay(0x1F000);
      
      // Incrementing necessary counters
      cp->content += 0x1;
      cc += 1;
      cp->cur += 1;
      
      // Decide if the process finishes
      if (cp->cur >= cp->exp){
	cp->fin = 1;
	fin_num += 1;
      }

      // Reset necessary counters
      if (cc >= 10){
	cp->content = cp->init_c;
	cc = 0;
      }
      
      if (cp->cur >= cycle){
	
	cp->exp -= cp->cur;
	cp->cur = 0;
	cp->run = 0;
	cur++;
      }
    } else{
      cur++;
    }
  }

  uart_write_to_lcd("All done!");  
  
  while(1){
    
  }

}

