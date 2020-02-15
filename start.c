#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

extern int main(void);

/* Static functions and constant data */
const int SYS_WRITE0 = 0x04;
const int SYS_EXIT = 0x18;

void semihost(int sys_id, const void *arg)
{
  register int r0 __asm__ ("r0") = sys_id;
  register const void *r1 __asm__ ("r1") = arg;
  __asm__ volatile ("bkpt 0xab");
  (void) r0;
  (void) r1;
}

void logPrint(const char* format, ...)
{ 
  char text[256];
  va_list args;
  va_start (args, format);
  sprintf(text, format, args);
  semihost(SYS_WRITE0, text);
  va_end (args);
}

void _start(void)
{
  main();
  int success = 0x20026;
  semihost(SYS_EXIT, &success);
  while (1) {
  }
}
