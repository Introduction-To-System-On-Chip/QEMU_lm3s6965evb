# MPU Fault Sample

## Description

The goal of this exercise is to understand and fix the faulty instruction.

The project has a single file:
- `chapt3_9/Ex2/main.c`: that tries to access a memory location which fails and
  create a fault when the MPU is enabled.

## Exercise

Go through the source file and fill the gaps where there is a TODO.

- `chapt3_9/Ex2/main.c`: Add the MPU definition so that the program can execute
  with the MPU enabled.

## Program and test

At the root of the project, run `make 3_9_2` and then `make run`. When the MPU
is enabled, all the memory accesses should pass with the memory region
configured.

The program can be debugged, by running `make gdbserver` and `make gdb`.
