#include "start.h"

#include <cmsis_os.h>
#include <stdio.h>

static void ThreadA(void const *argument)
{
  int counter = 10;

  while (counter != 0) {
    printString("Thread A\n");

    osDelay(1000);
    osThreadYield();
    counter--;
  }
}

osThreadId tid_ThreadA;
osThreadDef(ThreadA, osPriorityNormal, 1, 0);

static int createThread(void)
{
  tid_ThreadA = osThreadCreate(osThread(ThreadA), NULL);
  if (!tid_ThreadA)
  {
    printString("Could not create thread A\n");
    return -1;
  }

  return 0;
}

int main(void)
{
  printString("3_8_3\n");

  // initialize CMSIS-RTOS
  osKernelInitialize();

  if(createThread() == 0)
  {
    // start thread execution
    osKernelStart();
  }

  return 0;
}

