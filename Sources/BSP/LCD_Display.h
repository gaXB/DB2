/************************�ļ�˵��*****************************************************
* Copyright (c) 2020 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�Lcd_Display.h
* ����������LCDͷ�ļ� 
* �������� codewarrior IDE 11.1
* MCU˵������ģ��Ϊ���Ʋ��MCU ֮��Ľӿ�
*/        
/**************************���ļ�¼******************************************************
*
* �汾��2.1
* ���ߣ�xb
* ������ڣ�20200603
* 1 LCD �������
* 2 �Ż�LCD��ʾ
*
*/

#ifndef LCD_DISPLAY_H_
#define LCD_DISPLAY_H_

//Һ����ѡ��
#define   _HT_16K23_
//#define   _HT_1621_

//LCD ��ʾģʽ
 typedef enum  {									
   LCD_MODE_NORMAL=0,      //����
	LCD_MODE_ALL,           //������ʾ
	LCD_MODE_CLEAR,         //�������
}LCDModeType;


//LCD seg����ʾ���ݣ����ݽṹ����
typedef struct
{
	int16 i16Temp;         //�¶�����ʾ��-400 ~800 Ϊֱ����ʾ����Ҫlohi��Ҫ����
	uint8 SegAC;           //LCD��ʾ����ac
	uint8 SegMode;         //0-4 osf->ost
	uint8 SegNCF;
	uint8 SegFanLevel;
	uint8 SegAuto;
}LCDDATA;


//�����¶���ʾ��ʽ
#define TSET_LCD_OFF   0x7fff       // �����¶Ȳ���ʾ
#define TSET_LCD_LO    0x7ffe       // ��ʾ lo
#define TSET_LCD_HI    0x7ffd       // ��ʾHI

//LCD ʵ�ֺ���
void DisTsetShakeSet(int16 i16Temp);
void SetLcdDisplayMode(LCDModeType mode);
void SetLcdDisplayData(LCDDATA *pLCDData);
void Clear_LCD(void);
void Updata_LCD(void);
void Init_LCD(void);
#endif /* LCD_DISPLAY_H_ */
