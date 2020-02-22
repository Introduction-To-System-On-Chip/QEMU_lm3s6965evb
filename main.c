#include <ARMCM3.h>
#include <stdlib.h>
#include <stdint.h>
#include "mpu_manual.h"

void MPUFaultExit()
{
  logPrint("MPUFaultExit\n");
  exit(0);
  while(1);
}

/*
 *
 * 0x20010000		RAM (Stack start, going down)
 *
 * 0x20014000
 *     Region 3 -16KB
 * 0x20010000
 *     Region 2 -32KB
 * 0x20008000
 *     Region 1 -32KB
 * 0x20000000
 *
 *
 *
 * 0x00040000		ROM
 *
 * 0x00020000
 * 	Region 0 - 128KB
 * 0x00000000
 *
 */

void MemManage_Handler(void)
{
  uint32_t lrValue = 0;

  __ASM volatile ("MOV %0, LR\n" : "=r" (lrValue) );
  logPrint("LR has value 0x%x\n", lrValue);
  /* After the return, the faulty instruction will be executed again */

  uint32_t mpuAddr = MPUFaultExit;
  __ASM volatile ("MOV LR, %0\n" : "=r" (mpuAddr) );
  __ASM volatile ("MOV %0, LR\n" : "=r" (lrValue) );
  __ASM volatile ("BX LR\n");

  /*
   * It is possible to return to another address or to skip the faulty
   * instruction. However, skiping the instruction is not adviced as it would
   * break the execution flow.
   * In general, either the current execution is stopped, or the task is killed
   * (if an RTOS is used) or reconfigure the MPU and re-execute the instruction.
   */

}

void configureRegion (
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

void ManualInitMPU(void)
{
  char printBuffer[64];
  uint32_t* registerAddr;

  registerAddr = MPU_REG_TYPE;
  logPrint("0x%x has value 0x%x\n", registerAddr, *registerAddr);

  registerAddr = MPU_REG_CTRL;
  *registerAddr = 0x0;
  logPrint("0x%x has value 0x%x\n", registerAddr, *registerAddr);

  configureRegion(
    0,
    0,
    0b10000 /* 128KB */,
    0b11,
    0);

  configureRegion(
    1,
    0x20000000,
    0b01110 /* 32KB */,
    0b11,
    0);

  configureRegion(
    2,
    0x20008000,
    0b01110 /* 32KB */,
    0b0,
    0);

  configureRegion(
    3,
    0x2000C000,
    0b01101 /* 16KB */,
    0b11,
    1);


  registerAddr = MPU_REG_CTRL;
  *registerAddr = 0x7;
  logPrint("0x%x has value 0x%x\n", registerAddr, *registerAddr);
}

void CmsisInitMPU(void)
{
  ARM_MPU_Disable();

  ARM_MPU_SetRegionEx(
    0UL                                    /* Region Number */,
    0x00000000UL                           /* Base Address  */,
    ARM_MPU_RASR(0UL                       /* DisableExec */,
                 ARM_MPU_AP_FULL           /* AccessPermission*/,
                 0UL                       /* TypeExtField*/,
                 0UL                       /* IsShareable*/,
                 0UL                       /* IsCacheable*/,
                 0UL                       /* IsBufferable*/,
                 0x00UL                    /* SubRegionDisable*/,
                 ARM_MPU_REGION_SIZE_128KB /* Size*/)
    );

  ARM_MPU_SetRegionEx(
    1UL                                   /* Region Number */,
    0x20000000UL                          /* Base Address  */,
    ARM_MPU_RASR(0UL                      /* DisableExec */,
                 ARM_MPU_AP_FULL          /* AccessPermission*/,
                 0UL                      /* TypeExtField*/,
                 0UL                      /* IsShareable*/,
                 0UL                      /* IsCacheable*/,
                 0UL                      /* IsBufferable*/,
                 0x00UL                   /* SubRegionDisable*/,
                 ARM_MPU_REGION_SIZE_32KB /* Size*/)
    );

  ARM_MPU_SetRegionEx(
    2UL                                   /* Region Number */,
    0x20008000UL                          /* Base Address  */,
    ARM_MPU_RASR(0UL                      /* DisableExec */,
                 ARM_MPU_AP_FULL          /* AccessPermission*/,
                 0UL                      /* TypeExtField*/,
                 0UL                      /* IsShareable*/,
                 0UL                      /* IsCacheable*/,
                 0UL                      /* IsBufferable*/,
                 0x00UL                   /* SubRegionDisable*/,
                 ARM_MPU_REGION_SIZE_32KB /* Size*/)
    );

  ARM_MPU_SetRegionEx(
    3UL                                   /* Region Number */,
    0x20010000UL                          /* Base Address  */,
    ARM_MPU_RASR(1UL                      /* DisableExec */,
                 ARM_MPU_AP_NONE          /* AccessPermission*/,
                 0UL                      /* TypeExtField*/,
                 0UL                      /* IsShareable*/,
                 0UL                      /* IsCacheable*/,
                 0UL                      /* IsBufferable*/,
                 0x00UL                   /* SubRegionDisable*/,
                 ARM_MPU_REGION_SIZE_16KB /* Size*/)
    );

  ARM_MPU_Enable(MPU_CTRL_PRIVDEFENA_Msk
                 | MPU_CTRL_HFNMIENA_Msk
                 | MPU_CTRL_ENABLE_Msk);

  uint32_t* addrRegion1 = 0x20000400;
  uint32_t* addrRegion2 = 0x20010000;
  *addrRegion1 = 0;
  *addrRegion2 = 0;
}

int main(void)
{
  //ManualInitMPU();
  CmsisInitMPU();

  return 0;
}

