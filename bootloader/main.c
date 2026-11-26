#include "uart.h"
#include "timer.h"
#include "system_registers.h"
#include "irq_handler.h"
#include "kprintf.h"
#include "sha256.h"
#include "payload_info.h"

#define CNTP_CTL_ENABLE 0x1

static int is_payload_valid(void){
    uint8_t computed[32];

    sha256((const void *) PAYLOAD_ADDR, PAYLOAD_SIZE, computed);
    static const uint8_t expected[32] = PAYLOAD_SHA256;
    for (size_t i = 0; i < 32; i++){
        if (expected[i] != computed[i])
            return 0;
    }
    return 1;
}

int main(void){
    kprintf("Hello world from bare metal!\n");
    if (is_payload_valid()){
        void (*entry)(void) = (void (*)(void)) PAYLOAD_ADDR;
        entry();
        return 0;
    }
    kprintf("Payload rejected!\n");
    return 1;
}