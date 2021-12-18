/************************文件说明*****************************************************
* Copyright (c) 2019 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：DIDList.h
* 功能描述：DealDID 22服务中固定的DID列表， 只能被DealDID.C包含
* 具体不同点需要按要求更改。
* 编译器： codewarrior IDE 10.7
* MCU说明：此模块为上层应用，和MCU 无关
*/
/**************************更改记录******************************************************
 * 

2019.1.17
ver0.1
1 初始版本
2 模板程序

2019.7.20
ver0.2
1 需要存储的DID 可用SYSTEMSAVE 自动存储，改变格式 
2019.10.25
ver0.3
1 跟随DealDID.c 的版本

2020.5.21
ver0.4
作者：xb
1 版本号读取DID时  位BCD码
2 并读取SVN号
3 增加FE00DID

2020.9.27
ver0.5
作者：xb
增加  0xFDE4  ECU型号的DID
*/
#include "APPDrive.h"
#include "UDS_APP.h"

//pid tbl_char
//ECU BOOT SOFTWARE 
#define  PID_BOOTLOAD_SOFTWARE      0xF183
const uint8 tbl_BootloadSW[] = "C000000000" ;
#define  PID_BOOTLSW_LENGTH  10


//ecu num
#define  PID_ECU_NUM          0XF187     //ecu 号需要可更改
uint8 tbl_ECUNumber[]=
{
	0x43, 0x00, 0x09, 0x37, 0x11, 0x00};
#define  PID_ECUNUMBER_LENGTH     6

//supply id
#define  PID_SUPPER_ID        0xF18A
const uint8 tbl_SupperNumber[]=
{
   0x50, 0x03, 0x12, 0x60, 0x00, 0x00};
#define  PID_SUPPLIER_LENGTH     6

//ECU manufacture data
#define  PID_MADEDATA_NUMBER      0xF18B
const uint8 tbl_ECUMadeDate[] =
{
   0x11, 0x11, 0x11};
#define  PID_MADEDATE_LENGTH  3

//Supplier ECU Serial Number
#define  PID_ECUSERIAL_NUMBER      0xF18C
const uint8 tbl_ECUSerialNumber[] =
{
   0x11, 0x11, 0x11, 0x11, 0x11,
   0x11, 0x11, 0x11, 0x11, 0x11,};
#define  PID_ECUSERIAL_NUMBER_LENGTH  10

//车辆识别码
#define  PID_VIN              0xF190           //需要可更改
uint8 TBL_VIN[] = "LSJW26H327J000008" ;
#define  PID_VIN_LENGTH         17
//hardware
#define  PID_HARDWARE_NUMBER  0xF191


const uint8 tbl_HardwareNumber[] =
{
   0x48, 0x83, 0xB1, 0x11,0x01,0x01 };

#define  PID_HARDWARENUMBER_LENGTH     6




//ECU SUPPLIER hardware  number
#define  PID_GA_HARDWARE_NUMBER      0xF192
const uint8 tbl_GA_HardwareNumber[] =
"GAECU00104";
#define  PID_GA_HARDWARENUMBER_LENGTH     10


//ECU supplie software number
#define  PID_GA_SOFTWARE_NUMBER      0xF194
const uint8 tbl_GA_SoftwareNumber[] =
"GAECU00104";

#define  PID_GA_SOFTWARENUMBER_LENGTH     10


//app softeware number
#define  PID_APPSW_NUMBER      0xF1A0


const uint8 tbl_ASoftwareNumber[] =
{
   0x48, 0x83, 0xB1, 0x12,0x02,0x01 };

#define  PID_APPSWNUMBER_LENGTH  6

//calibration softeware number
#define  PID_CALSW_NUMBER      0xF1A1
const uint8 tbl_CSoftwareNumber[] =
{
		0x48, 0x83, 0xB1, 0x13,0x01,0x01 };
#define  PID_CALSWNUMBER_LENGTH  6

//ECU NET reference number
#define  PID_ECU_NETREFER_NUMBER      0xF1A2
const uint8 tbl_ECUNetRefNumber[] =
{0x4e, 0x12, 0x34, 0x56, 0x78, 0x00};

#define  PID_ECU_NETREFERNUMBER_LENGTH     6

// Vehicle Feature Information 需要更改，level1
#define  PID_VEHFI_NUMBER      0xF1A8
uint8 TBL_VehicleFeatureInf[] =
{
0xE2, 0x92, 0x00, 0x00, 0x70, 0x00, 
0x72, 0x00, 0x82, 0x59, 0x01, 0x18, 
0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
#define  PID_VEHFI_LENGTH  20






//DID 表， 对应长度
PID_DATA  PID[MAX_PID_NUM]=
{
   {PID_ECU_NUM, tbl_ECUNumber, PID_ECUNUMBER_LENGTH} ,    
   {PID_VIN, TBL_VIN, PID_VIN_LENGTH} ,
   {PID_VEHFI_NUMBER, TBL_VehicleFeatureInf, PID_VEHFI_LENGTH} ,
   {PID_SUPPER_ID, tbl_SupperNumber, PID_SUPPLIER_LENGTH} ,
   {PID_HARDWARE_NUMBER, tbl_HardwareNumber, PID_HARDWARENUMBER_LENGTH} ,
   {PID_APPSW_NUMBER, tbl_ASoftwareNumber, PID_APPSWNUMBER_LENGTH} ,
   {PID_CALSW_NUMBER, tbl_CSoftwareNumber, PID_CALSWNUMBER_LENGTH} ,
   {PID_MADEDATA_NUMBER, tbl_ECUMadeDate, PID_MADEDATE_LENGTH} ,
   {PID_BOOTLOAD_SOFTWARE, tbl_BootloadSW, PID_BOOTLSW_LENGTH} ,
   {PID_GA_HARDWARE_NUMBER, tbl_GA_HardwareNumber, PID_GA_HARDWARENUMBER_LENGTH},
   {PID_GA_SOFTWARE_NUMBER, tbl_GA_SoftwareNumber, PID_GA_SOFTWARENUMBER_LENGTH},
   {PID_ECU_NETREFER_NUMBER, tbl_ECUNetRefNumber, PID_ECU_NETREFERNUMBER_LENGTH},
   {PID_ECUSERIAL_NUMBER, tbl_ECUSerialNumber, PID_ECUSERIAL_NUMBER_LENGTH},
}; 
//其他DID
#define  ACTION_DIAG_SESSION   0xF186
#define  PID_F110      0xF110

#define  DID_SYSTEMSTATE       0xFDDD    //标定 系统状态值
#define  DID_SYSTEMPARMETER    0xFDDE    //标定自动参数
#define  DID_SYSTEMPARMETER1   0xFDDF    //标定自动参数1
#define  DID_SYSTMESET         0xFE00    //强制设置DID
#define  DID_CHECKSUM    0xFDFF        //checksum
#define  CODE_CHECK_ADDR   CODE_END_ADDR  //checksum地址
//IO DID
#define IODID_SYSTEMCONTROL     0xD000
#define IODID_BUSCONFIG         0xC000

//#define  DID_SVN_LOG            0xFDFE
#define  DID_PRODUCTTYPE            0xFDE4
#define  DID_SWCLIENT_VER       0xFDE3
#define  DID_SW_SVN_VER         0xFDE2
#define  DID_BT_VER             0xFDE1
#define  DID_HW_VER             0xFDE0
//标定DID 接口函数
extern void GetParameterData(uint8* rtx);
extern BOOL  SetParameterData(uint8* rtx);
extern void GetParameterData_Sec(uint8* rtx);
extern BOOL  SetParameterData_Sec(uint8* rtx);
extern void GetSystemData(uint8* pSystemData);
extern void GetSetData(uint8* rtx);
extern BOOL  SetSetData(uint8* rtx);
//io control
extern uint8 DID_IOGetSystemControl(void);
extern uint8 DID_GetDidData(uint16 Num);
extern uint8 DID_IOSetSystemControl(uint8* u8IOData, uint8 ControlType);
extern uint8 DID_SetDidData(uint16 Num, uint8 state, uint8 ControlType);
extern void DID_UpdataList(void);
extern void DID_WriteF110(uint8* WDidData);
extern BOOL DID_ReadF110(uint8 *RDidData, uint8 F110Number);
extern void DID_SetCANBUSConfig(uint8 u8Num, uint8 state);
extern uint8 DID_GetCANBUSConfig(uint8 u8Num);
