# FreeRTOS-Features-STM32
FreeRTOS is a real-time operating system kernel for embedded devices . it is capable of doing tasks, as an operating system does. The main purpose of an OS is to have the functionality, where we can use multiple tasks at the same time.  In addition to basic RTOS concepts, this course emphasizes on common design considerations like stack requirements, task operations such as switching between tasks, suspending and resuming tasks, terminating tasks . Task grouping , timing analysis and prioritization , task synchronization and communication features (semaphores) and how data transfer between tasks(queue) .


## FreeRTOS Introduction 
FreeRTOS is designed to be small and simple. It is mostly written in the C programming language to make it easy to port and maintain.
It also comprises a few assembly language functions where needed, mostly in architecture-specific scheduler routines.

FreeRTOS provides methods for multiple threads or tasks, mutexes, semaphores and software timers. A tickless mode is provided for low power applications.
Thread priorities are supported. FreeRTOS applications can be statically allocated, but objects can also be dynamically allocated with five schemes of memory
management (allocation):
- allocate only;
- allocate and free with a very simple, fast, algorithm;
- a more complex but fast allocate and free algorithm with memory coalescence;
- an alternative to the more complex scheme that includes memory coalescence that allows a heap to be broken across multiple memory areas
and C library allocate and free with some mutual exclusion protection.
- RTOSes typically do not have the more advanced features that are found in operating systems like Linux and Microsoft Windows, such as device drivers, advanced memory management, and user accounts. The emphasis is on compactness and speed of execution. FreeRTOS can be thought of as a thread library rather than an operating system, although command line interface and POSIX-like input/output (I/O) abstraction are available.

- FreeRTOS implements multiple threads by having the host program call a thread tick method at regular short intervals. The thread tick method switches tasks depending on priority and a round-robin scheduling scheme. The usual interval is 1 to 10 milliseconds (1⁄1000 to 1⁄100 of a second) via an interrupt from a hardware timer, but this interval is often changed to suit a given application.

The software distribution contains prepared configurations and demonstrations for every port and compiler, allowing rapid application design. The project website provides documentation and RTOS tutorials, and details of the RTOS design.
