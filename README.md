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
With this code, I would like demonstrate how it works on ARM Cortex-M3 and Cortex-M4. The main idea comes from the book "The definitive guide to ARM Cortex-M3 and Cortex-M4 processors" by Joseph Yiu [[1]](#references), altough my version gives more details of the fault.

### How it works
In case of a Hard Fault, my software writes out (through SWO trace interface) the stack frame registers, the Hard fault registers, and a short error report regarding the problem itself.<br>
The files I created are in Src and Inc folders. Src holds the source codes, Inc holds the headers. The main() function is inside main.c, it has the line of code, that makes the Hard fault. The fault is simple: I try to write to a memory location (External Devices), where otherwise I am not supposed to. The Communication.c and Communication.h hold the function, that are necessary for the printf() trace feature. The FaultHandler.c and FaultHandler.h are where the real magic happens. There are two functions: HardFault_Handler() is the function where we get in case of a fault by default. It is provided by CMSIS. It contains an Assembly code, what determines which SP was used (MSP or PSP) with the help of LR.<br>
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

The exact description of these register are in the datasheet or on ARM's website. [[2]](#references)[[3]](#references) <br>
Finally, the program ends with a breakpoint and with a while(1) loop.
<br><img src="https://raw.githubusercontent.com/iamferenc/arm-hard-fault-handler/master/Design/FaultHandler.png" ><br>
*Figure 1. The Activity diagram of the software.*

### About the demo
As I mentioned, in the main function I try to force a (Bus) fault, with writing to the "External Devices" memory location. You can see the result from the included port window and you can verify the register values with the included disassembly code.

*The output of the Bus fault:*
```
!!!Hard Fault detected!!!

Stack frame:
R0 :        0x20000000
R1 :        0x00000000
R2 :        0x01234567
R3 :        0xAAAAAAAA
R12:        0xFB775B7C
LR :        0x0800051B
PC :        0x080004DC
PSR:        0x61000000

Fault status:
HFSR:       0x40000000
CFSR:       0x00000400
MMAR:       0xE000ED34
BFAR:       0xE000ED38
AFSR:       0x00000000

Other:
EXC_RETURN: 0xFFFFFFF9

Details of the fault status:
Hard fault status:
 - Forced Hard fault.
MemManage fault status:
 - MMAR holds an invalid address.
Bus fault status:
 - BFAR holds an invalid address.
 - Data bus error has occurred, but the return address in the stack is not related to the fault.
Usage fault status:

```

*The disassembly code of the Bus fault:*
```assembly
  (*((volatile uint32_t *)(0xAAAAAAAA))) = 0x1234567;
 80004d4:	f04f 33aa 	mov.w	r3, #2863311530	; 0xaaaaaaaa
 80004d8:	4a01      	ldr	r2, [pc, #4]	; (80004e0 <main+0xc>)
 80004da:	601a      	str	r2, [r3, #0]
 80004dc:	e7fe      	b.n	80004dc <main+0x8>
```

I tried a second problem (not included): div-by-zero. I tried to div 0x0000FEC0 by 0x00000000. See the included port window and the disassembly code for the result.

*The output of the Usage fault:*
```
!!!Hard Fault detected!!!

Stack frame:
R0 :        0x20000000
R1 :        0x00000000
R2 :        0x00000000
R3 :        0x0000FEC0
R12:        0xFB775B7C
LR :        0x08000533
PC :        0x080004EE
PSR:        0x61000000

Fault status:
HFSR:       0x40000000
CFSR:       0x02000000
MMAR:       0xE000ED34
BFAR:       0xE000ED38
AFSR:       0x00000000

Other:
EXC_RETURN: 0xFFFFFFF9

Details of the fault status:
Hard fault status:
 - Forced Hard fault.
MemManage fault status:
 - MMAR holds an invalid address.
Bus fault status:
 - BFAR holds an invalid address.
Usage fault status:
 - The processor has executed an SDIV or UDIV instruction with a divisor of 0.
```

*The disassembly code of the Usage fault:*
```Assembly
  volatile int a = 0xFEC0;
 80004e0:	f64f 63c0 	movw	r3, #65216	; 0xfec0
 80004e4:	9301      	str	r3, [sp, #4]
  volatile int b = 0x0000;
 80004e6:	2300      	movs	r3, #0
 80004e8:	9302      	str	r3, [sp, #8]
  volatile int c = a/b;
 80004ea:	9b01      	ldr	r3, [sp, #4]
 80004ec:	9a02      	ldr	r2, [sp, #8]
 80004ee:	fb93 f3f2 	sdiv	r3, r3, r2
```

### Porting
I created this software with Atollic TrueSTUDIO and used an STM32VLDISCOVERY board with STM32F100RB microcontroller. If you have them, you can try it out immediately.<br>
The code inside the FaultHandler.c and FaultHandler.h should work on every ARM Cortex-M3 and Cortex-M4 systems. In theory it can be easily ported to M4F, I included the lazy stacking related problems, but I did not include the floating-point registers (S0-S15, FPSCR). About Cortex M0 and M0+... it's a different story.

### References
[1] Joseph Yiu: The definitive guide to ARM Cortex-M3 and Cortex-M4 processors; 3rd Edition; ISBN: 9780124080829; 2013<br>
[2] [ARM Cortex-M3 Processor Technical Reference Manual](http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.100165_0201_00_en/index.html)<br>
[3] [ARM Cortex-M4 Processor Technical Reference Manual](http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.100166_0001_00_en/index.html)<br>
