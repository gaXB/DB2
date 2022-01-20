/************************************************************************************************
 * NXP Semiconductors
 * (c) Copyright 2020 NXP Semiconductors
 * ALL RIGHTS RESERVED.
 *************************************************************************************************
 *************************************************************************************************
Services performed by NXP in this matter are performed AS IS and without any
warranty. CUSTOMER retains the final decision relative to the total design
and functionality of the end product. NXP neither guarantees nor will be held
liable by CUSTOMER for the success of this project.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
CUSTOMER agrees to hold NXP harmless against any and all claims demands
or actions by anyone on account of any damage, or injury, whether commercial,
contractual, or tortuous, rising directly or indirectly as a result
of the advise or assistance supplied CUSTOMER in connection with product,
services or goods supplied under this Agreement.
 *************************************************************************************************
 * File:             main.c
 * Owner:            Daniel Martynek
 * Version:          2.0
 * Date:             Aug-14-2020
 * Classification:   General Business Information
 * Brief             Fault handling on S32K14x
 *************************************************************************************************
 * Detailed Description:
 * This example forces certain fault exceptions to shows the exception handling on S32K144 MCU.
 * Core registers are read to determine the root cause of the exception.
 * Stack frame is read to find the fault instruction.
 * -----------------------------------------------------------------------------------------------
 * Test HW:         S32144EVB-Q100
 * MCU:             S32K144 0N57U
 * Debugger:        S32DS 2.2, OpenSDA
 * Target:          internal_FLASH
 *************************************************************************************************
 * Revision History:
 * 1.0      May-30-2017   Daniel Martynek
 * 2.0      Aug-15-2020   Daniel Martynek
 ************************************************************************************************/


/*******************************************************************************
 * Includes
 *******************************************************************************/
#include "S32K144.h" /* include peripheral declarations S32K144 */
#include "fault_exceptions.h"

/*******************************************************************************
 * Global variables
 *******************************************************************************/
volatile uint32_t* p_stack_frame = 0;
volatile uint32_t stack_pointer = 0;
volatile uint32_t program_counter; // Program counter holds address on the instruction that has caused the exception

volatile uint32_t HFSR;  // HardFault Status Register
volatile uint32_t SHCSR; // System Handler Control and State Register
volatile uint32_t MMFSR; // MemManage Fault Status Register
volatile uint32_t MMFAR; // MemManage Fault Address Register
volatile uint32_t BFSR;  // BusFault Status Register
volatile uint32_t BFAR;  // BusFault Address register
volatile uint32_t UFSR;  // UsageFault Status Register

/*******************************************************************************
Function Name : UsageFault_Handler
Parameters    : NONE
Returns       : NONE
Notes         : declared with __attribute__((naked))
 *******************************************************************************/
void UsageFault_Handler(void)
{
    __asm__("MOVS R0, #4");
    __asm__("MOV R1, LR");
    __asm__("TST R1, R0");     // Test LR (EXC_RETURN[2])
    __asm__("ITE NE");
    __asm__("MRSNE R1, PSP");  // EXC_RETURN[2] = 1 PSP
    __asm__("MRSEQ R1, MSP");  // EXC_RETURN[2] = 0 MSP

    __asm__("LDR R0, =stack_pointer");
    __asm__("STR R1, [R0]"); // Store MSP or PSP into stack_pointer

    p_stack_frame = (uint32_t*)stack_pointer;
    program_counter  = p_stack_frame[6];  // holds the address of the instruction that has caused the exception

    UFSR = (S32_SCB->CFSR & 0xFFFF0000);
    /*   UsageFault Status Register (UFSR) CFSR[31:16] indicates the cause of UsageFault
     *   [25] DIVBYZERO = 1 Executed an SDIV or UDIV instruction with a divisor of 0
     *   [24] UNALIGNED = 1 Made an unaligned memory access
     *   [19] NOCP      = 1 Attempted an access to a coprocessor if the access is denied or privileged only (configurable in CPACR)
     *   [18] INVPC     = 1 Attempted an illegal load of EXC_RETURN to the PC, as a result of an invalid context, or an invalid EXC_RETURN value
     *   [17] INVSTATE  = 1 Attempted to execute an instruction that makes illegal use of the EPSR
     *   [16] UNDEFINSTR = 1 Attempted to execute an undefined instruction
     * */

    __asm__("BKPT");

    while(1)
    {

    }
}

/*******************************************************************************
Function Name : MemManage_Handler
Parameters    : NONE
Returns       : NONE
Notes         : declared with __attribute__((naked))
 *******************************************************************************/
void MemManage_Handler(void)
{
    __asm__("MOVS R0, #4");
    __asm__("MOV R1, LR");
    __asm__("TST R1, R0");     // Test LR (EXC_RETURN[2])
    __asm__("ITE NE");
    __asm__("MRSNE R1, PSP");  // EXC_RETURN[2] = 1
    __asm__("MRSEQ R1, MSP");  // EXC_RETURN[2] = 0

    __asm__("LDR R0, =stack_pointer");
    __asm__("STR R1, [R0]"); // Store MSP or PSP into stack_pointer

    p_stack_frame = (uint32_t*)stack_pointer;
    program_counter  = p_stack_frame[6];  // holds the address of the instruction that has caused the exception

    MMFSR = (S32_SCB->CFSR & 0x000000FF);
    /*  MemManage Fault Status Register (MMFSR) CFSR[7:0] indicates the cause of MemManage fault
     *  [7]  MMARVALID = 1 MMFAR holds a valid fault address
     *  [5]  MLSPERR   = 1 MemManage fault occurred during floating-point lazy state preservation
     *  [4]  MSTKERR   = 1 Stacking error caused by ARM MPU violation, MMAR address is not valid
     *  [3]  MUNSTKERR = 1 Unstacking error caused by ARM MPU violation, MMAR address is not valid
     *  [1]  DACCVIOL  = 1 The processor attempted a load or store at a protected (by ARM MPU) location, MMAR address is valid
     *  [0]  IACCVIOL  = 1 The processor attempted an instruction fetch from a protected (by ARM MPU) location, MMAR address is not valid
     */
    if(MMFSR & S32_SCB_CFSR_MMARVALID_MASK)
    {                              // If the value in the MMFAR is valid
        MMFAR = S32_SCB->MMFAR;    // MMFAR holds the address of the location that generated the MemManage fault
    }

    __asm__("BKPT");

    while(1)
    {

    }
}

/*******************************************************************************
Function Name : BusFault_Handler
Parameters    : NONE
Returns       : NONE
Notes         : declared with __attribute__((naked))
 *******************************************************************************/
void BusFault_Handler(void)
{
    __asm__("MOVS R0, #4");
    __asm__("MOV R1, LR");
    __asm__("TST R1, R0");     // Test LR (EXC_RETURN[2])
    __asm__("ITE NE");
    __asm__("MRSNE R1, PSP");  // EXC_RETURN[2] = 1
    __asm__("MRSEQ R1, MSP");  // EXC_RETURN[2] = 0

    __asm__("LDR R0, =stack_pointer");
    __asm__("STR R1, [R0]"); // Store MSP or PSP into stack_pointer

    p_stack_frame = (uint32_t*)stack_pointer;
    program_counter  = p_stack_frame[6];  // holds the address of the instruction that has caused the exception

    BFSR = (S32_SCB->CFSR & 0x0000FF00);
    /*  BusFault Status Register (BFSR) CFSR[15:8] indicates the cause of BusFault
     *  [15] BFARVALID = 1 BFAR holds a valid fault address
     *  [13] LSPERR    = 1 Bus fault occurred during floating-point lazy state preservation
     *  [12] STKERR    = 1 Stacking for an exception entry has caused BusFault, BFAR address is not valid
     *  [11] UNSTKERR  = 1 Unstack for an exception return has caused BusFault, BFAR address is not valid
     *  [10] IMPRECISERR = 1 Imprecise data bus error, BFAR address is not valid
     *  [9]  PRECISERR = 1 Precise data bus error, BFAR address is valid
     *  [8]  IBUSERR   = 1 error on prefetching an instruction, BFAR address is invalid
     * */

    if(BFSR & S32_SCB_CFSR_BFARVALID_MASK)
    {                          // If the address in BFAR register is valid
        BFAR = S32_SCB->BFAR;  // BFAR holds the address of the location that generated the BusFault
    }

    __asm__("BKPT");

//    while(1)
//    {
//
//    }
}

/*******************************************************************************
Function Name : HardFault_Handler
Parameters    : NONE
Returns       : NONE
Notes         : declared with __attribute__((naked))
 *******************************************************************************/
void HardFault_Handler(void)
{
    __asm__("MOVS R0, #4");
    __asm__("MOV R1, LR");
    __asm__("TST R1, R0");     // Test LR (EXC_RETURN[2])
    __asm__("ITE NE");
    __asm__("MRSNE R1, PSP");  // EXC_RETURN[2] = 1
    __asm__("MRSEQ R1, MSP");  // EXC_RETURN[2] = 0

    __asm__("LDR R0, =stack_pointer");
    __asm__("STR R1, [R0]"); // Store PSP into stack_pointer

    p_stack_frame = (uint32_t*)stack_pointer;
    program_counter  = p_stack_frame[6];  // holds the address of the instruction that has caused the exception

    HFSR = S32_SCB->HFSR;
    /*  Hard Fault Status Register
     *  [30] FORCED  = 1 Indicates a forced, escalated fault from other fault handlers
     *  [1]  VECTTBL = 1 Indicates a BusFault on a vector table read during exception processing
     *                   This bus error is always handled by the hard fault handler.
     *                   the PC value return points to the instruction that was preempted by the exception.
     * */

    if(HFSR & S32_SCB_HFSR_FORCED_MASK)
    {
        SHCSR = S32_SCB->SHCSR;
        /*  System Handler Control and State Register shows the escalated exception
         *   Other fault status registers must be read to find the cause.
         *   [3] USGFAULTACT = 1 // UsageFault pending
         *   [1] BUSFAULTACT = 1 // BusFault pending
         *   [0] MEMFAULTACT = 1 // MemManageFault pending
         * */
    }

    /******************** Usage Fault *****************************************/
    UFSR = (S32_SCB->CFSR & 0xFFFF0000);
    /*   UsageFault Status Register (UFSR) CFSR[31:16] indicates the cause of UsageFault
     *   [25] DIVBYZERO = 1 Executed an SDIV or UDIV instruction with a divisor of 0
     *   [24] UNALIGNED = 1 Made an unaligned memory access
     *   [19] NOCP      = 1 Attempted an access to a coprocessor if the access is denied or privileged only (configurable in CPACR)
     *   [18] INVPC     = 1 Attempted an illegal load of EXC_RETURN to the PC, as a result of an invalid context, or an invalid EXC_RETURN value
     *   [17] INVSTATE  = 1 Attempted to execute an instruction that makes illegal use of the EPSR
     *   [16] UNDEFINSTR = 1 Attempted to execute an undefined instruction
     * */

    /******************** Bus Fault *******************************************/
    BFSR = (S32_SCB->CFSR & 0x0000FF00);
    /*  BusFault Status Register (BFSR) CFSR[15:8] indicates the cause of BusFault
     *  [15] BFARVALID = 1 BFAR holds a valid fault address
     *  [13] LSPERR    = 1 Bus fault occurred during floating-point lazy state preservation
     *  [12] STKERR    = 1 Stacking for an exception entry has caused BusFault, BFAR address is not valid
     *  [11] UNSTKERR  = 1 Unstack for an exception return has caused BusFault, BFAR address is not valid
     *  [10] IMPRECISERR = 1 Imprecise data bus error, BFAR address is not valid
     *  [9]  PRECISERR = 1 Precise data bus error, BFAR address is valid
     *  [8]  IBUSERR   = 1 Error on prefetching an instruction, BFAR address is invalid
     * */

    if(BFSR & S32_SCB_CFSR_BFARVALID_MASK)
    {
        // Bus Fault Address Register
        BFAR = S32_SCB->BFAR; // BFAR [31:0] ADDRESS holds the address of the location that generated the BusFault
    }

    /******************** MemManage Fault *************************************/
    MMFSR = (S32_SCB->CFSR & 0x000000FF);
    /*  MemManage Fault Status Register (MMFSR) CFSR[7:0] indicates the cause of MemManage fault
     *  [7]  MMARVALID = 1 MMFAR holds a valid fault address
     *  [5]  MLSPERR   = 1 MemManage fault occurred during floating-point lazy state preservation
     *  [4]  MSTKERR   = 1 Stacking error caused by ARM MPU violation, MMAR address is not valid
     *  [3]  MUNSTKERR = 1 Unstacking error caused by ARM MPU violation, MMAR address is not valid
     *  [1]  DACCVIOL  = 1 The processor attempted a load or store at a protected (by ARM MPU) location, MMAR address is valid
     *  [0]  IACCVIOL  = 1 The processor attempted an instruction fetch from a protected (by ARM MPU) location, MMAR address is not valid
     */
    if(MMFSR & S32_SCB_CFSR_MMARVALID_MASK)
    { // Indicates whether the value in the MMFAR is valid
        // MemManage Fault Address Register
        MMFAR = S32_SCB->MMFAR; // [31:0] ADDRESS (this field holds the address of the location that generated the MemManage fault)
    }

    __asm__("BKPT");

//    while(1)
//    {
//
//    }
}


/*******************************************************************************
Function Name : enable_fault_handlers
Parameters    : NONE
Returns       : NONE
Notes         : NONE
 *******************************************************************************/
void enable_fault_handlers(void)
{
    S32_SCB->SHCSR  |= S32_SCB_SHCSR_USGFAULTENA_MASK   // [19] USGFAULTENA = 1 UsageFault enabled
                    |  S32_SCB_SHCSR_BUSFAULTENA_MASK   // [18] BUSFAULTENA = 1 BusFault enabled
                    |  S32_SCB_SHCSR_MEMFAULTENA_MASK;  // [17] MEMFAULTENA = 1 MemManage enabled
}

/*******************************************************************************
Function Name : enable_div_by_zero_trap
Parameters    : NONE
Returns       : NONE
Notes         : Enables faulting when the processor executes
                an SDIV or UDIV instruction with a divisor of 0
 *******************************************************************************/
void enable_div_by_zero_trap(void)
{
    S32_SCB->CCR    |= S32_SCB_CCR_DIV_0_TRP_MASK;
    // [4] DIV_0_TRP   = 1
}

/*******************************************************************************
Function Name : enable_unaligned_access_trap
Parameters    : NONE
Returns       : NONE
Notes         : Enables unaligned access traps
 *******************************************************************************/
void enable_unaligned_access_trap(void)
{
    S32_SCB->CCR    |= S32_SCB_CCR_UNALIGN_TRP_MASK;
    // [3] UNALIGN_TRP = 1
}

/*******************************************************************************
Function Name : set_fault_handlers_priority
Parameters    : NONE
Returns       : NONE
Notes         : Fields PRI_4, PRI_5 and PRI_6 in SHPR1 core register are byte-accessible
                However, this device support only 16 priority level
                configurable in the four most significant bits.
                If set, the lower number, the higher priority
 *******************************************************************************/
void set_fault_handlers_priority(void)
{
    S32_SCB->SHPR1  |= S32_SCB_SHPR1_PRI_4(0x10)    // MemManage priority 1
                    |  S32_SCB_SHPR1_PRI_5(0x10)    // BusFault priority 1
                    |  S32_SCB_SHPR1_PRI_6(0x10);   // UsageFault priority 1
    // HardFualt Handler has priority always set to -1
}

/*******************************************************************************
Function Name : disable_write_buffer
Parameters    : NONE
Returns       : NONE
Notes         : If write buffer is disabled, all imprecise bus faults are forced to be precise,
                but it might decreases the CPU performance.
                Recommended only for debugging.
 *******************************************************************************/
void disable_write_buffer(void)
{
    S32_SCB->ACTLR |= S32_SCB_ACTLR_DISDEFWBUF_MASK;
    // [1] DISDEFWBUF = 1 write buffer disabled
}
