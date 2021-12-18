/************************�ļ�˵��*****************************************************
* Copyright (c) 2019 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�DataType.h
* ���������� �������͵�ͷ�ļ���
* ��д����Ļ���������������£������ñ������Ļ�������
* 
* �������� codewarrior IDE 10.7-KEA
* MCU˵������MCU�޹�
*/
/**************************���ļ�¼******************************************************
 * 
* �汾��1.0
* ˵������ʼ�汾����
* ԭ���ߣ�xb
* ������ڣ�2019��6��3��
* 
*/
#ifndef	TYPE_DEFINITION__
#define	TYPE_DEFINITION__ 

//��������
typedef	unsigned	char	uint8;
typedef	unsigned	short	uint16;
typedef	unsigned	long	uint32;

typedef	signed	char	int8;
typedef	signed	short	int16;
typedef	signed	long	int32;

//��������
typedef	unsigned	char	BOOL;
#ifndef TRUE
#define TRUE   (1)
#endif

#ifndef FALSE
#define FALSE  (0)
#endif

//λ���������
typedef enum {RESET = 0, SET = !RESET} FlagStatus, BitStatus;
//��������
typedef enum {OFF = 0, ON = !OFF} SwitchStatus;
//PIN OUT
typedef enum {PIN_LOW = 0, PIN_HIGH = !PIN_LOW} PinOutState;


//��ָ�붨��
#define  PDATA_NULL      (0)    


//�������״̬
#define  MOTOR_RUN_HALT    0X00
#define  MOTOR_RUN_FORWARD 0x01
#define  MOTOR_RUN_BACKWARD   0x02

#endif

 
 
