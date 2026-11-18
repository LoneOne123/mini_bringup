#include "uart.h"
#include "timer.h"
#include "system_registers.h"

int main(void){
    uart_puts("Hello world from bare metal!\n");
    //__asm__ volatile("svc #0");

    uart_puts("CNTFRQ_EL0: 0x");
    uart_puthex(read_cntfrq_el0());
    uart_puts("\n");

    for (int i = 0; i < 3; i++) {
        uart_puts("CNTPCT_EL0: 0x");
        uart_puthex(read_cntpct_el0());
        uart_puts("\n");
        delay_ms(1000);
    }
    return 0;
}