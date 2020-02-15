#include <ARMCM3.h>
#include <stdlib.h>
#include <stdint.h>
#include "mpu_manual.h"

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

void configureRegion (
  uint8_t  regionNumber,
  uint32_t regionBaseAddress,
  uint8_t  regionSize,
  uint8_t  accessPermission,
  uint8_t  executeNever
)
{
  char printBuffer[64];
  uint32_t* registerAddr;
  uint32_t registerValue = 0;

  registerAddr = MPU_REG_RNR;
  *registerAddr = regionNumber;

  registerAddr = MPU_REG_RBAR;
  *registerAddr = regionBaseAddress;
  sprintf(printBuffer,
          "0x%x has value 0x%x\n", registerAddr, *registerAddr);
  semihost(SYS_WRITE0, printBuffer);

  registerAddr = MPU_REG_RLAR;
  registerValue  = regionSize << 1;
  registerValue |= accessPermission << 24;
  registerValue |= executeNever << 28;
  registerValue |= 1;
  *registerAddr = registerValue;

  sprintf(printBuffer,
          "0x%x has value 0x%x\n", registerAddr, *registerAddr);
  semihost(SYS_WRITE0, printBuffer);

}

int main(void)
{
  char printBuffer[64];
  uint32_t* registerAddr;

  registerAddr = MPU_REG_TYPE;
  sprintf(printBuffer,
          "0x%x has value 0x%x\n", registerAddr, *registerAddr);
  semihost(SYS_WRITE0, printBuffer);

  registerAddr = MPU_REG_CTRL;
  *registerAddr = 0x0;
  sprintf(printBuffer,
          "0x%x has value 0x%x\n", registerAddr, *registerAddr);
  semihost(SYS_WRITE0, printBuffer);
  

  configureRegion(
    0,
    0,
    0b10000,
    0b11,
    0);

  configureRegion(
    1,
    0x20000000,
    0b01110,
    0b11,
    0);

  configureRegion(
    2,
    0x20008000,
    0b01101,
    0b0,
    0);

  configureRegion(
    3,
    0x2000C000,
    0b01101,
    0b11,
    1);


  registerAddr = MPU_REG_CTRL;
  *registerAddr = 0x7;
  sprintf(printBuffer,
          "0x%x has value 0x%x\n", registerAddr, *registerAddr);
  semihost(SYS_WRITE0, printBuffer);


  uint32_t* addrRegion1 = 0x20000400;
  uint32_t* addrRegion2 = 0x20008000;
  *addrRegion1 = 0;
  *addrRegion2 = 0;

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
