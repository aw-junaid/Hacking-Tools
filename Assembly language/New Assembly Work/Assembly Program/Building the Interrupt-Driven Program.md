Building an interrupt-driven program involves designing software that responds to hardware interrupts efficiently, allowing your system to handle real-time events without polling. Below is an overview of how to create a simple interrupt-driven program in ARM assembly language, along with an example.

### Key Concepts

1. **Interrupts**: Interrupts are signals from hardware or software that temporarily halt the current execution flow to execute a special piece of code (the interrupt handler or service routine). 

2. **Interrupt Vector Table (IVT)**: This table contains addresses for each interrupt handler. When an interrupt occurs, the processor uses this table to jump to the appropriate handler.

3. **Enabling/Disabling Interrupts**: Before and after executing critical sections of code, interrupts may need to be enabled or disabled to prevent context switches during important operations.

4. **Interrupt Handler**: This is the code that runs in response to the interrupt. It typically performs a specific action and then returns control to the interrupted program.

### Example: Simple Interrupt-Driven Program

The following example demonstrates a simple interrupt-driven program that responds to a timer interrupt. The program toggles an LED or a GPIO pin every time the interrupt occurs.

#### ARM Assembly Code

```assembly
.section .data
    LED_ON:  .word 0x1                  ; Define a value for LED ON
    LED_OFF: .word 0x0                  ; Define a value for LED OFF

.section .bss
    .lcomm led_state, 4                  ; Reserve space for LED state

.section .text
.global _start
.global Timer_ISR                         ; Declare the timer interrupt service routine

_start:
    // Initialize the system and GPIO
    BL     init_system                    ; Call system initialization
    BL     enable_interrupts              ; Enable interrupts

main_loop:
    // Main program loop (could perform other tasks)
    B      main_loop                     ; Loop indefinitely

// Interrupt Service Routine (ISR)
Timer_ISR:
    // Save the context (registers, if needed)
    PUSH    {r0, r1, r2, r3, r12}        ; Save registers

    // Toggle the LED state
    LDR     r0, =led_state               ; Load address of led_state
    LDR     r1, [r0]                     ; Load current LED state
    EOR     r1, r1, #1                   ; Toggle LED state (0 -> 1, 1 -> 0)
    STR     r1, [r0]                     ; Store new LED state

    // Acknowledge the interrupt (specific to hardware)
    BL      clear_interrupt_flag         ; Clear the interrupt flag

    // Restore the context
    POP     {r0, r1, r2, r3, r12}        ; Restore registers
    BX      r12                          ; Return from the ISR

// Functions for system initialization and interrupt handling
init_system:
    // Initialize GPIO and other peripherals
    // ...
    BX      lr                            ; Return from the function

enable_interrupts:
    // Enable interrupts in the processor
    // ...
    BX      lr                            ; Return from the function

clear_interrupt_flag:
    // Code to clear the interrupt flag
    // ...
    BX      lr                            ; Return from the function
```

### Explanation of the Example:

1. **Data Section**:
   - **`LED_ON` and `LED_OFF`**: Constants to represent the ON and OFF states of the LED.
   - **`led_state`**: A reserved space to hold the current state of the LED.

2. **Text Section**:
   - **`_start`**: The entry point of the program. It initializes the system and enables interrupts.
   - **`main_loop`**: The main program loop that continues running indefinitely. Here, you can add other tasks that the main program should perform.

3. **Interrupt Service Routine (ISR)**:
   - **`Timer_ISR`**: This is the interrupt handler for the timer interrupt.
     - **Context Saving**: The ISR starts by saving the context of registers that might be altered during the execution.
     - **Toggle LED State**: It toggles the LED state by reading the current state, performing an exclusive OR operation to toggle it, and then writing it back.
     - **Acknowledge Interrupt**: It calls a function to clear the interrupt flag specific to the hardware.
     - **Context Restoring**: Finally, it restores the registers and returns control to the interrupted program.

4. **Initialization Functions**:
   - **`init_system`**: Initializes the system and GPIO settings.
   - **`enable_interrupts`**: Enables the processor interrupts, allowing the system to respond to interrupt signals.
   - **`clear_interrupt_flag`**: A function to clear the interrupt flag so that the system can handle subsequent interrupts.

### Summary

- **Interrupt-Driven Programming**: This approach allows your program to respond immediately to hardware events, improving responsiveness and efficiency.
- **ISRs**: These routines must execute quickly to avoid missing subsequent interrupts.
- **Context Management**: Properly saving and restoring the processor state is crucial to ensure that the main program continues running as expected after an interrupt.
