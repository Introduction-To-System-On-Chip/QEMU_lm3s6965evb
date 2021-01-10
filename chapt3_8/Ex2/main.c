#include "start.h"

#include <ARMCM3.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

int main(void)
{
  __set_PSP(0x20000000U);

  logPrint("Control 0x%x\n"
           "PSP 0x%x\n"
           "MSP 0x%x\n\n",
            __get_CONTROL(), __get_PSP(), __get_MSP());


  return 0;
}

