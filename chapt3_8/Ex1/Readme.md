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
  information evolves.
