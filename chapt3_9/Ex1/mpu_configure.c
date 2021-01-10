#include "start.h"
#include "mpu_manual.h"

#include <ARMCM3.h>

#include <stdbool.h>
#include <stdint.h>

void disableMPU(bool manualConfiguration)
{
  if (manualConfiguration)
  {
    /* TODO: Fill with custom code to disable the MPU */
  }
  else
  {
    /* TODO: Fill with CMSIS function to disable the MPU */
  }
}

void ManualInitMPU(void)
{
  disableMPU(true);

  /*
   * TODO: Fill with written function ManualConfigureRegion to enable regions
   *       0 to 7 the MPU
   */

  /* TODO: Write in corresponding register to enable the MPU */
}

void CmsisInitMPU(void)
{
  disableMPU(false);

  /*
   * TODO: Fill with CMSIS function ARM_MPU_SetRegionEx to enable regions
   *       0 to 7 the MPU
   */

  /* TODO: Fill with CMSIS function to enable the MPU */
}
