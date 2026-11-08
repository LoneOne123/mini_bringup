#include "uart.h"

int main(void){
    uart_puts("Hello world from bare metal!\n");
    __asm__ volatile("svc #0");
    return 0;
}