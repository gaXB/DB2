/************************文件说明*****************************************************
* Copyright (c) 2019 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：NM_APP.h
* 功能描述：CAN 通讯是否使能功能模块的头文件，部分宏定义请按说明更改
* 编译器： codewarrior IDE 10.7
* MCU说明：此模块为上层应用，和MCU 无关
*/
/**************************更改记录******************************************************
 * 
* 版本：1.0
* 说明：初始版本建立
* 原作者：xb
* 完成日期：2019年1月9日
*/

#ifndef NM_APP_H_
#define NM_APP_H_

#define   APPSEND_DELAY_TIME   1   //应用报文上电延时时间，在接间网络管理时，应不需要延时，改为1即可


void Com_RxStart(void);
void Com_RxStop(void);
void Com_TxStart(void);
void Com_TxStop(void);
void Com_TxEnDealy(void);
#endif /* NM_APP_H_ */
