#include "uart.h"

#include <stdint.h>
#include <stdio.h>

/* Base address of the UART0 https://www.ti.com/product/LM3S6965 */
#define UART0_BASE 0x4000c000

#define UARTDR(base) (*((volatile uint32_t *)(base + 0x000)))

/*
 * This function is a primitive for printf. This is the base function that
 * printf uses.
 */
int __io_putchar(int ch)
{
  UARTDR(UART0_BASE) = ch;
  return ch;
}

void printString(const char* ptr)
{
  while (*ptr != '\0') {
      __io_putchar(*ptr);
      ptr++;
  }
}

