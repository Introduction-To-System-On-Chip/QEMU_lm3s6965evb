#include "start.h"
#include "uart.h"

#include <cmsis_os.h>
#include <stdio.h>
#include <stdlib.h>

static void ThreadA(void const *argument)
{
  int counter = 100;

  while (counter != 0) {
    printString("Hello, I am the running thread A\n");

    osDelay(1);
    counter--;
  }

  osDelay(1000);
  exit(0);
}

static void ThreadB(void const *argument)
{
  int counter = 100;

  while (counter != 0) {
    printString("Hello, I am the running thread B\n");
    counter--;
  }

  osDelay(1000);
  exit(0);
}

osThreadId tid_ThreadA;
osThreadDef(ThreadA, osPriorityHigh, 1, 0);
osThreadId tid_ThreadB;
osThreadDef(ThreadB, osPriorityNormal, 1, 0);

static int createThread(void)
{
  tid_ThreadA = osThreadCreate(osThread(ThreadA), NULL);
  if (!tid_ThreadA)
  {
    printString("Could not create thread A\n");
    return -1;
  }

  tid_ThreadB = osThreadCreate(osThread(ThreadB), NULL);
  if (!tid_ThreadB)
  {
    printString("Could not create thread B\n");
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

