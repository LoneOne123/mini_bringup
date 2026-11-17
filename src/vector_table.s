.section ".text.vector"

.global vector_table

.balign 2048

vector_table:
// Gruppe 1
.balign 0x80
curr_el_sp0_sync:
    b curr_el_sp0_sync
.balign 0x80
curr_el_sp0_irq:
    b curr_el_sp0_irq
.balign 0x80
curr_el_sp0_fiq:
    b curr_el_sp0_fiq
.balign 0x80
curr_el_sp0_serror:
    b curr_el_sp0_serror

// Gruppe 2
.balign 0x80
curr_el_spx_sync:
    b exception_handler
.balign 0x80
curr_el_spx_irq:
    b curr_el_spx_irq
.balign 0x80
curr_el_spx_fiq:
    b curr_el_spx_fiq
.balign 0x80
curr_el_spx_serror:
    b curr_el_spx_serror

// Gruppe 3
.balign 0x80
lower_el_a64_sync:
    b lower_el_a64_sync
.balign 0x80
lower_el_a64_irq:
    b lower_el_a64_irq
.balign 0x80
lower_el_a64_fiq:
    b lower_el_a64_fiq
.balign 0x80
lower_el_a64_serror:
    b lower_el_a64_serror

// Gruppe 4
.balign 0x80
lower_el_a32_sync:
    b lower_el_a32_sync
.balign 0x80
lower_el_a32_irq:
    b lower_el_a32_irq
.balign 0x80
lower_el_a32_fiq:
    b lower_el_a32_fiq
.balign 0x80
lower_el_a32_serror:
    b lower_el_a32_serror

// sauber ausrichten
.balign 0x80