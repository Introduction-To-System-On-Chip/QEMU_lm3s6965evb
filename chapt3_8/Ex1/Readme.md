# Vector Table

## Program and decompile

At the root of the project, run `make 3_8_1` and then `make run`. It should
produce the decompiled file `3_8_1_objdump.txt` from the binary `cm3.elf`.

# Instructions

Compile and disassemble the code:
- Look at the address of the vector table.
  Why do the disassembled instructions look strange in the vector table?
- Have a look at the address given to the handlers and check if they match the
  expected handle.
- Update the table and add define additional handlers to see how the disassembly
  information evolves. See the assembly source file `startup_ARMCM3.S`.
- What happens if the address of the vector table is updated to another? Use gdb
  to debug the application step by step and append `-d int,cpu_reset` to the
  `QEMU_RUN_COMMAND` variable in the `../../Makefile`.
  Hint: this can be done in the linker script `../../gcc_arm.ld`. The `.text`
  section starts at the ORIGIN address of the FLASH Memory.
