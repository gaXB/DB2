/*
 * Config.h
 *
 *  Created on: Oct 22, 2020
 *      Author: nxa07657
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#include "Cpu.h"

/**************************************************************************
 * RGB LED configuration for S32K144EVB-Q100
 **************************************************************************/
#define RGB_LED_GPIO        PTD    /*GPIO used for the on-board RGB led control*/
#define RGB_BLUE_LED_PIN    0u     /*PTD0  connected to the blue RGB LED*/
#define RGB_RED_LED_PIN     15u    /*PTD15 connected to the blue RGB LED*/
#define RGB_GREEN_LED_PIN   16u    /*PTD16 connected to the blue RGB LED*/

/*
 * define the MB index for CAN message TX and RX
 * the MB index is the logic MB index instead of classical MB
 *
 * If RxFIFO is enabled: the MB0 will be used by RxFIFO, and it's Buff configuration and ID filter settings
 *  are configured in init() API, user should not call CAN_ConfigRxBuff() and CAN_SetRxFilter() with MB0
 *  the MB0 cannot be used as TX as well.
 *
 *  If RxFIFO is disabled(default setting): all MB are individual to be used as TX or RX
 *
 */
/* For this demo, RxFIFO is enabled, so MB0 is reserved and cannot be assigned here
 * Note: for CAN_PAL here with RxFIFO enabled, the MB is the virtual MB, not actual MB
 */
 
#define RX_MB  11        /*define the RX MB*/
#define TX_MB  9         /*define the TX MB*/

  /* define the RxFIFO used standard MB number for RX ID filter configuration */
  #define RxFIFO_USED_MB (7 +(canCom1_InitConfig0.num_id_filters<<1))

/************************************************************************
* Configure the following RX_ACK_ID and RX_ACK_ID_MASK for the RX MB
* ID filter:
* Below default configuration to
*      receive standard CAN messages with ID = 0x120 ~ 0x12F(16 ID in total) with RX MB
*************************************************************************/
#define RX_ACK_ID1             0x356 /*define RX MB acceptance message ID*/
#define RX_ACK_ID             0x123/*define RX MB acceptance message ID*/
/**************************************************************************
 *  define the IDE-bit mask,
 * IDE_MASK_SET: match to receive only standard or extend ID message frames
 * IDE_MASK_CLR: mask and ignore IDE-bit, so both standard and extend ID message
 *               frames will be received
 **************************************************************************/
#define IDE_MASK_SET  (1<<FLEXCAN_RX_FIFO_ID_FILTER_FORMATAB_IDE_SHIFT)
#define IDE_MASK_CLR  (0<<FLEXCAN_RX_FIFO_ID_FILTER_FORMATAB_IDE_SHIFT)

/**************************************************************************
 *  define the IDE-bit mask,
 * RTR_MASK_SET: match to receive only data or remote message frames
 * RTR_MASK_CLR: mask and ignore RTR-bit, so both data and remote message
 *               frames will be received
 **************************************************************************/
#define RTR_MASK_SET  (1<<FLEXCAN_RX_FIFO_ID_FILTER_FORMATAB_RTR_SHIFT)
#define RTR_MASK_CLR  (0<<FLEXCAN_RX_FIFO_ID_FILTER_FORMATAB_RTR_SHIFT)

#define RX_ACK_ID_MASK        (0x7F0 | IDE_MASK_SET | RTR_MASK_SET) /* mask the lower 4LSB ID */

#define TX_MSG_ID              0x100 /*define TX message ID*/

#define CAN_RX_Buffer_MAX         2   /*define the RX buffer size*/
#define CAN_RxFIFO_RX_Buffer_MAX  2   /*define the RxFIFO RX buffer size*/

/*
 * define the RxFIFO ID filter table mask, for this demo, configure RxFIFO with Type-B filter:
 *  two 16-bit filer with standard ID
 */
#define RxFIFO_ACK_ID_MASK  (0x7FF | IDE_MASK_SET | RTR_MASK_SET)   /* all ID bits must match with RxFIFO ID table settings  */

/* enable this macro define to disable bus off automatically recovery feature, and user should add bus-off recovery process codes
 * comment this macro will use FlexCAN default configuration of bus off automatic recovery.
 */
#define CAN_BUSOFF_RECOVERY_MANUAL  1

extern uint32_t CAN_RX_BufferIndex ;    /*buffer index for CAN RX*/
extern uint32_t CAN_RxDataBufferIndex; /*the latest CAN message data buffer index */
extern bool CAN_RX_Completed_Flag; /*flag used for sync between CAN TXRX ISR and application*/
extern flexcan_msgbuff_t CAN_RX_Message_Buffer[CAN_RX_Buffer_MAX];/*circle buffer for CAN RX*/
extern flexcan_msgbuff_t CAN_RX1_Message_Buffer[CAN_RX_Buffer_MAX];

extern uint32_t CAN_RxFIFO_RX_BufferIndex;    /*buffer index for CAN RxFIFO RX*/
extern uint32_t CAN_RxFIFO_RxDataBufferIndex; /*the latest RxFIFO CAN message data buffer index */
extern bool CAN_RxFIFO_RX_Completed_Flag; /*flag used for sync between CAN TXRX ISR and application*/
extern flexcan_msgbuff_t CAN_RxFIFO_RX_Message_Buffer[CAN_RxFIFO_RX_Buffer_MAX];/*circle buffer for CAN RxFIFO RX*/

extern bool BusOff_Flag; /* the bus off flag */

/* CAN hardware MB(Message Buffer) configuration for TX MB */
extern flexcan_data_info_t CAN_TX_MB_Config;

/* CAN hardware MB(Message Buffer) configuration for RX MB */
//extern flexcan_data_info_t CAN_RX_MB_Config;

/* FlexCAN RxFIFO ID filter table */
extern const flexcan_id_table_t RxFIFO_FilterAcceptanceID_Table[16];

/* CAN message data buffer to transmit */
extern uint8_t can_msg_data[8];

extern void CAN_User_Init(void);
extern void FLEXCAN_BOFFREC_Config(uint32_t instance, bool enable);
extern void CAN_TX_RX_Callback(uint8_t instance, flexcan_event_type_t eventType, uint32_t buffIdx, flexcan_state_t *flexcanState);
extern void CAN_Error_Callback(uint8_t instance, flexcan_event_type_t eventType, flexcan_state_t *flexcanState);
extern void CAN_MB_Config_Update(flexcan_data_info_t *config, uint32_t cs);

#endif /* CONFIG_H_ */
