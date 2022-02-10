/*
 * SpecialRealy.h
 *
 *  Created on: 2022��2��9��
 *      Author: Administrator
 */

#ifndef MODULE_SPECIALREALY_H_
#define MODULE_SPECIALREALY_H_

#define  SREALY_ERR_NULL            0
#define  SREALY_ERR_OPENLOAD        1
#define  SREALY_ERR_OVERCUNRRENT    2


#define   ERRSET_TIMES          100   //�ȶ�ʱ�����ָ������ ���ù�����


#define    SREALY_NUM_SOV1      0
#define    SREALY_NUM_SOV2      1
#define    SREALY_NUM_TMU       2
#define    SREALY_NUM_RHEAT      3
#define    SREAL_MAXNUM         4

uint8 SRealyGetErr(uint8 id);
void SRealySet(uint8 id, uint8 bSetValue);
void SRealyControl(void);
#endif /* MODULE_SPECIALREALY_H_ */
