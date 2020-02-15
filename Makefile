BINARY := cm3.elf
MACHINE := lm3s6965evb

CMSIS ?= ./cmsis
QEMU_PATH ?= ./qemu/arm-softmmu/qemu-system-arm
TOOLCHAIN ?= ./gcc/bin

QEMU_COMMAND := $(QEMU_PATH) \
	-machine $(MACHINE) \
	-cpu cortex-m3 \
	-m 4096 \
	-nographic \
	-semihosting \
	-device loader,file=$(BINARY) \
	-machine accel=tcg

BINARY_OBJDUMP := objdump_$(BINARY)

CROSS_COMPILE = $(TOOLCHAIN)/arm-none-eabi-
CC = $(CROSS_COMPILE)gcc
GDB = $(CROSS_COMPILE)gdb
OBJ = $(CROSS_COMPILE)objdump

LINKER_SCRIPT = gcc_arm.ld
CMSIS_NN := $(CMSIS)/CMSIS/NN
CMSIS_DSP := $(CMSIS)/CMSIS/DSP

SRC_ASM = $(CMSIS)/Device/ARM/ARMCM3/Source/GCC/startup_ARMCM3.S

SRC_C = $(CMSIS)/Device/ARM/ARMCM3/Source/system_ARMCM3.c \
	start.c

SRC_NN = \
$(CMSIS_NN)/Source/ActivationFunctions/arm_nn_activations_q7.c \
$(CMSIS_NN)/Source/ActivationFunctions/arm_nn_activations_q15.c \
$(CMSIS_NN)/Source/ActivationFunctions/arm_relu6_s8.c \
$(CMSIS_NN)/Source/ActivationFunctions/arm_relu_q7.c \
$(CMSIS_NN)/Source/ActivationFunctions/arm_relu_q15.c \
 \
$(CMSIS_NN)/Source/BasicMathFunctions/arm_elementwise_add_s8.c \
$(CMSIS_NN)/Source/BasicMathFunctions/arm_elementwise_mul_s8.c \
 \
$(CMSIS_NN)/Source/ConcatenationFunctions/arm_concatenation_s8_w.c \
$(CMSIS_NN)/Source/ConcatenationFunctions/arm_concatenation_s8_x.c \
$(CMSIS_NN)/Source/ConcatenationFunctions/arm_concatenation_s8_y.c \
$(CMSIS_NN)/Source/ConcatenationFunctions/arm_concatenation_s8_z.c \
 \
$(CMSIS_NN)/Source/ConvolutionFunctions/arm_convolve_1x1_HWC_q7_fast_nonsquare.c \
$(CMSIS_NN)/Source/ConvolutionFunctions/arm_convolve_1x1_s8_fast.c \
$(CMSIS_NN)/Source/ConvolutionFunctions/arm_convolve_HWC_q7_basic.c \
$(CMSIS_NN)/Source/ConvolutionFunctions/arm_convolve_HWC_q7_basic_nonsquare.c \
$(CMSIS_NN)/Source/ConvolutionFunctions/arm_convolve_HWC_q7_fast.c \
$(CMSIS_NN)/Source/ConvolutionFunctions/arm_convolve_HWC_q7_fast_nonsquare.c \
$(CMSIS_NN)/Source/ConvolutionFunctions/arm_convolve_HWC_q7_RGB.c \
$(CMSIS_NN)/Source/ConvolutionFunctions/arm_convolve_HWC_q15_basic.c \
$(CMSIS_NN)/Source/ConvolutionFunctions/arm_convolve_HWC_q15_fast.c \
$(CMSIS_NN)/Source/ConvolutionFunctions/arm_convolve_HWC_q15_fast_nonsquare.c \
$(CMSIS_NN)/Source/ConvolutionFunctions/arm_convolve_s8.c \
$(CMSIS_NN)/Source/ConvolutionFunctions/arm_depthwise_conv_s8.c \
$(CMSIS_NN)/Source/ConvolutionFunctions/arm_depthwise_conv_s8_opt.c \
$(CMSIS_NN)/Source/ConvolutionFunctions/arm_depthwise_conv_u8_basic_ver1.c \
$(CMSIS_NN)/Source/ConvolutionFunctions/arm_depthwise_separable_conv_HWC_q7.c \
$(CMSIS_NN)/Source/ConvolutionFunctions/arm_depthwise_separable_conv_HWC_q7_nonsquare.c \
$(CMSIS_NN)/Source/ConvolutionFunctions/arm_nn_depthwise_conv_s8_core.c \
$(CMSIS_NN)/Source/ConvolutionFunctions/arm_nn_mat_mult_kernel_q7_q15.c \
$(CMSIS_NN)/Source/ConvolutionFunctions/arm_nn_mat_mult_kernel_q7_q15_reordered.c \
$(CMSIS_NN)/Source/ConvolutionFunctions/arm_nn_mat_mult_kernel_s8_s16.c \
$(CMSIS_NN)/Source/ConvolutionFunctions/arm_nn_mat_mult_kernel_s8_s16_reordered.c \
$(CMSIS_NN)/Source/ConvolutionFunctions/arm_nn_mat_mult_s8.c \
 \
$(CMSIS_NN)/Source/FullyConnectedFunctions/arm_fully_connected_mat_q7_vec_q15.c \
$(CMSIS_NN)/Source/FullyConnectedFunctions/arm_fully_connected_mat_q7_vec_q15_opt.c \
$(CMSIS_NN)/Source/FullyConnectedFunctions/arm_fully_connected_q7.c \
$(CMSIS_NN)/Source/FullyConnectedFunctions/arm_fully_connected_q7_opt.c \
$(CMSIS_NN)/Source/FullyConnectedFunctions/arm_fully_connected_q15.c \
$(CMSIS_NN)/Source/FullyConnectedFunctions/arm_fully_connected_q15_opt.c \
$(CMSIS_NN)/Source/FullyConnectedFunctions/arm_fully_connected_s8.c \
 \
$(CMSIS_NN)/Source/NNSupportFunctions/arm_nn_accumulate_q7_to_q15.c \
$(CMSIS_NN)/Source/NNSupportFunctions/arm_nn_add_q7.c \
$(CMSIS_NN)/Source/NNSupportFunctions/arm_nn_mult_q7.c \
$(CMSIS_NN)/Source/NNSupportFunctions/arm_nn_mult_q15.c \
$(CMSIS_NN)/Source/NNSupportFunctions/arm_nntables.c \
$(CMSIS_NN)/Source/NNSupportFunctions/arm_q7_to_q15_no_shift.c \
$(CMSIS_NN)/Source/NNSupportFunctions/arm_q7_to_q15_reordered_no_shift.c \
$(CMSIS_NN)/Source/NNSupportFunctions/arm_q7_to_q15_reordered_with_offset.c \
$(CMSIS_NN)/Source/NNSupportFunctions/arm_q7_to_q15_with_offset.c \
 \
$(CMSIS_NN)/Source/PoolingFunctions/arm_avgpool_s8.c \
$(CMSIS_NN)/Source/PoolingFunctions/arm_max_pool_s8.c \
$(CMSIS_NN)/Source/PoolingFunctions/arm_max_pool_s8_opt.c \
$(CMSIS_NN)/Source/PoolingFunctions/arm_pool_q7_HWC.c \
 \
$(CMSIS_NN)/Source/ReshapeFunctions/arm_reshape_s8.c \
 \
$(CMSIS_NN)/Source/SoftmaxFunctions/arm_softmax_q7.c \
$(CMSIS_NN)/Source/SoftmaxFunctions/arm_softmax_q15.c \
$(CMSIS_NN)/Source/SoftmaxFunctions/arm_softmax_s8.c \
$(CMSIS_NN)/Source/SoftmaxFunctions/arm_softmax_u8.c \
$(CMSIS_NN)/Source/SoftmaxFunctions/arm_softmax_with_batch_q7.c

SRC_DPS = \
$(CMSIS_DSP)/Source/BasicMathFunctions/BasicMathFunctions.c \
$(CMSIS_DSP)/Source/SupportFunctions/SupportFunctions.c


ifeq ($(APPLICATION),gru)
  CMSIS_NN_APP := $(CMSIS_NN)/Examples/ARM/arm_nn_examples/gru
  INCLUDE_FLAGS_APP := \
        -I$(CMSIS_NN)/Include \
        -I$(CMSIS_NN_APP) \
        -I$(CMSIS_DSP)/Include

  SRC_C += $(SRC_NN) \ 
           $(SRC_DPS) \
           $(CMSIS_NN_APP)/arm_nnexamples_gru.cpp
else
  ifeq ($(APPLICATION),cifar)
    CMSIS_NN_APP := $(CMSIS_NN)/Examples/ARM/arm_nn_examples/cifar10
    INCLUDE_FLAGS_APP := \         
        -I$(CMSIS_NN)/Include \
        -I$(CMSIS_NN_APP) \
        -I$(CMSIS_DSP)/Include

    SRC_C += $(SRC_NN) \
             $(SRC_DPS) \
             $(CMSIS_NN_APP)/arm_nnexamples_cifar10.cpp
  else
    CMSIS_NN_APP :=
    SRC_C += main.c
  endif
endif


INCLUDE_FLAGS = -I$(CMSIS)/Device/ARM/ARMCM3/Include \
	-I$(CMSIS)/CMSIS/Core/Include \
	-I. \
	$(INCLUDE_FLAGS_APP)

CFLAGS = -mcpu=cortex-m3 \
	-specs=nano.specs -specs=nosys.specs \
        -specs=rdimon.specs \
	-Wall \
	$(INCLUDE_FLAGS) \
	-mthumb \
	-nostartfiles

all: $(BINARY)

boot.o: $(SRC_ASM)
	$(CC) $(CFLAGS) -c $^ -o $@

$(BINARY): $(SRC_C) boot.o
	$(CC) $^ $(CFLAGS) -T $(LINKER_SCRIPT) -o $@
	$(OBJ) -D $@ > $(BINARY_OBJDUMP)


# Ctrl-A, then X to quit
run: $(BINARY)
	-$(QEMU_COMMAND) -d int,cpu_reset
	echo $? " has exited"

gdbserver: $(BINARY)
	$(QEMU_COMMAND) -S -s -d int,cpu_reset

help:
	$(QEMU_PATH) --machine help

gdb: $(BINARY)
	$(GDB) $(BINARY) -ex "target remote:1234"

clean:
	rm -f $(BINARY_OBJDUMP) *.o *.elf 
