/************************文件说明*****************************************************
* Copyright (c) 2020 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：Lcd_Display.h
* 功能描述：LCD头文件 
* 编译器： codewarrior IDE 11.1
* MCU说明：此模块为控制层和MCU 之间的接口
*/        
/**************************更改记录******************************************************
*
* 版本：2.1
* 作者：xb
* 完成日期：20200603
* 1 LCD 开机清空
* 2 优化LCD显示
*
*/

#ifndef LCD_DISPLAY_H_
#define LCD_DISPLAY_H_

//液晶屏选择
#define   _HT_16K23_
//#define   _HT_1621_

//LCD 显示模式
 typedef enum  {									
   LCD_MODE_NORMAL=0,      //正常
	LCD_MODE_ALL,           //所有显示
	LCD_MODE_CLEAR,         //所有清楚
}LCDModeType;


//LCD seg段显示内容，数据结构定义
typedef struct
{
	int16 i16Temp;         //温度区显示，-400 ~800 为直接显示，需要lohi需要设置
	uint8 SegAC;           //LCD显示区的ac
	uint8 SegMode;         //0-4 osf->ost
	uint8 SegNCF;
	uint8 SegFanLevel;
	uint8 SegAuto;
}LCDDATA;


//设置温度显示方式
#define TSET_LCD_OFF   0x7fff       // 设置温度不显示
#define TSET_LCD_LO    0x7ffe       // 显示 lo
#define TSET_LCD_HI    0x7ffd       // 显示HI

//LCD 实现函数
void DisTsetShakeSet(int16 i16Temp);
void SetLcdDisplayMode(LCDModeType mode);
void SetLcdDisplayData(LCDDATA *pLCDData);
void Clear_LCD(void);
void Updata_LCD(void);
void Init_LCD(void);
#endif /* LCD_DISPLAY_H_ */
