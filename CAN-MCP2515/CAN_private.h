/****************************************************************************************/
/* Author   :   Mustafa Mahmoud Mustafa                                                 */
/* Date     :   8 May 2021                                                           	*/
/* Version  :   V01                                                                     */
/* Desc		:   MCP2515 Driver for STM32F10x                                            */
/****************************************************************************************/

#ifndef _CAN_PRIVATE_H_
#define _CAN_PRIVATE_H_

/*	Control Registers Addresses				*/
#define CAN_BFPCTRL				(u8)0x0C
#define CAN_TXRTSCTRL			(u8)0x0D
#define CAN_CANSTAT				(u8)0x0E
#define CAN_CANCTRL				(u8)0x0F
#define CAN_TEC					(u8)0x1C
#define CAN_REC					(u8)0x1D
#define CAN_CNF3				(u8)0x28
#define CAN_CNF2				(u8)0x29
#define CAN_CNF1				(u8)0x2A
#define CAN_CANINTE				(u8)0x2B
#define CAN_CANINTF				(u8)0x2C
#define CAN_EFLG				(u8)0x2D
#define CAN_TXB0CTRL			(u8)0x30
#define CAN_TXB1CTRL			(u8)0x40
#define CAN_TXB2CTRL			(u8)0x50
#define CAN_RXB0CTRL			(u8)0x60
#define CAN_RXB1CTRL			(u8)0x70

/*	RX Buffer ID & Data						*/
#define CAN_RXB0SIDH			(u8)0x61
#define CAN_RXB0SIDL			(u8)0x62
#define CAN_RXB0EID8			(u8)0x63
#define CAN_RXB0EID0			(u8)0x64
#define CAN_RXB0DLC				(u8)0x65
#define CAN_RXB0D0				(u8)0x66
#define CAN_RX_ADDR_STEP		(u8)0x10

/*	RX Filter & Mask						*/
#define CAN_RXF0SIDH			(u8)0x00
#define CAN_RXF0SIDL			(u8)0x01
#define CAN_RXF0EID8			(u8)0x02
#define CAN_RXF0EID0			(u8)0x03
#define CAN_RXM0SIDH			(u8)0x20
#define CAN_RXM0SIDL			(u8)0x21
#define CAN_RXM0EID8			(u8)0x22
#define CAN_RXM0EID0			(u8)0x23
#define CAN_RX_MASK_STEP		(u8)0x04

/*	TX Buffer ID & Data						*/
#define CAN_TXB0SIDH			(u8)0x31
#define CAN_TXB0SIDL			(u8)0x32
#define CAN_TXB0EID8			(u8)0x33
#define CAN_TXB0EID0			(u8)0x34
#define CAN_TXB0DLC				(u8)0x35
#define CAN_TXB0D0				(u8)0x36
#define CAN_TX_ADDR_STEP		(u8)0x10

/*	SPI cmds								*/
#define CAN_CMD_RESET			(u8)0b11000000
#define CAN_CMD_READ			(u8)0b00000011
#define CAN_CMD_READ_RX_BUF		(u8)0b10010000
#define CAN_CMD_WRITE			(u8)0b00000010
#define CAN_CMD_LOAD_TX_BUF		(u8)0b01000000
#define CAN_CMD_RTS				(u8)0b10000000
#define CAN_CMD_READ_STATUS		(u8)0b10100000
#define CAN_CMD_RX_STATUS		(u8)0b10110000
#define CAN_CMD_BIT_MODIFY 		(u8)0b00000101

typedef struct
{
	u8 DataLen : 4;
	u8 RTR : 1;
	u8 Data[8];
	u32 MsgID;
} CANBuff_t;

/*	Private data							*/
u8 CAN_RX0_Full;
u8 CAN_RX1_Full;
CANBuff_t CAN_RX0_Buff;
CANBuff_t CAN_RX1_Buff;
/*u8 CAN_RX0_Data[8];
u8 CAN_RX1_Data[8];
u8 CAN_RX0_Data_Len;
u8 CAN_RX1_Data_Len;
u32 CAN_RX0_Msg_ID;
u32 CAN_RX1_Msg_ID;*/

/*	Private functions prototype				*/
static void voidSetRXMask(u8 copy_u8MaskID, u8 copy_u8ExtIDEn, u32 copy_u32Mask);
static void voidSetRXFilter(u8 copy_u8FilterID, u8 copy_u8ExtIDEn, u32 copy_u32Filter);
static void voidBuildMsgId(u32 copy_u32MsgID, u8 copy_u8ExtIDEn, u8 *copy_pu8RetBuff);
static void voidWrite(u8 copy_u8Addr, u8 copy_u8ByteCount, u8 *copy_pu8Data);
static void voidRead(u8 copy_u8Addr, u8 copy_u8ByteCount, u8 *copy_pu8Data);
// static void voidRead(u8 copy_u8Addr, u8 copy_u8ByteCount, u8 *copy_pu8Data);
// static void voidWrite(u8 copy_u8Addr, u8 copy_u8ByteCount, u8 *copy_pu8Data);
// static void	voidReadRXBuffer(u8 copy_u8RXBufferID, u8 *copy_pu8DataArr);
// static u8	u8GetRXStatus(void);
// static void voidLoadTXBuffer(u8 copy_u8TXBufferID, u8 copy_u8Data);
// static void voidRTS(u8 copy_u8Mask);
// static void voidReadStatus(void);
// static void voidBitModify(u8 copy_u8Addr, u8 copy_u8Mask, u8 copy_u8Data);
// static void voidReset(void);

#endif // _CAN_PRIVATE_H_