/************************文件说明*****************************************************
* Copyright (c) 2019 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：GlobalDefine.h
* 功能描述：版本号定义
 *  
* 编译器： codewarrior IDE 11.1
* MCU说明：此模块为上层应用，和MCU 无关。
* 此文件会为模板，基本不用更改，按照需求更改宏定义
*/
/**************************更改记录******************************************************
 * 
* 版本：1.0
* 说明：初始版本建立
* 原作者：xb
* 完成日期：2020年1月14日
* 
* 版本：2.0
* 说明：增加内外部版本及说明
* 原作者：lyc
* 完成日期：2020年5月29日
* 
* 版本：3.0
* 说明： 将名字GolbalDefine改为GlobalDefine
* 原作者：xb
* 完成日期：2020年6月18日
* 
*  版本：3.1
* 说明： 产品型号宏定义放到此处
* 作者：xb
* 完成日期：2021年8月23日
* 
*/


#ifndef GOLBALDEFINE_H_
#define GOLBALDEFINE_H_

/*******************************************************************************
 * 版本号定义
 *
********************************************************************************/
#define     SwV_        00        //软件版本第一位,代表客户改了几次共2位
#define     SwsV_       02        //软件版本第二位，代表内部更改的次数，客户更改时清零
#define     HwV_        00        //硬件版本号不体现
#define     HwsV_       02
#define     SVN_LOG     0x02      //svn 追溯号，BCD码表示 

#define    DIAG_SW_     0x01      //诊断用的版本号
#define    DIAG_CW_     0x01
#define    DIAG_HW_     0x01

#define     SwV_CLIENT        00        //外部版本高位，一般不进行显示
#define     SwsV_CLIENT       01        //外部版本地位


#define  SW_YEAR_       (2019-2014)
#define  SW_MONTH_       (11)
#define  SW_DAY_         (17)

//产品型号定义 BT 程序烧入时候会检测ECUTYPE 的前 TypeCheckNum 位 需要兼容不同型号可更改 BT 程序的TypeCheckNum 值
#define   PRODUCT_TYPE_TYPE        'A'      //第一位为 'A'  ACP    'E'  ECP    'U' ECU  为了兼容以前的BT 程序，这一位写100
#define   PRODUCT_TYPE_NUM        0x101     //第二，三位为控制器编号  BCD 码 为ECP101即为 0x0101
#define   PRODUCT_TYPE_SUFFIX      '-'      //第4位为  -后面的数字，  不-,则为'-'； -A 则为'A',



#endif /* GLOBALDEFINE_H_ */
