BINARY := cm3.elf
MACHINE := lm3s6965evb

# Variable with ?= assignement can be redefined when the makefile is called:
# make TOOLCHAIN= QEMU_PATH= will update the variable of the Makefile

CMSIS ?= ./CMSIS_5
# Use QEMU_PATH= if QEMU was installed through the apt-get command.
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
  -machine accel=tcg \
  -d int,cpu_reset

BINARY_OBJDUMP := objdump.txt

CROSS_COMPILE = $(TOOLCHAIN)arm-none-eabi-
CC = $(CROSS_COMPILE)gcc
GDB = $(CROSS_COMPILE)gdb
OBJ = $(CROSS_COMPILE)objdump

LINKER_SCRIPT = gcc_arm.ld

SRC_ASM = $(CMSIS)/Device/ARM/ARMCM3/Source/GCC/startup_ARMCM3.S

SRC_C = $(CMSIS)/Device/ARM/ARMCM3/Source/system_ARMCM3.c \
        start.c

RTX_OS_CMSIS_SRC = $(wildcard $(CMSIS)/CMSIS/RTOS/RTX/SRC/*.c) \
                   $(CMSIS)/CMSIS/RTOS/RTX/Templates/RTX_Conf_CM.c

RTX_SRC_ASM = $(CMSIS)/CMSIS/RTOS/RTX/SRC/GCC/HAL_CM3.S
RTX_SVC_ASM = $(CMSIS)/CMSIS/RTOS/RTX/SRC/GCC/SVC_Table.S

3_8_1_SRC = $(wildcard chapt3_8/Ex1/*.c)
3_8_1_SRC_ASM = chapt3_8/Ex1/startup_ARMCM3.S
3_8_3_SRC = $(wildcard chapt3_8/Ex3/*.c)
3_9_1_SRC = $(wildcard chapt3_9/Ex1/*.c)

INCLUDE_FLAGS = -I$(CMSIS)/Device/ARM/ARMCM3/Include \
  -I$(CMSIS)/CMSIS/Core/Include \
  -I$(CMSIS)/CMSIS/RTOS/RTX/SRC \
  -I$(CMSIS)/CMSIS/RTOS/RTX/INC \
  -I.

CFLAGS = -mcpu=cortex-m3 \
         -specs=nano.specs \
         -specs=nosys.specs \
         -specs=rdimon.specs \
         -Wall \
         -g3 \
         $(INCLUDE_FLAGS) \
         -mthumb \
         -nostartfiles \
         -fdata-sections \
         -ffunction-sections \
         -Wl,--gc-sections \
         -DARMCM3 \
         -D__CORTEX_M3 \
         -D__CMSIS_RTOS

all: 3_8_1

boot.o: $(SRC_ASM)
	$(CC) $(CFLAGS) -c $^ -o $@

rtxsvc.o: $(RTX_SVC_ASM)
	$(CC) $(CFLAGS) -c $^ -o $@

rtxboot.o: $(RTX_SRC_ASM)
	$(CC) $(CFLAGS) -c $^ -o $@

3_8_1_boot.o: $(3_8_1_SRC_ASM)
	$(CC) $(CFLAGS) -c $^ -o $@


3_8_1: $(SRC_C) $(3_8_1_SRC) 3_8_1_boot.o
	$(CC) $^ $(CFLAGS) -T $(LINKER_SCRIPT) -o $(BINARY)
	$(OBJ) -D $(BINARY) > $@_$(BINARY_OBJDUMP)

3_8_3: $(SRC_C) $(RTX_OS_CMSIS_SRC) $(3_8_3_SRC) rtxsvc.o rtxboot.o boot.o
	$(CC) $^ $(CFLAGS) -T $(LINKER_SCRIPT) -o $(BINARY)
	$(OBJ) -D $(BINARY) > $@_$(BINARY_OBJDUMP)

3_9_1: $(SRC_C) $(3_9_1_SRC) boot.o
	$(CC) $^ $(CFLAGS) -T $(LINKER_SCRIPT) -o $(BINARY)
	$(OBJ) -D $(BINARY) > $@_$(BINARY_OBJDUMP)

# Ctrl-A, then X to quit QEMU
run: $(BINARY)
	-$(QEMU_RUN_COMMAND)
	echo $? " has exited"

gdbserver: $(BINARY)
	$(QEMU_RUN_COMMAND) -S -s

help:
	$(QEMU_COMMAND) --machine help

gdb: $(BINARY)
	$(GDB) $(BINARY) -ex "target remote:1234"

clean:
	rm -f $(BINARY_OBJDUMP) *.o *.elf 
