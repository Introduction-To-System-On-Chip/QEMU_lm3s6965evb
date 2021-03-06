#include "start.h"
#include "mpu_manual.h"
#include "mpu_configure.h"

#include <ARMCM3.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/*
 * Memory mapping with the regions that are accessed in the program.
 * The linker script defines the RAM, ROM addresses
 *
 * 0x20030000U
 *     Region 8 - Size 16KB
 * 0x20028000U
 *     Region 7 - Size 16KB
 * 0x20024000U
 *     Region 6 - Size 16KB
 * 0x20020000U
 *     Region 5 - Size 16KB
 * 0x20018000U
 *     Region 4 - Size 16KB
 * 0x20014000
 *     Region 3 - Size 16KB
 * 0x20010000
 *     Region 2 - Size 32KB
 * 0x20008000
 *     Region 1 - Size 32KB
 * 0x20000000
 *
 * ...
 *
 * 0x00020000
 *     Region 0 - 128KB
 * 0x00000000                                -- ROM
 *
 */
void accessRegionsMPU(bool manualConfiguration)
{
  uint32_t* const addrRegion2 = (uint32_t*) 0x20010000U;
  uint32_t* const addrRegion3 = (uint32_t*) 0x20014000U;
  uint32_t* const addrRegion4 = (uint32_t*) 0x20018000U;
  uint32_t* const addrRegion5 = (uint32_t*) 0x20020000U;
  uint32_t* const addrRegion6 = (uint32_t*) 0x20024000U;
  uint32_t* const addrRegion7 = (uint32_t*) 0x20028000U;

  uint32_t readValueRegion;
  /* Suppress warning as variable is only assigned and never used after */
  (void) readValueRegion;

  /* ARM_MPU_AP_NONE No access */
  *addrRegion2 = 0;
  /* Fault and MPU has been disabled in MemHandler. */
  disableMPU(manualConfiguration);

  readValueRegion = *addrRegion2;
  /* Fault and MPU has been disabled in MemHandler. */
  disableMPU(manualConfiguration);

  /* ARM_MPU_AP_FULL  Full access */
  *addrRegion3 = 0;
  readValueRegion = *addrRegion3;
  /* No fault. No need to enable MPU. */

  /* ARM_MPU_AP_PRO  Privileged Read-only */
  readValueRegion = *addrRegion4;
  /* No fault. No need to enable MPU. */
  *addrRegion4 = 0;
  /* Fault and MPU has been disabled in MemHandler. */
  disableMPU(manualConfiguration);

  /* ARM_MPU_AP_PRIV Privileged Read/Write: privileged access only */
  *addrRegion5 = 0;
  readValueRegion = *addrRegion5;

  /* ARM_MPU_AP_URO  Privileged Read/Write; Unprivileged Read-only */
  *addrRegion6 = 0;
  readValueRegion = *addrRegion6;

  /* ARM_MPU_AP_RO   Privileged and Unprivileged Read-only */
  *addrRegion7 = 0;
  /* Fault and MPU has been disabled in MemHandler. */
  disableMPU(manualConfiguration);

  readValueRegion = *addrRegion7;
  /* No fault. No need to enable MPU. */

  logPrint("Ending priviledge mode. Switching to Usermode\n");

  /* Switch to User Thread Mode!*/
  __set_CONTROL(__get_CONTROL() | CONTROL_nPRIV_Msk);
  /*
   * Make sure all instructions fetched before the change of context are stopped.
   * http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dai0321a/BIHFJCAC.html
   */
  __ASM volatile ("ISB\n");
  *addrRegion7 = 0;

  __ASM volatile ("SVC #8\n");

}

int main(void)
{
  __set_PSP(0x20000000U);

  logPrint("Control 0x%x\n"
           "PSP 0x%x\n"
           "MSP 0x%x\n\n",
            __get_CONTROL(), __get_PSP(), __get_MSP());

  //ManualInitMPU();
  //accessRegionsMPU(true);

  CmsisInitMPU();
  accessRegionsMPU(false);

  return 0;
}

