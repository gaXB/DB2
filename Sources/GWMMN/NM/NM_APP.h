/************************�ļ�˵��*****************************************************
* Copyright (c) 2019 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�NM_APP.h
* ����������CAN ͨѶ�Ƿ�ʹ�ܹ���ģ���ͷ�ļ������ֺ궨���밴˵������
* �������� codewarrior IDE 10.7
* MCU˵������ģ��Ϊ�ϲ�Ӧ�ã���MCU �޹�
*/
/**************************���ļ�¼******************************************************
 * 
* �汾��1.0
* ˵������ʼ�汾����
* ԭ���ߣ�xb
* ������ڣ�2019��1��9��
*/

#ifndef NM_APP_H_
#define NM_APP_H_

#define   APPSEND_DELAY_TIME   1   //Ӧ�ñ����ϵ���ʱʱ�䣬�ڽӼ��������ʱ��Ӧ����Ҫ��ʱ����Ϊ1����


void Com_RxStart(void);
void Com_RxStop(void);
void Com_TxStart(void);
void Com_TxStop(void);
void Com_TxEnDealy(void);
#endif /* NM_APP_H_ */
