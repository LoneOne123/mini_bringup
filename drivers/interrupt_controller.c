#include "interrupt_controller.h"
#include "uart.h"

#define GICD_BASE 0x08000000
#define GICD_CTLR (*(volatile uint32_t*)GICD_BASE)
#define GICD_TYPER (*(volatile uint32_t*)(GICD_BASE + 0x4))
#define GICD_IIDR (*(volatile uint32_t*)(GICD_BASE + 0x8))
#define GICD_ISENABLER0 (*(volatile uint32_t*)(GICD_BASE + 0x100))
#define GICD_ISPENDR0 (*(volatile uint32_t*)(GICD_BASE + 0x200))

#define GICC_BASE 0x08010000
#define GICC_CTLR (*(volatile uint32_t*)GICC_BASE)
#define GICC_PMR (*(volatile uint32_t*)(GICC_BASE + 0x4))
#define GICC_IIDR (*(volatile uint32_t*)(GICC_BASE + 0xFC))


void print_gic_registers(void){
    uart_puts("GICD_CTLR: 0x");
    uart_puthex(GICD_CTLR);
    uart_putc('\n');
    uart_puts("GICD_TYPER: 0x");
    uart_puthex(GICD_TYPER);
    uart_putc('\n');
    uart_puts("GICD_IIDR: 0x");
    uart_puthex(GICD_IIDR);
    uart_putc('\n');
    uart_putc('\n');
    uart_puts("GICC_CTLR: 0x");
    uart_puthex(GICC_CTLR);
    uart_putc('\n');
    uart_puts("GICC_PMR: 0x");
    uart_puthex(GICC_PMR);
    uart_putc('\n');
    uart_puts("GICC_IIDR: 0x");
    uart_puthex(GICC_IIDR);
    uart_putc('\n');
}

void gic_init(void){
    GICC_PMR = 0xff;
    GICD_ISENABLER0 = (1u << 30);
    GICC_CTLR = 0x1;
    GICD_CTLR = 0x1;
}

void print_gicd_ispendr0(void){
    uart_puts("GICD_ISPENDR0: 0x");
    uart_puthex(GICD_ISPENDR0);
    uart_putc('\n');
}