#ifndef MPU_MANUAL_H
#define MPU_MANUAL_H

#include <stdint.h>

/* Define addresses for the Registers of the MPU. */
#define MPU_REG_RNR  ((uint32_t*) 0xE000ED98)

/* TODO: Update the address for the TYPE register */
#define MPU_REG_TYPE ((uint32_t*) 0xDEADBEEF)
/* TODO: Update the address for the CTRL register */
#define MPU_REG_CTRL ((uint32_t*) 0xDEADBEEF)
/* TODO: Update the address for the RBAR register */
#define MPU_REG_RBAR ((uint32_t*) 0xDEADBEEF)
/* TODO: Update the address for the RLAR register */
#define MPU_REG_RLAR ((uint32_t*) 0xDEADBEEF)

void ManualConfigureRegion
(
  uint8_t  regionNumber,
  uint32_t regionBaseAddress,
  uint8_t  regionSize,
  uint8_t  accessPermission,
  uint8_t  executeNever
);

#endif
