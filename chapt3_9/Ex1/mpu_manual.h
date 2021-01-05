#ifndef MPU_MANUAL_H
#define MPU_MANUAL_H

#include <stdint.h>

/* Define addresses for the Registers of the MPU. */
#define MPU_REG_TYPE ((uint32_t*) 0xE000ED90)
#define MPU_REG_CTRL ((uint32_t*) 0xE000ED94)
#define MPU_REG_RNR  ((uint32_t*) 0xE000ED98)
#define MPU_REG_RBAR ((uint32_t*) 0xE000ED9C)
#define MPU_REG_RLAR ((uint32_t*) 0xE000EDA0)

void ManualConfigureRegion
(
  uint8_t  regionNumber,
  uint32_t regionBaseAddress,
  uint8_t  regionSize,
  uint8_t  accessPermission,
  uint8_t  executeNever
);

#endif
