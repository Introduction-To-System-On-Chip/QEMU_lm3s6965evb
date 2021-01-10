#include "start.h"

#include <cmsis_os.h>
#include <stdio.h>

static void ThreadA(void const *argument)
{
  while (1) {
    osDelay(1000);
    osThreadYield();
  }
}

osThreadId tid_ThreadA;
osThreadDef(ThreadA, osPriorityNormal, 1, 0);

static int createThread(void)
{
  tid_ThreadA = osThreadCreate(osThread(ThreadA), NULL);
  if (!tid_ThreadA)
  {
    return -1;
  }

  return 0;
}

int main(void)
{
  // initialize CMSIS-RTOS
  osKernelInitialize();

  if(createThread() == 0)
  {
    // start thread execution
    osKernelStart();
  }

  return 0;
}

