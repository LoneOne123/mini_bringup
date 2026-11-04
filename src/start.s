.section ".text.boot"

.global _start

_start:
    ldr x0, =stack_top
    mov sp, x0
    ldr x0, =bss_begin
    ldr x1, =bss_end
_loop:
    cmp x0, x1
    b.hs _loop_end
    str xzr, [x0], #8
    b _loop

_loop_end:
bl main
1:  wfe
    b 1b
