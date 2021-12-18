/************************文件说明*****************************************************
* Copyright (c) 2019 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：keypad.h
* 功能描述：按键 模块的头文件。
* 为此模块能正常工作需要加入 需要定时器模块
*
* 编译器： codewarrior IDE 10.7-KEA
* MCU说明：于MCU无关
*/
/**************************更改记录******************************************************
 *
* 版本：1.0
* 说明：初始版本建立
* 原作者：xb
* 完成日期：2019年5月9日
*
* 版本：1.1
* 说明：组合双击和4击键程序优化
* 原作者：xb
* 完成日期：2020年5月15日
*
* 版本：1.2
* 说明：增加AC、循环4击键内外部软件版本检测
* 原作者：lyc
* 完成日期：2020年5月19日
* 
* 版本：1.3
* 说明：
* 1 增加内循环按键为长按键功能，用来显示客户版本号
* 2 更改AC 内外的名字，为版本号高低位键
* 作者：xb
* 完成日期：2020年6月3日
*/
#ifndef KEY_PAD__
#define KEY_PAD__
#include"DataType.h"


//特殊键值
#define   KEY_CODE_NO                  0        //无键按下
#define   KEY_CODE_INVALID             0xFF     //键值无效
#define   KEY_CODE_TREMBLE             0xFFFFu   //键抖动
#define   KEY_CODE_CONTINUE1           0x8000u    //此键值有连续键功能
#define   KEY_CODE_CONTINUE2           0x4000u    //此键值有连续键功能
#define   KEY_CODE_LONG                0x2000u   //此键值有长按键功能,响应长按键键值为0x20FF
#define   KEY_CODE_COMBFOUR            0x1800u   //此键值有4击功能

#define   KEY_VER_L	      	         0x0001u   //版本号键的低位键值
#define   KEY_VER_H              	   0x0002u   //版本号键的高位键值
#define   KEY_CODE_VER_COMBIN          0x0003u   //组合值

#define   KEY_CODE_SPEC                0x00FFu   //把后8位都为特殊功能响应的键，如果驱动层返回一个这样的键值，则认为错误键
#define   KEY_CODE_SPEC_H					0xFF00u
#define   KEY_CODE_SPEC_H1					0xFE00u

#define   KEY_CODE_COMBFOUR_VERH       (KEY_CODE_SPEC_H+KEY_CODE_VER_COMBIN)    //按低位键，4击高位
#define   KEY_CODE_COMBFOUR_VERL       (KEY_CODE_SPEC_H1+KEY_CODE_VER_COMBIN)   //按高位键，4击低位
#define   KEY_VERH_LONG                (KEY_VER_H+KEY_CODE_SPEC_H)
//键盘状态
#define   CONKEY_STATE_BEGIN       0   //开始按键长安计时
#define   CONKEY_STATE_CONTINUE    1   //连续响应按键

//键盘故障代码
#define KEY_ERROR_NONE      0x00   //键盘正常
#define KEY_ERROR_INVALID   0x01   //无效键
#define KEY_ERROR_LONGDOWN  0x02   //某个键长按
#define KEY_ERROR_READ	    0X03    //物理层送来键有问题


//功能键时间定义
#define   CONTINUEKEY_SEG1_TIME     7   //进入连续功能为0.7s
#define   CONTINUEKEY_SEG2_TIME     3   //连续反应为 0.3s
#define   COMBFOUR_TIME             30  //4击在多少时间内。*0.1
#define   LONGKEY_TIME              50  //长按功能为 5s

//滤波时间
#define  KEY_FILTER_TIME         20      //滤波时间 ms
#define  KEY_LONG_FILTER_TIME    300     //长时间按此键认为故障，时间 0.1
#define  NOKEY_LONG_FILTER_TIME  10      //无键时长时间按键的恢复 时间 0.1

#define  RESKEY_BUFFERNUM        3       //按键相应的缓存数量

uint16 Read_Keydate(void);
uint8 Read_Key_Errcode(void);
void Treat_Read_Keypad(void);
void Respond_Keydata(uint16 keycode);
#endif
