# MPU Region

## Description

The purpose of this exercise is to setup some MPU regions in two different ways:
- by manually writting into the MPU registers
- by using the CMSIS API functions

The project has multiple files:
- `chapt3_9/Ex1/handler_definitions.c`: Redefine the `MemManage_Handler` to
  print some logs on such errors and `SVC_Handler` to switch to from privileged
  mode to user mode.
- `chapt3_9/Ex1/main.c`: Used as main entry point to call the functions for
  configuring and enabling the MPU, then making some accesses.
- `chapt3_9/Ex1/mpu_configure.c`: contains the functions used for configuring
  the MPU and the different regions that will be accessed.
- `chapt3_9/Ex1/mpu_configure.h`: Header file for the previous source file.
- `chapt3_9/Ex1/mpu_manual.c`: Contains the prototype of the function to be used
  when the MPU is programmed "manually", i.e. by writting into the registers
- `chapt3_9/Ex1/mpu_manual.h`: Contains the list of the registers to be written
  to.

## Exercise

Go through the different source files and fill the gaps where there is a TODO.

- `chapt3_9/Ex1/main.c`: In the main, comment or uncomment the lines depending
  on which mode (Manual or CMSIS) the program has to run.
- `chapt3_9/Ex1/mpu_configure.c`: Complete the functions for disabling the MPU,
  configuring the regions and enabling the MPU.
- `chapt3_9/Ex1/mpu_manual.c`: Fill in the function to
- `chapt3_9/Ex1/mpu_manual.h`: Update the register defines (set to 0xDEADBEEF)
  to the correct address (the `MPU_REG_RNR` is already correct).
- Instead of using `ARM_MPU_SetRegion` function, use `ARM_MPU_Load` and create
  a table of `MPU_Region_t` containing the definition of all the regions to be
  configured.

## Program and test

At the root of the project, run `make 3_9_1` and then `make run`. If the MPU is
enabled, all the memory accesses should pass with the memory region configured.

The program can be debugged, by running `make gdbserver` and `make gdb`.
