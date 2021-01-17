# RTX RTOS

## Goal

Get a basic understanding of the RTX RTOS API and experiment though QEMU.

## Program and decompile

At the root of the project, run `make 3_8_3` and then `make run`.
This base program instanciate a thread A that prints something each 1 second.
Even if the thread A will stop after a certain amount of prints (limited by the
counter variable), the program will never finish as there is still an IDLE task
in the RTX RTOS. You can terminate the program and exit QEMU by using:
`CTRL+A + X`.

# Instructions

- Add a new thread task B that prints a value on the UART from the RTOS.
What happens?

Note: You can add the following code at the end of the function of the thread B
to exit immediately QEMU once the counter has finished:
```
  osDelay(1000);
  exit(0);
```

- What would happen if both tasks were incrementing the same global variable?

- How would you fix this issue?
