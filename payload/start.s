.section ".text.boot"

.global start

start:
    ldr x0, =stack_top
    mov sp, x0
    ldr x0, =bss_begin
    ldr x1, =bss_end
loop:
    cmp x0, x1
    b.hs loop_end
    str xzr, [x0], #8
b loop

loop_end:
    bl payload_main
1:  wfe
    b 1b
