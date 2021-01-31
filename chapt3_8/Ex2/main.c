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
}

/*
 * TODO: Create the ThreadB function for the B thread that will print a string
 */

osThreadId tid_ThreadA;
osThreadDef(ThreadA, osPriorityHigh, 1, 0);

/*
 * TODO: Create the initialization for Thread B with a lower priority than
 * thread A
 */

static int createThread(void)
{
  tid_ThreadA = osThreadCreate(osThread(ThreadA), NULL);
  if (!tid_ThreadA)
  {
    printString("Could not create thread A\n");
    return -1;
  }

  /* TODO: Create create the Thread B */

  return 0;
}

int main(void)
{
  printString("3_8_2\n");

  // initialize CMSIS-RTOS
  osKernelInitialize();

  if(createThread() == 0)
  {
    // start thread execution
    osKernelStart();
  }

  return 0;
}

