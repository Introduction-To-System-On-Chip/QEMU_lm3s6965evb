BINARY := cm3.elf
MACHINE := lm3s6965evb

# Variable with ?= assignement can be redefined when the makefile is called:
# make QEMU_PATH=qemu-system-arm will update the variable of the Makefile
CMSIS ?= ./CMSIS_5
# Use QEMU_PATH = qemu-system-arm if QEMU was installed through the apt command.
QEMU_PATH ?= ./qemu/arm-softmmu/
TOOLCHAIN ?= ./gcc-arm-none-eabi-9-2019-q4-major/bin/

QEMU_COMMAND := $(QEMU_PATH)qemu-system-arm

QEMU_RUN_COMMAND := $(QEMU_COMMAND) \
	-machine $(MACHINE) \
	-cpu cortex-m3 \
	-m 4096 \
	-nographic \
	-semihosting \
	-device loader,file=$(BINARY) \
	-machine accel=tcg

BINARY_OBJDUMP := objdump.txt

CROSS_COMPILE = $(TOOLCHAIN)arm-none-eabi-
CC = $(CROSS_COMPILE)gcc
GDB = $(CROSS_COMPILE)gdb
OBJ = $(CROSS_COMPILE)objdump

LINKER_SCRIPT = gcc_arm.ld

SRC_ASM = $(CMSIS)/Device/ARM/ARMCM3/Source/GCC/startup_ARMCM3.S

SRC_C = $(CMSIS)/Device/ARM/ARMCM3/Source/system_ARMCM3.c \
	start.c

3_9_1_SRC = $(wildcard chapt3_9/Ex1/*.c)

INCLUDE_FLAGS = -I$(CMSIS)/Device/ARM/ARMCM3/Include \
	-I$(CMSIS)/CMSIS/Core/Include \
	-I. \
	$(INCLUDE_FLAGS_APP)

CFLAGS = -mcpu=cortex-m3 \
         -specs=nano.specs \
         -specs=nosys.specs \
         -specs=rdimon.specs \
         -Wall \
	       -g3 \
	       $(INCLUDE_FLAGS) \
	       -mthumb \
	       -nostartfiles

all: base

boot.o: $(SRC_ASM)
	$(CC) $(CFLAGS) -c $^ -o $@

base: $(SRC_C) main.c boot.o
	$(CC) $^ $(CFLAGS) -T $(LINKER_SCRIPT) -o $(BINARY)
	$(OBJ) -D $(BINARY) > $@_$(BINARY_OBJDUMP)

3_8_1: $(SRC_C) main.c boot.o
	$(CC) $^ $(CFLAGS) -T $(LINKER_SCRIPT) -o $(BINARY)
	$(OBJ) -D $(BINARY) > $@_$(BINARY_OBJDUMP)

3_9_1: $(SRC_C) $(3_9_1_SRC) boot.o
	$(CC) $^ $(CFLAGS) -T $(LINKER_SCRIPT) -o $(BINARY)
	$(OBJ) -D $(BINARY) > $@_$(BINARY_OBJDUMP)

# Ctrl-A, then X to quit QEMU
run: $(BINARY)
	-$(QEMU_RUN_COMMAND) -d int,cpu_reset
	echo $? " has exited"

gdbserver: $(BINARY)
	$(QEMU_RUN_COMMAND) -S -s -d int,cpu_reset

help:
	$(QEMU_COMMAND) --machine help

gdb: $(BINARY)
	$(GDB) $(BINARY) -ex "target remote:1234"

clean:
	rm -f $(BINARY_OBJDUMP) *.o *.elf 
