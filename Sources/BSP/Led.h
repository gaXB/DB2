/************************文件说明*****************************************************
* Copyright (c) 2020 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：Led.h
* 
* 功能描述： LED.h文件，定义LED高低版本的灯
*  
*/
/**************************更改记录******************************************************
 * 
* 版本：1.0
* 说明：初始版本建立
* 原作者：xb
* 完成日期：2020年1月13日
* 
* * * 版本：2.0
* 说明：LED 显示版本号功能升级
* 原作者：lyc
* 完成日期：2020年5月29日
*/
#ifndef LED_H_
#define LED_H_

#include "APPDrive.h"
#include "SystemType.h"


//版本高低位定义
#define  SWVERH_LED   sDisplayData.CIRF  
#define  SWVERL_LED   sDisplayData.AC    


//外部可调用函数
void	LED_AllOpen(void);
void	LED_AllClose(void);
void  LedDisVerEn(uint16 u16VerData);
void LEDSetTask(void);
BOOL  LED_GetDisVer(void);
#endif /* LED_H_ */
