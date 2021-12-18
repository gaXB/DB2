/*  BEGIN_FILE_HDR
 ********************************************************************************
 *   NOTICE                              
 *   This software is the property of HiRain Technologies. Any information 
 *   contained in this doc should not be reproduced, or used, or disclosed 
 *   without the written authorization from HiRain Technologies.
 ********************************************************************************
 *   File Name       : Platform_Types.h
 ********************************************************************************
 *   Project/Product : Common
 *   Title           : Platform_Types.h 
 *   Author          : ning.chen
 ********************************************************************************
 *   Description     : platform related types                         
 *
 ********************************************************************************
 *   Limitations     : only used for Freescale MPC5xxx
 *
 ********************************************************************************
 *
 ********************************************************************************
 *   Revision History:
 * 
 *   Version     Date          Initials      CR#          Descriptions
 *   ---------   ----------    ------------  ----------   ---------------
 *   01.00.00    13/06/2013    ning.chen     N/A          Original
 *   01.00.01    17/07/2013    ning.chen     N/A          DPN_COMMON_130717_01
 *   01.00.02    15/08/2013    ning.chen     N/A          D10_COMMON52_130815_01
 *   01.00.03    08/01/2014    ning.chen     N/A          D10_COMMON52_140108_01
 ********************************************************************************
 * END_FILE_HDR*/

#ifndef _PLATFORM_TYPES_H_
#define _PLATFORM_TYPES_H_

/*******************************************************************************
 *   Includes 
 *******************************************************************************/

/*******************************************************************************
 *   Macro 
 *******************************************************************************/
#define CPU_TYPE_8       (8)
#define CPU_TYPE_16      (16)
#define CPU_TYPE_32      (32)

#define MSB_FIRST        (0)    /* big endian bit ordering */
#define LSB_FIRST        (1)    /* little endian bit ordering */

#define HIGH_BYTE_FIRST  (0)    /* big endian byte ordering */
#define LOW_BYTE_FIRST   (1)    /* little endian byte ordering */

#ifndef TRUE
#define TRUE   (1)
#endif

#ifndef FALSE
#define FALSE  (0)
#endif

#define CPU_TYPE            CPU_TYPE_16 
#define CPU_BIT_ORDER       LSB_FIRST 
#define CPU_BYTE_ORDER      HIGH_BYTE_FIRST                      

/*******************************************************************************
 *   Typedef 
 *******************************************************************************/
typedef unsigned char boolean;

typedef signed char sint8;
typedef unsigned char uint8;
typedef signed short sint16;
typedef unsigned short uint16;
typedef signed long sint32;
typedef unsigned long uint32;
/*******************************************************************************
 * PRQA S 1207 MISRA-C:2004 Rule 1.1
 * This "long long" type only used in the 56xx Platform_Types.h, because the 
 * flash driver need this type to access flash.
 * This part of code is verified manually and has no impact.
 *******************************************************************************/
typedef signed long long sint64;
typedef unsigned long long uint64;

typedef unsigned long uint8_least;
typedef unsigned long uint16_least;
typedef unsigned long uint32_least;
typedef signed long sint8_least;
typedef signed long sint16_least;
typedef signed long sint32_least;

typedef float float32;
typedef double float64;

#endif  /* _PLATFORM_TYPES_H_ */

