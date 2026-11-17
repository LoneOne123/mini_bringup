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

void uart_puthex(uint64_t value){
    for (int shift = 60; shift >= 0; shift -= 4) {
        unsigned int nibble = (value >> shift) & 0xF;
        if (nibble < 10)
            uart_putc('0' + nibble);
        else
            uart_putc('a' + (nibble - 10));
    }
}