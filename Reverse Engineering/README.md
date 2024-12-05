# Personal Research For Malware Developing

Most of the malwares are written in 32bit or 16 bit. They also works on 64 bit or higher then that.

- [Converting between Intel and AT&T syntax](https://github.com/aw-junaid/Hacking-Tools/blob/master/Reverse%20Engineering/Malware%20Data/Malware/Converting%20between%20Intel%20and%20AT%26T%20syntax.md)

## Every assembly language program is divided into three sections:

1. Data Section: This section is used for declaring initialized data or constants as this data does not ever change at runtime. You can declare constant values, buffer sizes, file names, etc.
2. BSS Section: This section is used for declaring uninitialized data or variables.
3. Text Section: This section is used for the actual code sections as it begins with a global _start which tells the kernel where execution begins.

- [moving_immediate_data](https://github.com/aw-junaid/Hacking-Tools/blob/master/Reverse%20Engineering/Malware%20Data/Malware/%3Bmoving_immediate_data.md)
- [Debugging - moving_immediate_data](https://github.com/aw-junaid/Hacking-Tools/blob/master/Reverse%20Engineering/Malware%20Data/Malware/Debugging%20-%20moving_immediate_data.md)


![image](https://github.com/user-attachments/assets/b16de288-bb19-4636-8697-eb6b762a87cf)

When we load the values as we demonstrated above and call INT 0x80, the very next instruction’s address in the User Space, ASM Code section which is your code, is placed into the Return Address area in The Stack. This is critical so that when INT 0x80 does its work, it can properly know what instruction is to be carried out next to ensure proper and sequential program execution.

Keep in mind in modern versions of Linux, we are utilizing Protected Mode which means you do NOT have access to the Linux Kernel Space. Everything under the long line that runs in the middle of the diagram above represents the Linux Kernel Space.

The natural question is why can’t we access this? The answer is very simple, Linux will NOT allow your code to access operating system internals as that would be very dangerous as any Malware could manipulate those components of the OS to track all sorts of things such as user keystrokes, activities and the like.

In addition, modern Linux OS architecture changes the address of these key components constantly as new software is installed and removed in addition to system patches and upgrades. This is the cornerstone of Protected Mode operating systems.

- [interrupt vector](https://github.com/aw-junaid/Hacking-Tools/blob/master/Reverse%20Engineering/Malware%20Data/Malware/interrupt%20vector.md)

The way that we have our code communicate with the Linux Kernel is through the use of a kernel servies call gate which is a protected gateway between User Space where your program is running and Kernel Space which is implemented through the Linux Software Interrupt of 0x80.

At the very, very bottom of memory where segment 0, offset 0 exists is a lookup table with 256 entries. Every entry is a memory address including segment and offset portions which comprise of 4 bytes per entry as the first 1,024 bytes are reserved for this table and NO OTHER CODE can be manipulated there. Each address is called an interrupt vector which comprises the whole called the interrupt vector table where every vector has a number from 0 to 255 to which vector 0 starts off occupying bytes 0 to 3. This continues with vector 1 which contains 4 to 7, etc.

Keep in mind, none of these addresses are part of permanent memory. What is static is vector 0x80 which points to the services dispatcher which point to Linux kernel service routines.

When the return address is popped off the stack returns to the next instruction, the instruction is called the Interrupt Return or IRET which completes the execution of program flow.


