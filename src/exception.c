#include "uart.h"
#include "system_registers.h"

void exception_handler(void)
{
    uart_puts("\n*** EXCEPTION ***\n");

    uart_puts("ESR_EL1: 0x");
    uart_puthex(read_esr_el1());
    uart_puts("\n");

    uart_puts("ELR_EL1: 0x");
    uart_puthex(read_elr_el1());
    uart_puts("\n");

    uart_puts("FAR_EL1: 0x");
    uart_puthex(read_far_el1());
    uart_puts("\n");

    for (;;)
        __asm__ volatile("wfe");
}