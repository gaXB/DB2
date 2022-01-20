/************************�ļ�˵��*****************************************************
* Copyright (c) 2020 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�LCD_MAP.h
* ����������LCD ��ӳ��
* �������� codewarrior IDE 11.1
* MCU˵������ģ��Ϊ���Ʋ��MCU ֮��Ľӿ�
*/        
/**************************���ļ�¼******************************************************
* 
* �汾��1.0
* ���ߣ�xb
* ������ڣ�2020��4��1��
* 1 ��ʼ�汾
* 
* �汾��2.0
* ���ߣ�xb
* ������ڣ�2020��4��1��
* 2 ����LCD_Display.c
* 
* �汾��2.1
* ���ߣ�xb
* ������ڣ�2021��3��15��
* 2 ֧��HT1621��HT16K23
*/
#ifndef LCD_MAP_H_
#define LCD_MAP_H_


//��ʾIC�����ֽṹ�嶨��
typedef  union
{
   uint8  Byte;
   struct      
   {
         uint8 b0 :1;
         uint8 b1 :1;
         uint8 b2 :1;
         uint8 b3 :1;
         uint8 b4 :1;
         uint8 b5 :1;
         uint8 b6 :1;
         uint8 b7 :1;
    }Bits;
} lcddata;


#define   HTDATA_FACTNUM    10
lcddata   htdata[HTDATA_FACTNUM];   
//ht16k23LCD������ʽΪ20*4
//seg0��seg1   adress:0x00

//seg0 1
#define		HT1621_SEG0_D0      htdata[0].Bits.b7	
#define		HT1621_SEG0_D1      htdata[0].Bits.b6		
#define		HT1621_SEG0_D2      htdata[0].Bits.b5		
#define		HT1621_SEG0_D3      htdata[0].Bits.b4		
#define		HT1621_SEG1_D0      htdata[0].Bits.b3		
#define		HT1621_SEG1_D1      htdata[0].Bits.b2	
#define		HT1621_SEG1_D2      htdata[0].Bits.b1
#define		HT1621_SEG1_D3      htdata[0].Bits.b0

//seg2 3
#define		HT1621_SEG2_D0       htdata[1].Bits.b7	
#define		HT1621_SEG2_D1       htdata[1].Bits.b6		
#define		HT1621_SEG2_D2       htdata[1].Bits.b5		
#define		HT1621_SEG2_D3       htdata[1].Bits.b4		
#define		HT1621_SEG3_D0       htdata[1].Bits.b3		
#define		HT1621_SEG3_D1       htdata[1].Bits.b2	
#define		HT1621_SEG3_D2       htdata[1].Bits.b1
#define		HT1621_SEG3_D3       htdata[1].Bits.b0

//seg4 5
#define		HT1621_SEG4_D0       htdata[2].Bits.b7	
#define		HT1621_SEG4_D1       htdata[2].Bits.b6		
#define		HT1621_SEG4_D2       htdata[2].Bits.b5		
#define		HT1621_SEG4_D3       htdata[2].Bits.b4		
#define		HT1621_SEG5_D0       htdata[2].Bits.b3		
#define		HT1621_SEG5_D1       htdata[2].Bits.b2	
#define		HT1621_SEG5_D2       htdata[2].Bits.b1
#define		HT1621_SEG5_D3       htdata[2].Bits.b0	

//seg6 7
#define		HT1621_SEG6_D0       htdata[3].Bits.b7	
#define		HT1621_SEG6_D1       htdata[3].Bits.b6		
#define		HT1621_SEG6_D2       htdata[3].Bits.b5		
#define		HT1621_SEG6_D3       htdata[3].Bits.b4		
#define		HT1621_SEG7_D0       htdata[3].Bits.b3		
#define		HT1621_SEG7_D1       htdata[3].Bits.b2	
#define		HT1621_SEG7_D2       htdata[3].Bits.b1
#define		HT1621_SEG7_D3       htdata[3].Bits.b0


//seg8 9
#define		HT1621_SEG8_D0       htdata[4].Bits.b7	
#define		HT1621_SEG8_D1       htdata[4].Bits.b6		
#define		HT1621_SEG8_D2       htdata[4].Bits.b5		
#define		HT1621_SEG8_D3       htdata[4].Bits.b4		
#define		HT1621_SEG9_D0       htdata[4].Bits.b3		
#define		HT1621_SEG9_D1       htdata[4].Bits.b2	
#define		HT1621_SEG9_D2       htdata[4].Bits.b1
#define		HT1621_SEG9_D3       htdata[4].Bits.b0		

//seg10 11
#define		HT1621_SEG10_D0       htdata[5].Bits.b7	
#define		HT1621_SEG10_D1       htdata[5].Bits.b6		
#define		HT1621_SEG10_D2       htdata[5].Bits.b5		
#define		HT1621_SEG10_D3       htdata[5].Bits.b4		
#define		HT1621_SEG11_D0       htdata[5].Bits.b3		
#define		HT1621_SEG11_D1       htdata[5].Bits.b2	
#define		HT1621_SEG11_D2       htdata[5].Bits.b1
#define		HT1621_SEG11_D3       htdata[5].Bits.b0

//seg12 13
#define		HT1621_SEG12_D0       htdata[6].Bits.b7	
#define		HT1621_SEG12_D1       htdata[6].Bits.b6		
#define		HT1621_SEG12_D2       htdata[6].Bits.b5		
#define		HT1621_SEG12_D3       htdata[6].Bits.b4		
#define		HT1621_SEG13_D0       htdata[6].Bits.b3		
#define		HT1621_SEG13_D1       htdata[6].Bits.b2	
#define		HT1621_SEG13_D2       htdata[6].Bits.b1
#define		HT1621_SEG13_D3       htdata[6].Bits.b0	

//seg14 15
#define		HT1621_SEG14_D0       htdata[7].Bits.b7	
#define		HT1621_SEG14_D1       htdata[7].Bits.b6		
#define		HT1621_SEG14_D2       htdata[7].Bits.b5		
#define		HT1621_SEG14_D3       htdata[7].Bits.b4		
#define		HT1621_SEG15_D0       htdata[7].Bits.b3		
#define		HT1621_SEG15_D1       htdata[7].Bits.b2	
#define		HT1621_SEG15_D2       htdata[7].Bits.b1
#define		HT1621_SEG15_D3       htdata[7].Bits.b0

//seg16��seg17   adress:0x08
#define		HT1621_SEG16_D0       htdata[8].Bits.b7
#define		HT1621_SEG16_D1       htdata[8].Bits.b6
#define		HT1621_SEG16_D2       htdata[8].Bits.b5
#define		HT1621_SEG16_D3       htdata[8].Bits.b4
#define		HT1621_SEG17_D0       htdata[8].Bits.b3
#define		HT1621_SEG17_D1       htdata[8].Bits.b2
#define		HT1621_SEG17_D2       htdata[8].Bits.b1
#define		HT1621_SEG17_D3       htdata[8].Bits.b0

//seg18��seg19   adress:0x09
#define		HT1621_SEG18_D0       htdata[9].Bits.b7
#define		HT1621_SEG18_D1       htdata[9].Bits.b6
#define		HT1621_SEG18_D2       htdata[9].Bits.b5
#define		HT1621_SEG18_D3       htdata[9].Bits.b4
#define		HT1621_SEG19_D0       htdata[9].Bits.b3
#define		HT1621_SEG19_D1       htdata[9].Bits.b2
#define		HT1621_SEG19_D2       htdata[9].Bits.b1
#define		HT1621_SEG19_D3       htdata[9].Bits.b0

#define		HT1621_SEG20_D0       htdata[10].Bits.b7
#define		HT1621_SEG20_D1       htdata[10].Bits.b6
#define		HT1621_SEG20_D2       htdata[10].Bits.b5
#define		HT1621_SEG20_D3       htdata[10].Bits.b4
#define		HT1621_SEG21_D0       htdata[10].Bits.b3
#define		HT1621_SEG21_D1       htdata[10].Bits.b2
#define		HT1621_SEG21_D2       htdata[10].Bits.b1
#define		HT1621_SEG21_D3       htdata[10].Bits.b0

#define		HT1621_SEG22_D0       htdata[11].Bits.b7
#define		HT1621_SEG22_D1       htdata[11].Bits.b6
#define		HT1621_SEG22_D2       htdata[11].Bits.b5
#define		HT1621_SEG22_D3       htdata[11].Bits.b4
#define		HT1621_SEG23_D0       htdata[11].Bits.b3
#define		HT1621_SEG23_D1       htdata[11].Bits.b2
#define		HT1621_SEG23_D2       htdata[11].Bits.b1
#define		HT1621_SEG23_D3       htdata[11].Bits.b0
/*********************************��������Ҫ���Ĳ���******************************************/
#define  LCD_FAN1    HT1621_SEG0_D0
#define  LCD_FAN2    HT1621_SEG0_D1
#define  LCD_FAN3    HT1621_SEG0_D2
#define  LCD_FAN4    HT1621_SEG0_D3
#define  LCD_FAN8    HT1621_SEG1_D0
#define  LCD_FAN7    HT1621_SEG1_D1
#define  LCD_FAN6    HT1621_SEG1_D2
#define  LCD_FAN5    HT1621_SEG1_D3

#define   LCD_FANL   HT1621_SEG7_D0
#define   LCD_OSD    HT1621_SEG7_D1//��
#define   LCD_OST    HT1621_SEG7_D2//��˪
#define   LCD_OSF    HT1621_SEG7_D3//ͷ

#define   LCD_NE     HT1621_SEG6_D0//����
#define   LCD_1E     HT1621_SEG6_D1
#define   LCD_1G     HT1621_SEG6_D2
#define   LCD_1F     HT1621_SEG6_D3

#define   LCD_1D     HT1621_SEG5_D0
#define   LCD_1C     HT1621_SEG5_D1
#define   LCD_1B     HT1621_SEG5_D2
#define   LCD_1A     HT1621_SEG5_D3

#define   LCD_N0     HT1621_SEG4_D0
#define   LCD_2E     HT1621_SEG4_D1
#define   LCD_2G     HT1621_SEG4_D2
#define   LCD_2F     HT1621_SEG4_D3

#define   LCD_2D     HT1621_SEG3_D0
#define   LCD_2C     HT1621_SEG3_D1
#define   LCD_2B     HT1621_SEG3_D2
#define   LCD_2A     HT1621_SEG3_D3

#define   LCD_BODY   HT1621_SEG2_D0
#define   LCD_N1     HT1621_SEG2_D1
#define   LCD_CEL     HT1621_SEG2_D2//�¶ȷ���
#define   LCD_AUTO   HT1621_SEG2_D3

#endif /* LCD_MAP_H_ */
