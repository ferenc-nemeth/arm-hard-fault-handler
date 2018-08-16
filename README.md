# arm-hard-fault-handler
What to do when Hard fault hits? Debugger and error reporter solution for ARM Cortex M3 and M4.

### Table of content
- [Introduction](#introduction)
- [How it works](#how-it-works)
- [About the demo](#about-the-demo)
- [Porting](#porting)
- [References](#references)


### Introduction
Lot of people have no idea what to do when a Hard fault hits. I even met well-experienced, senior developers, who were like, "yeah Hard fault, what now". ARM provides us with some useful features and registers to help us in this case.<br>
With this code, I would like demonstrate how it works on ARM Cortex-M3 and Cortex-M4. The main idea comes from the book "The definitive guide to ARM Cortex-M3 and Cortex-M4 processors" by Joseph Yiu [1], altough my version gives more details of the fault.

### How it works
In case of a Hard Fault, my software writes out (through UART) the stack frame registers, the Hard fault registers, and a short error report regarding the problem itself.<br>
The files I created are in Src and Inc folders. Src holds the source codes, Inc holds the headers. The main() function is inside main.c, it has an initialization for UART port and the line of code, that makes the Hard fault. The fault is simple: I try to write to a memory location (External Devices), where otherwise I am not supposed to. The UART.c and UART.h hold the functions, that are necessary for the communication. The FaultHandler.c and FaultHandler.h are where the real magic happens. There are two functions: HardFault_Handler() is the function where we get in case of a fault by default. It is provided by CMSIS. It contains an Assembly code, what determines which SP was used (MSP or PSP) with the help of LR.<br>
After that, it passes the address of SP and value of LR to my function: ReportHardFault(). This function reads and then prints out the stack frame and the fault related registers with detailed information about the problem itself.<br>
The registers are:
- R0
- R1
- R2
- R3
- R12
- Link Register
- Program Counter
- Program Status Register
- Hard Fault Status Register
- Configurable Fault Status Register
- Memory Manage Address Register
- Bus Fault Address Register
- Auxiliary Fault Status Register
- Exception Return

The exact description of these register are in the datasheet or on ARM's website. [2][3] <br>
Finally, the program ends with a breakpoint and with a while(1) loop.
<br><img src="https://raw.githubusercontent.com/iamferenc/arm-hard-fault-handler/master/Design/FaultHandler.png" ><br>
*Figure 1. The Activity diagram of the software.*

### About the demo
As I mentioned, in the main function I try to force a (Bus) fault, with writing to the "External Devices" memory location. You can see the result from the included terminal window and you can verify the register values with the included disassembly code.
<br><img src="https://raw.githubusercontent.com/iamferenc/arm-hard-fault-handler/master/Pictures/bus_fault_terminal.PNG" ><br>
*Figure 2. The output of the Bus fault.*

<br><img src="https://raw.githubusercontent.com/iamferenc/arm-hard-fault-handler/master/Pictures/bus_fault_asm.PNG" ><br>
*Figure 3. The disassembly code of the Bus fault.*

I tried a second problem (not included): div-by-zero. I tried to div 0x0000FEC0 by 0x00000000. See the included terminal window and the disassembly code for the result.
<br><img src="https://raw.githubusercontent.com/iamferenc/arm-hard-fault-handler/master/Pictures/usage_fault_terminal.PNG" ><br>
*Figure 4. The output of the Usage fault.*

<br><img src="https://raw.githubusercontent.com/iamferenc/arm-hard-fault-handler/master/Pictures/usage_fault_asm.PNG" ><br>
*Figure 5. The disassembly code of the Usage fault.*

### Porting
I created this software with Atollic TrueSTUDIO and used an STM32VLDISCOVERY board with STM32F100RB microcontroller. If you have them, you can try it out immediately.<br>
The code inside the FaultHandler.c and FaultHandler.h should work on every ARM Cortex-M3 and Cortex-M4 systems. In theory it can be easily ported to M4F, I included the lazy stacking related problems, but I did not include the floating-point registers (S0-S15, FPSCR). About Cortex M0 and M0+... it's a different story.

### References
[1] Joseph Yiu: The definitive guide to ARM Cortex-M3 and Cortex-M4 processors; 3rd Edition; ISBN: 9780124080829; 2013<br>
[2] [ARM Cortex-M3 Processor Technical Reference Manual](http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.100165_0201_00_en/index.html)<br>
[3] [ARM Cortex-M4 Processor Technical Reference Manual](http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.100166_0001_00_en/index.html)<br>
