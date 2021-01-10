/*
 * Memory mapping with the regions that are allocated in the program.
 */

#include "start.h"
#include "mpu_manual.h"
#include "mpu_configure.h"

#include <ARMCM3.h>

#ifdef MPU_USE_EXIT_HANDLER
static void MPUFaultExit(void)
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
  uint32_t cfsr = SCB->CFSR;

  __ASM volatile ("MOV %0, LR\n" : "=r" (lrValue) );

  logPrint("MemManage_Handler:\n"
           "\tcontrol 0x%x\n"
           "\tmmfar 0x%x\n"
           "\tLR 0x%x\n",
           __get_CONTROL(), SCB->MMFAR, lrValue);

  if (cfsr & SCB_CFSR_MMARVALID_Msk)
  {
    logPrint("Attempt to access address\n");
  }

  if (cfsr & SCB_CFSR_DACCVIOL_Msk)
  {
    logPrint("Operation not permitted\n");
  }

  if (cfsr & SCB_CFSR_IACCVIOL_Msk)
  {
    logPrint("Non-executable region\n");
  }

  if (cfsr & SCB_CFSR_MSTKERR_Msk)
  {
    logPrint("Stacking error\n");
  }

  /*
   * It is possible to return to another address or to skip the faulty
   * instruction. However, skiping the instruction is not adviced as it would
   * break the execution flow.
   * In general, either the current execution is stopped, or the task is killed
   * (if an RTOS is used) or the MPU is reconfigure and the instruction
   * re-executed.
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
