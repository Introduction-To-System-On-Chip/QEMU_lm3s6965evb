#include <ARMCM3.h>
#include <stdlib.h>
#include <stdint.h>
#include "mpu_manual.h"

/*
 * 0x20030000U
 *     Region 8 -16KB
 * 0x20028000U
 *     Region 7 -16KB
 * 0x20024000U
 *     Region 6 -16KB
 * 0x20020000U
 *     Region 5 -16KB
 * 0x20018000U
 *     Region 4 -16KB
 * 0x20014000
 *     Region 3 -16KB
 * 0x20010000		Stack start, going down
 *     Region 2 -32KB
 * 0x20008000
 *     Region 1 -32KB
 * 0x20000000           Heap start, going up -- RAM
 *
 * ...
 *
 * 0x00040000
 *
 * 0x00020000
 *     Region 0 - 128KB
 * 0x00000000                                -- ROM
 *
 */

#ifdef MPU_USE_EXIT_HANDLER
void MPUFaultExit(void)
{
  logPrint("MPUFaultExit: exiting...\n");
  exit(0);
  while(1);
}
#endif

void SVC_Handler(void)
{
  logPrint("SVC Call, switching to Priviledge\n");
  __set_CONTROL(__get_CONTROL() & ~CONTROL_nPRIV_Msk);
}

void MemManage_Handler(void)
{
  uint32_t lrValue = 0;

  __ASM volatile ("MOV %0, LR\n" : "=r" (lrValue) );

  logPrint("MemManage_Handler:\n"
           "\tcontrol 0x%x\n"
           "\tmmfar 0x%x\n"
           "\tLR 0x%x\n",
           __get_CONTROL(), SCB->MMFAR, lrValue);

  /*
   * It is possible to return to another address or to skip the faulty
   * instruction. However, skiping the instruction is not adviced as it would
   * break the execution flow.
   * In general, either the current execution is stopped, or the task is killed
   * (if an RTOS is used) or reconfigure the MPU and re-execute the instruction.
   */

#ifdef MPU_USE_EXIT_HANDLER
  /* Return to another address */
  uint32_t mpuAddr = MPUFaultExit;
  __ASM volatile ("MOV LR, %0\n" : "=r" (mpuAddr));
  __ASM volatile ("BX LR\n");
#else
  /* Disable MPU and restart instruction */
  ARM_MPU_Disable();
#endif
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

  ARM_MPU_SetRegionEx(
    4UL                                   /* Region Number */,
    0x20014000UL                          /* Base Address  */,
    ARM_MPU_RASR(1UL                      /* DisableExec */,
                 ARM_MPU_AP_FULL          /* AccessPermission*/,
                 0UL                      /* TypeExtField*/,
                 0UL                      /* IsShareable*/,
                 0UL                      /* IsCacheable*/,
                 0UL                      /* IsBufferable*/,
                 0x00UL                   /* SubRegionDisable*/,
                 ARM_MPU_REGION_SIZE_16KB /* Size*/)
  );

  ARM_MPU_SetRegionEx(
    5UL                                   /* Region Number */,
    0x20018000UL                          /* Base Address  */,
    ARM_MPU_RASR(1UL                      /* DisableExec */,
                 ARM_MPU_AP_PRO           /* AccessPermission*/,
                 0UL                      /* TypeExtField*/,
                 0UL                      /* IsShareable*/,
                 0UL                      /* IsCacheable*/,
                 0UL                      /* IsBufferable*/,
                 0x00UL                   /* SubRegionDisable*/,
                 ARM_MPU_REGION_SIZE_16KB /* Size*/)
  );

  ARM_MPU_SetRegionEx(
    6UL                                   /* Region Number */,
    0x20020000UL                          /* Base Address  */,
    ARM_MPU_RASR(1UL                      /* DisableExec */,
                 ARM_MPU_AP_PRIV          /* AccessPermission*/,
                 0UL                      /* TypeExtField*/,
                 0UL                      /* IsShareable*/,
                 0UL                      /* IsCacheable*/,
                 0UL                      /* IsBufferable*/,
                 0x00UL                   /* SubRegionDisable*/,
                 ARM_MPU_REGION_SIZE_16KB /* Size*/)
  );


  ARM_MPU_SetRegionEx(
    7UL                                   /* Region Number */,
    0x20024000UL                          /* Base Address  */,
    ARM_MPU_RASR(1UL                      /* DisableExec */,
                 ARM_MPU_AP_URO           /* AccessPermission*/,
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

  uint32_t* addrRegion2 = 0x20010000U;
  uint32_t* addrRegion4 = 0x20014000U;
  uint32_t* addrRegion5 = 0x20018000U;
  uint32_t* addrRegion6 = 0x20020000U;
  uint32_t* addrRegion7 = 0x20024000U;
  uint32_t* addrRegion8 = 0x20028000U;
  uint32_t readValueRegion;

  /* ARM_MPU_AP_NONE No access */
  *addrRegion2 = 0;
  /* Fault and MPU has been disabled in MemHandler. */
  ARM_MPU_Enable(MPU_CTRL_PRIVDEFENA_Msk
                 | MPU_CTRL_HFNMIENA_Msk
                 | MPU_CTRL_ENABLE_Msk);
  readValueRegion = *addrRegion2;
  /* Fault and MPU has been disabled in MemHandler. */
  ARM_MPU_Enable(MPU_CTRL_PRIVDEFENA_Msk
                 | MPU_CTRL_HFNMIENA_Msk
                 | MPU_CTRL_ENABLE_Msk);

  /* ARM_MPU_AP_FULL  Full access */
  *addrRegion4 = 0;
  readValueRegion = *addrRegion4;
  /* No fault. No need to enable MPU. */

  /* ARM_MPU_AP_PRO  Privileged Read-only */
  readValueRegion = *addrRegion5;
  /* No fault. No need to enable MPU. */
  *addrRegion5 = 0;
  /* Fault and MPU has been disabled in MemHandler. */
  ARM_MPU_Enable(MPU_CTRL_PRIVDEFENA_Msk
                 | MPU_CTRL_HFNMIENA_Msk
                 | MPU_CTRL_ENABLE_Msk);

  /* ARM_MPU_AP_PRIV Privileged Read/Write: privileged access only */
  *addrRegion6 = 0;
  readValueRegion = *addrRegion6;

  /* ARM_MPU_AP_URO  Privileged Read/Write; Unprivileged Read-only */
  *addrRegion7 = 0;
  readValueRegion = *addrRegion7;


  ARM_MPU_Disable();
  ARM_MPU_Enable(MPU_CTRL_PRIVDEFENA_Msk
  	         | MPU_CTRL_HFNMIENA_Msk
                 | MPU_CTRL_ENABLE_Msk);

  ARM_MPU_SetRegionEx(
    7UL                                   /* Region Number */,
    0x20028000UL                          /* Base Address  */,
    ARM_MPU_RASR(1UL                      /* DisableExec */,
                 ARM_MPU_AP_RO            /* AccessPermission*/,
                 0UL                      /* TypeExtField*/,
                 0UL                      /* IsShareable*/,
                 0UL                      /* IsCacheable*/,
                 0UL                      /* IsBufferable*/,
                 0x00UL                   /* SubRegionDisable*/,
                 ARM_MPU_REGION_SIZE_16KB /* Size*/)
  );

  /* ARM_MPU_AP_RO   Privileged and Unprivileged Read-only */
  *addrRegion8 = 0;
  /* Fault and MPU has been disabled in MemHandler. */
  ARM_MPU_Enable(MPU_CTRL_PRIVDEFENA_Msk
                 | MPU_CTRL_HFNMIENA_Msk
                 | MPU_CTRL_ENABLE_Msk);

  readValueRegion = *addrRegion8;
  /* No fault. No need to enable MPU. */

  logPrint("Ending priviledge mode. Switching to Usermode\n");

  /* Switch to User Thread Mode!*/
  __set_CONTROL(__get_CONTROL() | CONTROL_nPRIV_Msk);
  *addrRegion7 = 0;

  
  __ASM volatile ("SVC #8\n");
}

int main(void)
{
  __set_PSP(0x20000000U);

  logPrint("Control 0x%x\n"
           "PSP 0x%x\n"
           "MSP 0x%x\n",
            __get_CONTROL(), __get_PSP(), __get_MSP());

  //ManualInitMPU();
  CmsisInitMPU();

  return 0;
}

