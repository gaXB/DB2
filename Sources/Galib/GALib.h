/************************�ļ�˵��*****************************************************
* Copyright (c) 2019 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�GALib.h
* ����������ȫ����ͷ�ļ���
* 
* �������� codewarrior IDE 10.7-KEA
* MCU˵������MCU�޹�
*/
/**************************���ļ�¼******************************************************
 * 
* �汾��1.0
* ˵������ʼ�汾����
* ԭ���ߣ�xb
* ������ڣ�2019��7��25��
*
* �汾��1.3
* ������ļ�˵���ļ�
* ���ߣ�xb
* ������ڣ�2020��1��6��
* 
* �汾��1.4
* AD ģ��������1.1 
* ADInit ԭ�͸ı�
* ���ߣ�xb
* ������ڣ�2020��3��28��
* 
* �汾��1.5
* keypad 1.3
* ht16kk23 1.1
* ���ߣ�xb
* ������ڣ�2020��6��17��
* 
* * �汾��1.6
* 1 �����λ��ģ��
* 
* ���ߣ�xb
* ������ڣ�2020��7��17��
* 
* * * �汾��1.7
* 1 ������1.1
* 2 ���1.1
* ����0xffΪͣ��
* ���ߣ�xb
* ������ڣ�2020��10��21��
* 
* �汾��1.8
* 1 ���1.2
* 2 �ķ�� 1.4
* �������BUG
* ���ߣ�xb
* ������ڣ�2020��12��2��
* 
* �汾��1.9
* 1 sigfilter v1.2
* 2 motor  v1.3
* 3 fancontrol  v1.5
* ���ߣ�xb
* ������ڣ�2020��12��17��
*/


#ifndef GALIB_H_
#define GALIB_H_

#include "DataType.h"
#include "Encoder28.h"
#include	"FanControl.h"
#include	"Ht16k23_Drive.h"
#include	"IIC_Drive_Software.h"
#include "mcp7708.h"
#include	"SPI_Drive.h"
#include "Encoder35.h"
#include "EncoderESE24.h"
#include "ComdQueue.h"
#include "ADTreat.h"
#include	"keypad.h"
#include	"mathGA.h"
#include "motor.h" 
#include "ncfmotor.h"
#include "SignalFilter.h"
#include "timerout.h"
#include "DealString.h"
#include "FanControl.h"
#include "PTMETER_Fun.h"

#endif /* GALIB_H_ */
