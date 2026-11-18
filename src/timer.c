#include "timer.h"
#include "system_registers.h"

void delay_ms(uint64_t ms){
    uint64_t frequence = read_cntfrq_el0();
    uint64_t start = read_cntpct_el0();
    uint64_t ticks_to_wait = (ms * frequence) / 1000;

    while (read_cntpct_el0() - start < ticks_to_wait){
        __asm__("nop");
    }
}