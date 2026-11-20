#ifndef SYSTEM_REGISTERS_H
#define SYSTEM_REGISTERS_H

#include <stdint.h>

static inline uint64_t read_current_el(void) {
    uint64_t cur_el;
    __asm__ volatile("mrs %0, CurrentEl" : "=r"(cur_el));
    return cur_el;
}

static inline uint64_t read_esr_el1(void) {
    uint64_t esr_el1;
    __asm__ volatile("mrs %0, esr_el1" : "=r"(esr_el1));
    return esr_el1;
}

static inline uint64_t read_elr_el1(void) {
    uint64_t elr_el1;
    __asm__ volatile("mrs %0, elr_el1" : "=r"(elr_el1));
    return elr_el1;
}

static inline uint64_t read_far_el1(void) {
    uint64_t far_el1;
    __asm__ volatile("mrs %0, far_el1" : "=r"(far_el1));
    return far_el1;
}

static inline uint64_t read_cntfrq_el0(void) {
    uint64_t cntfrq_el0;
    __asm__ volatile("mrs %0, cntfrq_el0" : "=r"(cntfrq_el0));
    return cntfrq_el0;
}

static inline uint64_t read_cntpct_el0(void) {
    uint64_t cntpct_el0;
    __asm__ volatile("isb\nmrs %0, cntpct_el0" : "=r"(cntpct_el0));
    return cntpct_el0;
}

static inline void write_cntp_tval_el0(uint32_t value){
    __asm__ volatile("msr cntp_tval_el0, %0" : : "r"(value));
}

static inline void write_cntp_ctl_el0(uint64_t value){
    __asm__ volatile("msr cntp_ctl_el0, %0" : : "r"(value));
}

static inline uint64_t read_cntp_ctl_el0(void){
    uint64_t cntp_ctl_el0;
    __asm__ volatile("isb\nmrs %0, cntp_ctl_el0" : "=r"(cntp_ctl_el0));
    return cntp_ctl_el0;
}

static inline void enable_irq(void){
    __asm__ volatile("msr daifclr, #2" : : : "memory");
}

#endif