#include "uart.h"
#include "timer.h"
#include "system_registers.h"
#include "interrupt_controller.h"
#include "irq_handler.h"

#define CNTP_CTL_ENABLE 0x1

int main(void){
    uart_puts("Hello world from bare metal!\n");

    gic_init();
    write_cntp_tval_el0(read_cntfrq_el0()/10);
    write_cntp_ctl_el0(CNTP_CTL_ENABLE);
    enable_irq();


    for (int i = 0; i < 3; i++){
        delay_ms(1000);
        uart_puts("ticks: 0x");
        uart_puthex(tick_count);
        uart_puts("\n");
    }
    return 0;
}