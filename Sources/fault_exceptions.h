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
* Test HW:         S32144EVB-Q100
* MCU:             S32K144 0N57U
* Debugger:        S32DS 2.2, OpenSDA
* Target:          internal_FLASH
*************************************************************************************************
* Revision History:
* 1.0      May-30-2017   Daniel Martynek
* 2.0      Aug-15-2020   Daniel Martynek
************************************************************************************************/

#ifndef FAULT_EXCEPTIONS_H_
#define FAULT_EXCEPTIONS_H_

/*******************************************************************************
* Function prototypes
*******************************************************************************/
void enable_fault_handlers(void);
void set_fault_handlers_priority(void);
void disable_write_buffer(void);
void enable_div_by_zero_trap(void);
void enable_unaligned_access_trap(void);
__attribute__((naked)) void UsageFault_Handler(void);
__attribute__((naked)) void UsageFault_Handler(void);
__attribute__((naked)) void UsageFault_Handler(void);

#endif /* FAULT_EXCEPTIONS_H_ */

