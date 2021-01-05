#include "mpu_manual.h"

#include <stdint.h>

void ManualConfigureRegion
(
  uint8_t  regionNumber,
  uint32_t regionBaseAddress,
  uint8_t  regionSize,
  uint8_t  accessPermission,
  uint8_t  executeNever
)
{
  uint32_t* registerAddr;
  uint32_t registerValue = 0;

  registerAddr = MPU_REG_RNR;
  *registerAddr = regionNumber;

  registerAddr = MPU_REG_RBAR;
  *registerAddr = regionBaseAddress;
  logPrint("0x%x has value 0x%x\n", registerAddr, *registerAddr);

  registerAddr = MPU_REG_RLAR;
  registerValue  = regionSize << 1;
  registerValue |= accessPermission << 24;
  registerValue |= executeNever << 28;
  registerValue |= 1;
  *registerAddr = registerValue;

  logPrint("0x%x has value 0x%x\n", registerAddr, *registerAddr);

}
