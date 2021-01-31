# RTX RTOS

- Adding the thread will create a concurrency issue as the function used to
print the character of the strings can be interrupted at any moment. Which means
that, while Thread B is printing some characters, Thread A can interrupt Thread
B and start printing its own string. Here is a typical example of output that
can happen:

```
Hello, I am the running thread A
Hello, I am the running thread B
Hello, I am the running thread B
Hello, I am the running thread B
Hello, I am the running thread B
Hello, I am the running threadHello, I am the running thread A
 B
Hello, I am the running thread B
Hello, I am the running thread B
Hello, I am the running thread B
Hello, I am the running thread B
Hello, I am the running thread B
Hello, I am the running thread Hello, I am the running thread A
B
Hello, I am the running thread B
Hello, I am the running thread B
Hello, I am the running thread B
Hello, I am the running thread B
Hello, I am the running thread B
Hello, I am the running thread B
Hello, I am the running thread B
Hello, I am the running thread B
Hello, I am the running thread B
Hello, I am Hello, I am the running thread A
the running thread B
Hello, I am the running thread B
Hello, I am the running thread B
Hello, I am the running thread B
Hello, I am the running thread B
Hello, I am the running thread B
Hello, I am the running thread B
Hello, I am the running thread B
Hello, I am the running thread Hello, I am the running thread A
B
Hello, I am the running thread B
Hello, I Hello, I am the running thread A
am the running thread B
Hello, I am the running thread B
Hello, I am the running thread B
Hello, I am the running thread B
Hello, I am the running thread B
Hello, I am the running thread B
Hello, I am the running thread B
Hello, I am the running thread B
Hello, I am the running thread B
Hello, I am the running thread B
Hello, I am the running thread B
Hello, I am theHello, I am the running thread A
 running thread B
Hello, I am the running thread B
Hello, I am the running thread B
Hello, I am the running thread B
Hello, I am the running thread B
Hello, I am the running thread B
Hello, I am the running thrHello, I am the running thread A
ead B
Hello, I am the running thread B
Hello, I am the rHello, I am the running thread A
unning thread B
Hello, I am the running thread B
Hello, I am the running thread B
Hello, I am the running thread B
Hello, I am the running thread B
Hello, I am the running thread B
Hello, I amHello, I am the running thread A
 the running thread B
Hello, I am the running thread B
Hello, I am the running thread B
Hello, I am the running thread B
Hello, I am the running thread BHello, I am the running thread A

Hello, I am the running thread B
Hello, I am the running thread B
Hello, I am the running thread B
Hello, I am the running thread B
Hello, I am the running thread B

```
Note that, because Thread B has a lower priority than Thread A, it is generaly
interrupted by thread A during the print of its string.

- If both tasks were incrementing the same variable, the same concurrency issue
would happen. Incrementing a variable in C is a single instruction like `i++`.
However, this instruction is in fact multiple assembly instruction as the
program needs to: load the value, increment it and store it to memory. Any
task can be interrupted at any moment, so the following scenario could happen:

```
-------------A-----B----C----D----E----F--> Step
==========================================
Thread A|    |   Fetch Add Store  |    |
Thread B|  Fetch   |    |    |   Add Store
==========================================
i Value |    0     0    0    1     1   1
```

As thread A and B fetched the same value of `i` in memory (0), they both add 1
and store the value 1 to memory.

- Fixing concurrency can be done by protecting the ressource that needs to be
accessed concurrently. This is call a critical section and the most popular
mecanisms used are mutexes or semaphores.
