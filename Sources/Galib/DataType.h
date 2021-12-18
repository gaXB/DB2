/************************文件说明*****************************************************
* Copyright (c) 2019 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：DataType.h
* 功能描述： 数据类型的头文件。
* 编写程序的基本类型请符号以下，不能用编译器的基本类型
* 
* 编译器： codewarrior IDE 10.7-KEA
* MCU说明：于MCU无关
*/
/**************************更改记录******************************************************
 * 
* 版本：1.0
* 说明：初始版本建立
* 原作者：xb
* 完成日期：2019年6月3日
* 
*/
#ifndef	TYPE_DEFINITION__
#define	TYPE_DEFINITION__ 

//基本类型
typedef	unsigned	char	uint8;
typedef	unsigned	short	uint16;
typedef	unsigned	long	uint32;

typedef	signed	char	int8;
typedef	signed	short	int16;
typedef	signed	long	int32;

//布尔类型
typedef	unsigned	char	BOOL;
#ifndef TRUE
#define TRUE   (1)
#endif

#ifndef FALSE
#define FALSE  (0)
#endif

//位，标记类型
typedef enum {RESET = 0, SET = !RESET} FlagStatus, BitStatus;
//开关类型
typedef enum {OFF = 0, ON = !OFF} SwitchStatus;
//PIN OUT
typedef enum {PIN_LOW = 0, PIN_HIGH = !PIN_LOW} PinOutState;


//空指针定义
#define  PDATA_NULL      (0)    


//电机运行状态
#define  MOTOR_RUN_HALT    0X00
#define  MOTOR_RUN_FORWARD 0x01
#define  MOTOR_RUN_BACKWARD   0x02

#endif

 
 
