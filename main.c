#include <ARMCM3.h>
#include <stdlib.h>
#include <stdint.h>

/* Static functions and constant data */
const int SYS_WRITE0 = 0x04;
const int SYS_EXIT = 0x18;
static void semihost(int sys_id, const void *arg)
{
    register int r0 __asm__ ("r0") = sys_id;
    register const void *r1 __asm__ ("r1") = arg;
    __asm__ volatile ("bkpt 0xab");
    (void) r0;
    (void) r1;
}

int main(void)
{
    char printBuffer[32];
    uint32_t* registerAddr;

    semihost(SYS_WRITE0, "Start\n");
    registerAddr = 0xE000ED90;
    sprintf(printBuffer, "%x\n", *registerAddr);
    semihost(SYS_WRITE0, printBuffer);

    registerAddr = 0xE000ED94;
    sprintf(printBuffer, "%x\n", *registerAddr);
    semihost(SYS_WRITE0, printBuffer);

    return 0;
}

void _start(void)
{
    main();
    int success = 0x20026;
    semihost(SYS_EXIT, &success);
    while (1) {
        __NOP();
    }
}
