/**************************�ļ�˵��*****************************************************
* Copyright (c) 2019 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�SignalFilter.h
* �����������ź��˲��ȶ�ģ��ͷ�ļ��� ��Ҫ��ʱ��ģ��
* �������� codewarrior IDE 10.7-KEAZ128
* MCU˵�������ļ���MCU�޹�
* 
* */
/**************************���ļ�¼******************************************************
 * 
* �汾��1.0
* ˵������ʼ�汾������ ��ԭģ���1.0��ֲ
* ԭ���ߣ�xb
* ������ڣ�2019��5��15��
* 
* �汾��1.1
* ˵��������SIG_MAX_NUM ��10��Ϊ15
* ���ߣ�xb
* ������ڣ�2020��1��6��
* 
* �汾��1.2
* ˵����
* 1 ����05���˲������¶Ȼز��
* 2 �Ż�-��ʱ����������
* ���ߣ�xb
* ������ڣ�2020��12��4��
*/

#ifndef  SIGNAL_FILTER_H_
#define  SIGNAL_FILTER_H_

#include "timerout.h"

#define SIG_MAX_NUM            15        //���֧����

#define COLLECT_PRIDE_TIME     25        //�ɼ����ڣ� ms
#define COLLECT_TIMES           8        //�ɼ��˴�    �ܹ�ʱ��Ϊ COLLECT_PRIDE_TIME �� COLLECT_TIMES
#define COLLECT_CYCTIME     (COLLECT_PRIDE_TIME * COLLECT_TIMES)
#define COLLECT_WEIGHT         65       //���ε�Ȩ�� %
/*********************�����ı����ֽ�************************/


void SigFControl(void);
BOOL SigF_Init(uint8 u8MaxNum);
int16 SigF_GetResult(uint8 u8SigNum);
int16 SigF_GetResult05(uint8 u8SigNum);
#endif
