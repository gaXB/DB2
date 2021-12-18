/************************�ļ�˵��*****************************************************
* Copyright (c) 2019 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�keypad.h
* �������������� ģ���ͷ�ļ���
* Ϊ��ģ��������������Ҫ���� ��Ҫ��ʱ��ģ��
*
* �������� codewarrior IDE 10.7-KEA
* MCU˵������MCU�޹�
*/
/**************************���ļ�¼******************************************************
 *
* �汾��1.0
* ˵������ʼ�汾����
* ԭ���ߣ�xb
* ������ڣ�2019��5��9��
*
* �汾��1.1
* ˵�������˫����4���������Ż�
* ԭ���ߣ�xb
* ������ڣ�2020��5��15��
*
* �汾��1.2
* ˵��������AC��ѭ��4�������ⲿ����汾���
* ԭ���ߣ�lyc
* ������ڣ�2020��5��19��
* 
* �汾��1.3
* ˵����
* 1 ������ѭ������Ϊ���������ܣ�������ʾ�ͻ��汾��
* 2 ����AC ��������֣�Ϊ�汾�Ÿߵ�λ��
* ���ߣ�xb
* ������ڣ�2020��6��3��
*/
#ifndef KEY_PAD__
#define KEY_PAD__
#include"DataType.h"


//�����ֵ
#define   KEY_CODE_NO                  0        //�޼�����
#define   KEY_CODE_INVALID             0xFF     //��ֵ��Ч
#define   KEY_CODE_TREMBLE             0xFFFFu   //������
#define   KEY_CODE_CONTINUE1           0x8000u    //�˼�ֵ������������
#define   KEY_CODE_CONTINUE2           0x4000u    //�˼�ֵ������������
#define   KEY_CODE_LONG                0x2000u   //�˼�ֵ�г���������,��Ӧ��������ֵΪ0x20FF
#define   KEY_CODE_COMBFOUR            0x1800u   //�˼�ֵ��4������

#define   KEY_VER_L	      	         0x0001u   //�汾�ż��ĵ�λ��ֵ
#define   KEY_VER_H              	   0x0002u   //�汾�ż��ĸ�λ��ֵ
#define   KEY_CODE_VER_COMBIN          0x0003u   //���ֵ

#define   KEY_CODE_SPEC                0x00FFu   //�Ѻ�8λ��Ϊ���⹦����Ӧ�ļ�����������㷵��һ�������ļ�ֵ������Ϊ�����
#define   KEY_CODE_SPEC_H					0xFF00u
#define   KEY_CODE_SPEC_H1					0xFE00u

#define   KEY_CODE_COMBFOUR_VERH       (KEY_CODE_SPEC_H+KEY_CODE_VER_COMBIN)    //����λ����4����λ
#define   KEY_CODE_COMBFOUR_VERL       (KEY_CODE_SPEC_H1+KEY_CODE_VER_COMBIN)   //����λ����4����λ
#define   KEY_VERH_LONG                (KEY_VER_H+KEY_CODE_SPEC_H)
//����״̬
#define   CONKEY_STATE_BEGIN       0   //��ʼ����������ʱ
#define   CONKEY_STATE_CONTINUE    1   //������Ӧ����

//���̹��ϴ���
#define KEY_ERROR_NONE      0x00   //��������
#define KEY_ERROR_INVALID   0x01   //��Ч��
#define KEY_ERROR_LONGDOWN  0x02   //ĳ��������
#define KEY_ERROR_READ	    0X03    //�����������������


//���ܼ�ʱ�䶨��
#define   CONTINUEKEY_SEG1_TIME     7   //������������Ϊ0.7s
#define   CONTINUEKEY_SEG2_TIME     3   //������ӦΪ 0.3s
#define   COMBFOUR_TIME             30  //4���ڶ���ʱ���ڡ�*0.1
#define   LONGKEY_TIME              50  //��������Ϊ 5s

//�˲�ʱ��
#define  KEY_FILTER_TIME         20      //�˲�ʱ�� ms
#define  KEY_LONG_FILTER_TIME    300     //��ʱ�䰴�˼���Ϊ���ϣ�ʱ�� 0.1
#define  NOKEY_LONG_FILTER_TIME  10      //�޼�ʱ��ʱ�䰴���Ļָ� ʱ�� 0.1

#define  RESKEY_BUFFERNUM        3       //������Ӧ�Ļ�������

uint16 Read_Keydate(void);
uint8 Read_Key_Errcode(void);
void Treat_Read_Keypad(void);
void Respond_Keydata(uint16 keycode);
#endif
