#ifndef INTERRUPT_CONTROLLER_H
#define INTERRUPT_CONTROLLER_H

void print_gic_registers(void);
void gic_init(void);
void print_gicd_ispendr0(void);

#endif