#include "start.h"
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
  /*
   * TODO: Use the defines in the mpu_manual.h file to write into corresponding 
   *       registers to setup a region of the MPU depending on the given
   *       parameters.
   */

  uint32_t* registerAddr;

  registerAddr = MPU_REG_RNR;
  *registerAddr = regionNumber;

  registerAddr = MPU_REG_RBAR;
  /* TODO: Fill the fields of the register RBAR */
  logPrint("0x%x has value 0x%x\n", registerAddr, *registerAddr);

  registerAddr = MPU_REG_RLAR;
  /* TODO: Fill the fields of the register RLAR */

  logPrint("0x%x has value 0x%x\n", registerAddr, *registerAddr);

}
