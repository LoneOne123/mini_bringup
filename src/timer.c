#include "timer.h"
#include "system_registers.h"

#define CNTP_CTL_ISTATUS 0x4
#define CNTP_CTL_ENABLE 0x1

void delay_ms(uint64_t ms){
    uint64_t frequence = read_cntfrq_el0();
    uint64_t start = read_cntpct_el0();
    uint64_t ticks_to_wait = (ms * frequence) / 1000;

    while (read_cntpct_el0() - start < ticks_to_wait){
        __asm__("nop");
    }
}

void delay_ms_timer(uint64_t ms){
    uint64_t frequency = read_cntfrq_el0();
    uint32_t ticks_to_wait = (ms * frequency) / 1000;

    write_cntp_tval_el0(ticks_to_wait);
    write_cntp_ctl_el0(CNTP_CTL_ENABLE);
    while (!(read_cntp_ctl_el0() & CNTP_CTL_ISTATUS)){
    }
}