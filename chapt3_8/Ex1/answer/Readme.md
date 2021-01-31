## Disassembly of the code

```
arm-none-eabi-objdump -D cm3.elf > 3_8_1_objdump.txt
```

## Disassembly result

```
Disassembly of section .text:

00000000 <__Vectors>:
       0:	20010000 	andcs	r0, r1, r0
       4:	00000585 	andeq	r0, r0, r5, lsl #11
       8:	000005c5 	andeq	r0, r0, r5, asr #11
       c:	000005c3 	andeq	r0, r0, r3, asr #11
      10:	000005c5 	andeq	r0, r0, r5, asr #11
      14:	000005c5 	andeq	r0, r0, r5, asr #11
      18:	000005c5 	andeq	r0, r0, r5, asr #11
	...
      2c:	00000531 	andeq	r0, r0, r1, lsr r5
      30:	000005c5 	andeq	r0, r0, r5, asr #11
      34:	00000000 	andeq	r0, r0, r0
      38:	000005c5 	andeq	r0, r0, r5, asr #11
      3c:	000005c5 	andeq	r0, r0, r5, asr #11
      40:	000005c5 	andeq	r0, r0, r5, asr #11
      44:	000005c5 	andeq	r0, r0, r5, asr #11
      48:	000005c5 	andeq	r0, r0, r5, asr #11
      4c:	000005c5 	andeq	r0, r0, r5, asr #11
      50:	000005c5 	andeq	r0, r0, r5, asr #11
      54:	000005c5 	andeq	r0, r0, r5, asr #11
      58:	000005c5 	andeq	r0, r0, r5, asr #11
      5c:	000005c5 	andeq	r0, r0, r5, asr #11
      60:	000005c5 	andeq	r0, r0, r5, asr #11
      64:	000005c5 	andeq	r0, r0, r5, asr #11
	...

000003c0 <SystemInit>:
     3c0:	b480      	push	{r7}
     3c2:	af00      	add	r7, sp, #0
     3c4:	4b04      	ldr	r3, [pc, #16]	; (3d8 <__Vectors_Size+0x18>)
     3c6:	4a05      	ldr	r2, [pc, #20]	; (3dc <__Vectors_Size+0x1c>)
     3c8:	609a      	str	r2, [r3, #8]
     3ca:	4b05      	ldr	r3, [pc, #20]	; (3e0 <__Vectors_Size+0x20>)
     3cc:	4a05      	ldr	r2, [pc, #20]	; (3e4 <__Vectors_Size+0x24>)
     3ce:	601a      	str	r2, [r3, #0]
     3d0:	bf00      	nop
     3d2:	46bd      	mov	sp, r7
     3d4:	bc80      	pop	{r7}
     3d6:	4770      	bx	lr

```

### Inspecting the data

Looking at this result, we can see the following different data:
```
       0:	20010000 	andcs	r0, r1, r0
       4:	00000585 	andeq	r0, r0, r5, lsl #11
       8:	000005c5 	andeq	r0, r0, r5, asr #11
       c:	000005c3 	andeq	r0, r0, r3, asr #11
  ...
      2c:	00000531 	andeq	r0, r0, r1, lsr r5
	...
      34:	00000000 	andeq	r0, r0, r0
```

Where:
- `0x20010000` is the address of the base stack pointer
- `0x00000585` is the address of the `<Reset_Handler>`
- `0x000005c5` is the address of the `<BusFault_Handler>`
- `0x000005c3` is the address of the `<HardFault_Handler>`
- `0x00000000` are for undefined entries in the vector.

As they are addresses and not instructions, when the tool is trying to decode
those data, it gives some assembly instructions that are not related and for
some data, undefined.
Most of the handler indexes are pointing to the same function (`0x000005c5`) by
default.

Note: for the addresses, the LSb is always bit 1. This bit indicates thumb
instructions. Which means that the address `0x000005c3` is in fact mapping at
`0x000005c2` which can be retrieved in the objdump as the `Reset_Handler`
function.

### Looking at the indexes

`NMI_Handler` and `MemManage_Handler` indexes are inverted in the vector table.
This means that if there is an error and the `MemManage_Handler` index of the
vector is supposed to be called, the actual function will be `NMI_Handler`.

This can be corrected by updating the file `startup_ARMCM3.S`:
```
                .long    __StackTop        /*     Top of Stack */
                .long    Reset_Handler     /*     Reset Handler */
                .long    NMI_Handler
                .long    HardFault_Handler
                .long    MemManage_Handler
                .long    BusFault_Handler
                .long    UsageFault_Handler
```

### Redefining the handlers

All handlers in `startup_ARMCM3.S` are defined as weak functions and can be
redefined by creating a function with the same name. This can be seen in the
`handler_definitions.c` file like it was done for the `SVC_Handler`.

### Update the vector table

Updating the address of the Cortex-M3's Vector Table causes an undefined
behavior. For Cortex-M3, the vector table should start at address (VTOR) 0.
The format of this table must be as follow:
- Index 0 should be the start address of the Stack pointer
- Index 1 should be the address to the reset handler
- Other indexes are for other handlers

If there is no vector table at address 0, the Cortex-M3 will have no indication
on how to boot and will execute some random instructions that are located at
this memory location. This will probably create some faults. As the fault
handler are not properly defined, the processor behavior is undefined.

This can be done in the linker script by updating the variable: __ROM_BASE of
the `FLASH (rx)  : ORIGIN = __ROM_BASE, LENGTH = __ROM_SIZE` which is where the
.text section starts.
