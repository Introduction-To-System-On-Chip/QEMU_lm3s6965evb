# qemu-lm3s6965evb

Start Arm-v7M program using QEMU (https://www.qemu.org/).
Note: This project is based on a Linux machine like Ubuntu. If you want to run
it on Windows, a virtual machine can be used to follow the steps.

Clone this project using the following command:
```
git clone --recurse-submodules https://github.com/pokitoz/qemu-lm3s6965evb.git
```

It will create a folder called `qemu-lm3s6965evb`.

QEMU can be used to emulate Arm based systems for v7M architecture. The one used
in this project is lm3s6965. More information on this device can be found on the
Texas Instrument website . This is a Cortex-M3 based chip (v7M architecture)
that has an MPU.

## Prerequisites

It is necessary to have `build-essential`, `make` and `git`:
```
sudo apt -y install git make build-essential
```

### CMSIS

During the clone of the project, CMSIS is already downloaded and configured
to be built in the Makefile of the project.

The path to CMSIS in the Makefile is stored in variable CMSIS.

### Install QEMU

Before compiling and running programs, you need to install `QEMU`:
- Either using apt from Ubuntu
```
sudo apt -y install qemu-system-arm
```

- Or by building it from source (note: you might need to install
  additional dependencies and the compilation might take a while..):
```
cd qemu
./configure
make -j4
```

QEMU path to the qemu-arm-system is configured in the Makefile through
the variable `QEMU_PATH`.

### Get the Arm Cortex-M toolchain

This project will cross-compile a C application from an intel processor
(like most Laptop, Desktop running on a `x86` or `x86_64` architecture) to
an Arm V7M core.

The toolchain can be found on Arm website:
https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads

The version used for this project is gcc-arm-none-eabi-9-2019-q4-major.
```
wget "https://developer.arm.com/-/media/Files/downloads/gnu-rm/9-2019q4/gcc-arm-none-eabi-9-2019-q4-major-x86_64-linux.tar.bz2"

tar xvf gcc-arm-none-eabi-9-2019-q4-major-x86_64-linux.tar.bz2
```

The Makefile is also configured to get this toolchain through the variable
TOOLCHAIN.

## Compile and run the project

The Makefile is gathering all the compilation flags, source files and includes
to compile the final binary.
The command to be used is `make` which should output something like:

```
$ make

# Build the assembly startup file (first assembly code line called when
# the processor starts. This generates the boot.o binary file
./gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-gcc \
  -mcpu=cortex-m3 \
  -specs=nano.specs \
  -specs=nosys.specs \
  -specs=rdimon.specs \
  -Wall -g3 \
  -I./CMSIS_5/Device/ARM/ARMCM3/Include \
  -I./CMSIS_5/CMSIS/Core/Include \
  -I. \
  -mthumb -nostartfiles \
  -c CMSIS_5/Device/ARM/ARMCM3/Source/GCC/startup_ARMCM3.S -o boot.o

# Compile the .c file and link the boot.o binary. The linker script
# gcc_arm.ld is used by the linker to know where to put the different
# sections of the program and at which address.
# This line creates the cm3.elf program.
./gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-gcc \
  CMSIS_5/Device/ARM/ARMCM3/Source/system_ARMCM3.c \
  start.c \
  main.c \
  boot.o \
  -mcpu=cortex-m3 \
  -specs=nano.specs \
  -specs=nosys.specs \
  -specs=rdimon.specs \
  -Wall -g3 \
  -I./CMSIS_5/Device/ARM/ARMCM3/Include \
  -I./CMSIS_5/CMSIS/Core/Include \
  -I. \
  -mthumb \
  -nostartfiles \
  -T gcc_arm.ld -o cm3.elf

# Dumps the content of the produced elf into the file objdump_cm3.elf
# Can be used for inspection of the code and to analyse the assembly
# code produced and the addresses of the functions.
./gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-objdump \
  -D cm3.elf > objdump_cm3.elf
```

Now that the program is build, it can either be run or debugged. Note the flag
(-g3) indicating that some symbols are put in the final binary to help the
debug process.

### Debug the project

This can be done in two steps:
- Start QEMU with the binary and request a gdbserver in one terminal

```
$ make gdbserver
qemu-system-arm -machine lm3s6965evb -cpu cortex-m3 -m 4096 -nographic -semihosting -device loader,file=cm3.elf -machine accel=tcg -S -s -d int,cpu_reset

CPU Reset (CPU 0)
R00=00000000 R01=00000000 R02=00000000 R03=00000000
R04=00000000 R05=00000000 R06=00000000 R07=00000000
R08=00000000 R09=00000000 R10=00000000 R11=00000000
R12=00000000 R13=00000000 R14=00000000 R15=00000000
XPSR=40000000 -Z-- A priv-thread
CPU Reset (CPU 0)
R00=00000000 R01=00000000 R02=00000000 R03=00000000
R04=00000000 R05=00000000 R06=00000000 R07=00000000
R08=00000000 R09=00000000 R10=00000000 R11=00000000
R12=00000000 R13=00000000 R14=ffffffff R15=00000000
XPSR=40000000 -Z-- A priv-thread
```

In the Makefile, you can remove `-d int,cpu_reset` to suppress the following
traces from QEMU:
```
Taking exception 7 [Breakpoint]
...handling as semihosting call 0x18
```

- Open another terminal and run gdb with the gdbserver as target.

```
$ make gdb
./gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-gdb cm3.elf -ex "target remote:1234"
GNU gdb (GNU Tools for Arm Embedded Processors 9-2019-q4-major) 8.3.0.20190709-git
Copyright (C) 2019 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.
Type "show copying" and "show warranty" for details.
This GDB was configured as "--host=x86_64-linux-gnu --target=arm-none-eabi".
Type "show configuration" for configuration details.
For bug reporting instructions, please see:
<http://www.gnu.org/software/gdb/bugs/>.
Find the GDB manual and other documentation resources online at:
    <http://www.gnu.org/software/gdb/documentation/>.

For help, type "help".
Type "apropos word" to search for commands related to "word"...
Reading symbols from cm3.elf...
Remote debugging using :1234
Reset_Handler () at CMSIS_5/Device/ARM/ARMCM3/Source/GCC/startup_ARMCM3.S:78
78	                bl       SystemInit
(gdb) 
```

Now, typical gdb commands (step, next, continue, break,..) can be used to debug
the program. Pressing CTRL+X + A, it is possible to need the source file with
the line being debugged in highlight.

### Run

If debug is not necessary, simply use `make run` and the program will execute
and stop.
