/*  BEGIN_FILE_HDR
 ********************************************************************************
 *   NOTICE                              
 *   This software is the property of HiRain Technologies. Any information 
 *   contained in this doc should not be reproduced, or used, or disclosed 
 *   without the written authorization from HiRain Technologies.
 ********************************************************************************
 *   File Name       : Bsw_Common.h
 ********************************************************************************
 *   Project/Product : AUTOSAR 4.0
 *   Title           : Bsw Common File
 *   Author          : Hirain
 ********************************************************************************
 *   Description     : Bsw_Common header file.                    
 *
 ********************************************************************************
 *   Limitations     : None
 *
 ********************************************************************************
 *
 ********************************************************************************
 *   Revision History:
 *   
 ********************************************************************************
 * END_FILE_HDR*/

#ifndef _BSW_COMMON_H_
#define _BSW_COMMON_H_
/*******************************************************************************
 *   Includes 
 *******************************************************************************/
#include "Std_Types.h"

#define BSW_START_SEC_CODE
#include "MemMap.h"
/*******************************************************************************
 *   Declare 
 *******************************************************************************/
extern FUNC(void, BSW_CODE) Bsw_MemCpy(
CONSTP2VAR(uint8, AUTOMATIC,BSW_APPL_DATA) dest,
CONSTP2CONST(uint8, AUTOMATIC,BSW_APPL_DATA) src,
const uint32 size
);
extern FUNC(void, BSW_CODE) Bsw_MemSet(
CONSTP2VAR(uint8, AUTOMATIC,BSW_APPL_DATA) dest,
const uint8 src,
const uint32 size
);
#define BSW_STOP_SEC_CODE
#include "MemMap.h"

#endif
