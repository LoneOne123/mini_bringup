#ifndef IRQ_HANDLER_H
#define IRQ_HANDLER_H

#include <stdint.h>

extern volatile uint32_t tick_count;

void irq_handler(void);

#endif