#include "kprintf.h"
#include "system_registers.h"

void exception_handler(void)
{
    kprintf("\n*** EXCEPTION ***\n");
    kprintf("ESR_EL1 = 0x%lx\n", read_esr_el1());
    kprintf("ELR_EL1 = 0x%lx\n", read_elr_el1());
    kprintf("FAR_EL1 = 0x%lx\n", read_far_el1());

    for (;;)
        __asm__ volatile("wfe");
}