#include "irq_handler.h"
#include "system_registers.h"

#define CNTP_CTL_DISABLE 0x0
#define GICC_BASE 0x08010000
#define GICC_IAR (*(volatile uint32_t*)(GICC_BASE + 0xc))
#define GICC_EOIR (*(volatile uint32_t*)(GICC_BASE + 0x10))

#define INTID_MASK 0x3FF
#define INTID_SPURIOUS 1023
#define INTID_TIMER 30

volatile uint32_t tick_count = 0;

void irq_handler(void){
    uint32_t iar = GICC_IAR;
    uint32_t intid = iar & INTID_MASK;

    if (intid == INTID_SPURIOUS){
        return;
    }

    if (iar == INTID_TIMER){
        tick_count++;
        write_cntp_tval_el0(read_cntfrq_el0() / 10);
    }
    GICC_EOIR = iar;
}