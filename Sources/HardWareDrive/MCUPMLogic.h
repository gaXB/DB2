/**************************�ļ�˵��*****************************************************
* Copyright (c) 2019 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�MCUPMLogic.h
* ����������MCU �����߼�ͷ�ļ�
* �������� codewarrior IDE 11.1
* MCU˵������MCU�޹�
* 
* */
/**************************���ļ�¼******************************************************
 * 
* �汾��1.0
* ˵������ʼ�汾����
* ԭ���ߣ�xb
* ������ڣ�2020��5��13��
* 
* 
*/

#ifndef MCUPMLOGIC_H_
#define MCUPMLOGIC_H_

//�ⲿ�ɲ鿴�ļĴ��������京��
extern uint32 u32MCUData[7];
#define   MCUDATA_NUM_REST             0
#define   MCUDATA_NUM_IGNCOUNT         1
#define   MCUDATA_NUM_REST_1           2
#define   MCUDATA_NUM_RESETCOUNT       3
#define   MCUDATA_NUM_IGNWAKE          4
#define   MCUDATA_NUM_FLG              5 

//LDO ��ģʽ�� ������궨��_PROJECT_LDO_MODE ������
#define   LDO_MODE_BATTIGN        0
#define   LDO_MODE_2575           1
#define   LDO_MODE_IGN            2

void MCUPM_SetReset(uint32 u32ResonRegister);
void MCUPM_IgnOn(void);
void MCUPM_Over(void);
void MCUPM_ReadEE(void);
BOOL MCUPM_IsLPowerRest(void);
BOOL MCUPM_IsLastIgnOFFNormal(void);
BOOL MCUPM_IgnCountReset(uint32 u32times);
uint32 MCUPM_PowerOnResetTimes(void);

#endif /* MCUPMLOGIC_H_ */
