#include "kprintf.h"

int payload_main(void){
    kprintf("Payload running at 0x%lx\n", (unsigned long) &payload_main);
    return 0;
}