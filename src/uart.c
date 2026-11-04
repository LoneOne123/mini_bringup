#include <stdint.h>

#define UART_DATA_REG (*(volatile uint32_t*)0x09000000)
#define UART_FLAG_REG (*(volatile uint32_t*)0x09000018)
#define UART_FR_TXFF (1 << 5)

void uart_putc(char c){
    if (c == '\n'){
        uart_putc('\r');
    }
    while (UART_FLAG_REG & UART_FR_TXFF){
    }
    UART_DATA_REG = (unsigned char) c;
}

void uart_puts(const char *str){
    while (*str){
        uart_putc(*str);
        str++;
    }
}