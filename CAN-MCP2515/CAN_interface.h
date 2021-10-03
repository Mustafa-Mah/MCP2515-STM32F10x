/****************************************************************************************/
/* Author   :   Mustafa Mahmoud Mustafa                                                 */
/* Date     :   8 May 2021                                                           	*/
/* Version  :   V01                                                                     */
/* Desc		:   MCP2515 Driver for STM32F10x                                            */
/****************************************************************************************/

#ifndef _CAN_INTERFACE_H_
#define _CAN_INTERFACE_H_

/*	TX Buffer ID							*/
#define CAN_TX_0				0
#define CAN_TX_1				1
#define CAN_TX_2				2

/*	RX Buffer ID							*/
#define CAN_RX_0				0
#define CAN_RX_1				1

/*	TX Filter ID							*/
#define CAN_FILTER_0			0
#define CAN_FILTER_1			1
#define CAN_FILTER_2			2
#define CAN_FILTER_3			3
#define CAN_FILTER_4			4
#define CAN_FILTER_5			5

/*	TX Mask ID								*/
#define CAN_MASK_0				0
#define CAN_MASK_1				1

void HCAN_voidInit(void);
void HCAN_voidSetTXMsgID(u8 copy_u8TXBufferID, u8 copy_u8ExtIDEn, u32 copy_u32MsgID);
void HCAN_voidGetRXMsgID(u8 copy_u8RXBufferID, u8 *copy_pu8ExtIDEn, u32 *copy_pu32MsgID);
void HCAN_voidSetTXData(u8 copy_u8TXBufferID, u8 copy_u8DataLen, const u8 *copy_pu8Data);
void HCAN_voidGetRXData(u8 copy_u8RXBufferID, u8 *copy_pu8DataLen, u8 *copy_pu8Data);
u8	 HCAN_u8GetRXStatus(u8 copy_u8RXBufferID);
void HCAN_voidSendFrame(u8 copy_u8TXBufferID);
void HCAN_voidEXTICallBack(void);
//void HCAN_voidSetRXMask(u8 copy_u8MaskID, u8 copy_u8ExtIDEn, u32 copy_u32Mask);
//void HCAN_voidSetRXFilter(u8 copy_u8FilterID, u8 copy_u8ExtIDEn, u32 copy_u32Filter);

#endif // _CAN_INTERFACE_H_