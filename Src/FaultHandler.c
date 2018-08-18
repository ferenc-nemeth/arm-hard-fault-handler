/**
 * @file    FaultHandler.c
 * @author  Ferenc Nemeth
 * @date    12 Aug 2018
 * @brief   This is the fault handler, where the program gets, if there is a problem.
 *          In case of a Hard fault, the following actions are going to be executed:
 *          - Prints out the stack frame registers (R0-R3, R12, LR, PC, PSR).
 *          - Prints out the fault status registers (HFSR, CFSR, MMFAR, BFAR, AFSR).
 *          - Prints out the EXC_RETURN register.
 *          - After these, the software analyzes the value of the registers
 *            and prints out a report regarding the fault.
 *          - Stop the execution with a breakpoint.
 *
 *          Copyright (c) 2018 Ferenc Nemeth - https://github.com/ferenc-nemeth
 */

#include "FaultHandler.h"

/* Local function. */
void ReportHardFault(uint32_t *hard_fault_values, uint32_t exc);

/**
 * @brief   The program jumps here in case of a Hard fault.
 *          Checks which stack (MSP or PSP) was used.
 *          Extracts the stack frame location (and EXC_return),
 *          then pass them to ReportHardFault().
 * @param   void
 * @return  void
 */
void HardFault_Handler(void)
{
  __asm volatile
  (
      "TST    LR, #0b0100;      "
      "ITE    EQ;               "
      "MRSEQ  R0, MSP;          "
      "MRSNE  R0, PSP;          "
      "MOV    R1, LR;           "
      "B      ReportHardFault;  "
  );
}

/**
 * @brief   Prints the registers and gives detailed information about the error(s).
 * @param   *stack_frame: Stack frame registers (R0-R3, R12, LR, LC, PSR).
 * @param   exc: EXC_RETURN register.
 * @return  void
 */
void ReportHardFault(uint32_t *stack_frame, uint32_t exc)
{
  uint32_t r0   = stack_frame[0];
  uint32_t r1   = stack_frame[1];
  uint32_t r2   = stack_frame[2];
  uint32_t r3   = stack_frame[3];
  uint32_t r12  = stack_frame[4];
  uint32_t lr   = stack_frame[5];
  uint32_t pc   = stack_frame[6];
  uint32_t psr  = stack_frame[7];
  uint32_t hfsr = SCB->HFSR;
  uint32_t cfsr = SCB->CFSR;
  uint32_t mmar = SCB->MMFAR;
  uint32_t bfar = SCB->BFAR;
  uint32_t afsr = SCB->AFSR;

  printf("\n\r!!!Hard Fault detected!!!\n\r");

  printf("\n\rStack frame:\n\r");
  printf("R0 :        0x%08lX\n\r", r0);
  printf("R1 :        0x%08lX\n\r", r1);
  printf("R2 :        0x%08lX\n\r", r2);
  printf("R3 :        0x%08lX\n\r", r3);
  printf("R12:        0x%08lX\n\r", r12);
  printf("LR :        0x%08lX\n\r", lr);
  printf("PC :        0x%08lX\n\r", pc);
  printf("PSR:        0x%08lX\n\r", psr);

  printf("\n\rFault status:\n\r");
  printf("HFSR:       0x%08lX\n\r", hfsr);
  printf("CFSR:       0x%08lX\n\r", cfsr);
  printf("MMAR:       0x%08lX\n\r", mmar);
  printf("BFAR:       0x%08lX\n\r", bfar);
  printf("AFSR:       0x%08lX\n\r", afsr);

  printf("\n\rOther:\n\r");
  printf("EXC_RETURN: 0x%08lX\n\r", exc);

  printf("\n\rDetails of the fault status:\n\r");
  printf("Hard fault status:\n\r");
  if (CHECK_BIT(hfsr, FORCED))
  {
    printf(" - Forced Hard fault.\n\r");
  }
  if (CHECK_BIT(hfsr, VECTTBL))
  {
    printf(" - Bus fault on vector table read.\n\r");
  }
  printf("MemManage fault status:\n\r");
  if (CHECK_BIT(cfsr, MMARVALID))
  {
    printf(" - MMAR holds a valid address.\n\r");
  }
  else
  {
    printf(" - MMAR holds an invalid address.\n\r");
  }
  if (CHECK_BIT(cfsr, MLSPERR))
  {
    printf(" - Fault occurred during floating-point lazy state preservation.\n\r");
  }
  if (CHECK_BIT(cfsr, MSTKERR))
  {
    printf(" - Stacking has caused an access violation.\n\r");
  }
  if (CHECK_BIT(cfsr, MUNSTKERR))
  {
    printf(" - Unstacking has caused an access violation.\n\r");
  }
  if (CHECK_BIT(cfsr, DACCVIOL))
  {
    printf(" - Load or store at a location that does not permit the operation.\n\r");
  }
  if (CHECK_BIT(cfsr, IACCVIOL))
  {
    printf(" - Instruction fetch from a location that does not permit execution.\n\r");
  }
  printf("Bus fault status:\n\r");
  if (CHECK_BIT(cfsr, BFARVALID))
  {
    printf(" - BFAR holds a valid address.\n\r");
  }
  else
  {
    printf(" - BFAR holds an invalid address.\n\r");
  }
  if (CHECK_BIT(cfsr, LSPERR))
  {
    printf(" - Fault occurred during floating-point lazy state preservation.\n\r");
  }
  if (CHECK_BIT(cfsr, STKERR))
  {
    printf(" - Stacking has caused a Bus fault.\n\r");
  }
  if (CHECK_BIT(cfsr, UNSTKERR))
  {
    printf(" - Unstacking has caused a Bus fault.\n\r");
  }
  if (CHECK_BIT(cfsr, IMPRECISERR))
  {
    printf(" - Data bus error has occurred, but the return address in the stack is not related to the fault.\n\r");
  }
  if (CHECK_BIT(cfsr, PRECISERR))
  {
    printf(" - Data bus error has occurred, and the return address points to the instruction that caused the fault.\n\r");
  }
  if (CHECK_BIT(cfsr, IBUSERR))
  {
    printf(" - Instruction bus error.\n\r");
  }
  printf("Usage fault status:\n\r");
  if (CHECK_BIT(cfsr, DIVBYZERO))
  {
    printf(" - The processor has executed an SDIV or UDIV instruction with a divisor of 0.\n\r");
  }
  if (CHECK_BIT(cfsr, UNALIGNED))
  {
    printf(" - The processor has made an unaligned memory access.\n\r");
  }
  if (CHECK_BIT(cfsr, NOCP))
  {
    printf(" - Attempted to access a coprocessor.\n\r");
  }
  if (CHECK_BIT(cfsr, INVPC))
  {
    printf(" - Illegal attempt to load of EXC_RETURN to the PC.\n\r");
  }
  if (CHECK_BIT(cfsr, INVSTATE))
  {
    printf(" - Attempted to execute an instruction that makes illegal use of the EPSR.\n\r");
  }
  if (CHECK_BIT(cfsr, INVSTATE))
  {
    printf(" - The processor has attempted to execute an undefined instruction.\n\r");
  }

  /* Breakpoint. */
  __asm volatile("BKPT #0");

  /* Infinite loop to stop the execution. */
  while(1);
}
