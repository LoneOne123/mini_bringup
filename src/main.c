#include "uart.h"
#include "timer.h"
#include "system_registers.h"
#include "interrupt_controller.h"

int main(void){
    uart_puts("Hello world from bare metal!\n");

    gic_init();
    print_GICD_ISPENDR0();
    delay_ms_timer(1000);
    print_gicd_ispendr0();
    return 0;
}