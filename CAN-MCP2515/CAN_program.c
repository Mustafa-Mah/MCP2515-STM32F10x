/****************************************************************************************/
/* Author   :   Mustafa Mahmoud Mustafa                                                 */
/* Date     :   8 May 2021                                                           	*/
/* Version  :   V01                                                                     */
/* Desc		:   MCP2515 Driver for STM32F10x                                            */
/****************************************************************************************/

#include "STD_TYPES.h"
#include "BIT_MATH.h"

#include "GPIO_interface.h"
#include "SPI_interface.h"

#include "CAN_interface.h"
#include "CAN_private.h"
#include "CAN_config.h"

void HCAN_voidInit(void)
{
	u8 local_u8Temp[6];

	MGPIO_voidSetPinDirection(CAN_CS_PIN, OUTPUT_10MHZ_PP);
	MGPIO_voidSetPinValue(CAN_CS_PIN, GPIO_HIGH);

	MSPI_voidInit();
	MSPI_voidEnableSPIx(CAN_SPI_INTF);

	CAN_RX0_Full = 0;
	CAN_RX1_Full = 0;

	/*	Reset CAN controller and start config	*/
	MSPI_u32SendReceiveDataSync(CAN_SPI_INTF, CAN_CMD_RESET);

	/*	CNF Registers							*/
	local_u8Temp[0] = (CAN_PHSEG2) | (CAN_WAKFIL << 6) | (CAN_SOF << 7);
	local_u8Temp[1] = (CAN_PRSEG) | (CAN_PHSEG1 << 3) | (CAN_SAM << 6) | (CAN_BTLMODE << 7);
	local_u8Temp[2] = (CAN_BRP) | (CAN_SJW << 6);
	voidWrite(CAN_CNF3, 3, local_u8Temp);

	/**********************/

	local_u8Temp[0] = CAN_RX0_INT_EN | (CAN_RX1_INT_EN << 1);
	voidWrite(CAN_CANINTE, 1, local_u8Temp);
	
	local_u8Temp[0] = (CAN_CLKPRE) | (CAN_CLKEN << 2) | (CAN_OSM << 3);
	voidWrite(CAN_CANCTRL, 1, local_u8Temp);
}

void HCAN_voidSetTXMsgID(u8 copy_u8TXBufferID, u8 copy_u8ExtIDEn, u32 copy_u32MsgID)
{
	u8 local_u8Temp[4];
	u8 local_u8Addr;

	voidBuildMsgId(copy_u32MsgID, copy_u8ExtIDEn, local_u8Temp);
	if (copy_u8TXBufferID <= CAN_TX_2)
	{
		local_u8Addr = CAN_TXB0SIDH + (CAN_TX_ADDR_STEP * copy_u8TXBufferID);
		/*	Send Msg ID	*/
		//MGPIO_voidSetPinValue(CAN_CS_PIN, GPIO_LOW);
		voidWrite(local_u8Addr, 4, local_u8Temp);
		//MGPIO_voidSetPinValue(CAN_CS_PIN, GPIO_HIGH);
	}
}
void HCAN_voidGetRXMsgID(u8 copy_u8RXBufferID, u8 *copy_pu8ExtIDEn, u32 *copy_pu32MsgID)
{
	u8 local_u8Addr;
	u8 local_u8MsgIDTemp[4];

	if (copy_u8RXBufferID <= CAN_RX_1)
	{
		local_u8Addr = CAN_RXB0SIDH + (CAN_TX_ADDR_STEP * copy_u8RXBufferID);
		/*	Send data length and data	*/
		voidRead(local_u8Addr, 4, local_u8MsgIDTemp);
	}
	/*	EXTID En	*/
	*copy_pu8ExtIDEn = GET_BIT(local_u8MsgIDTemp[1], 3);
	/*	SID3-10		*/
	*copy_pu32MsgID = ((u32)local_u8MsgIDTemp[0]) << 3;
	/*	SID0-2		*/
	*copy_pu32MsgID |= ((u32)((local_u8MsgIDTemp[1] >> 5) & 0x07));
	/*	EID16-17	*/
	*copy_pu32MsgID |= (((u32)local_u8MsgIDTemp[1]) & 0x03) << 27;
	/*	EID8-15		*/
	*copy_pu32MsgID |= (((u32)local_u8MsgIDTemp[2]) << 19);
	/*	EID0-7		*/
	*copy_pu32MsgID |= (((u32)local_u8MsgIDTemp[3]) << 11);
}

//void HCAN_voidSetTXDataLength(u8 copy_u8TXBufferID, u8 copy_u8DataLen);
//void HCAN_voidGetRXDataLength(u8 copy_u8RXBufferID, u8 *copy_pu8DataLen);

void HCAN_voidSetTXData(u8 copy_u8TXBufferID, u8 copy_u8DataLen, const u8 *copy_pu8Data)
{
	u8 local_u8TempData[9];
	u8 local_u8Addr;

	if (copy_u8DataLen > 8)
	{
		copy_u8DataLen = 8;
	}
	local_u8TempData[0] = copy_u8DataLen;
	for (u8 i = 0; i < copy_u8DataLen; i++)
	{
		local_u8TempData[i + 1] = copy_pu8Data[i];
	}
	
	if (copy_u8TXBufferID <= CAN_TX_2)
	{
		local_u8Addr = CAN_TXB0DLC + (CAN_TX_ADDR_STEP * copy_u8TXBufferID);
		/*	Send data length and data	*/
		copy_u8DataLen++;
		//MGPIO_voidSetPinValue(CAN_CS_PIN, GPIO_LOW);
		voidWrite(local_u8Addr, copy_u8DataLen, local_u8TempData);
		//MGPIO_voidSetPinValue(CAN_CS_PIN, GPIO_HIGH);
	}
}

void HCAN_voidGetRXData(u8 copy_u8RXBufferID, u8 *copy_pu8DataLen, u8 *copy_pu8Data)
{
	u8 local_u8DLC;
	CANBuff_t *local_CANBuffptr;

	MGPIO_voidSetPinValue(CAN_CS_PIN, GPIO_LOW);
	MSPI_u32SendReceiveDataSync(CAN_SPI_INTF, CAN_CMD_READ);
	if ((copy_u8RXBufferID == CAN_RX_0) && (CAN_RX0_Full == 1))
	{
		MSPI_u32SendReceiveDataSync(CAN_SPI_INTF, CAN_RXB0DLC);
		local_CANBuffptr = &CAN_RX0_Buff;
		CAN_RX0_Full = 0;
	}
	else if ((copy_u8RXBufferID == CAN_RX_1) && (CAN_RX1_Full == 1))
	{
		MSPI_u32SendReceiveDataSync(CAN_SPI_INTF, (CAN_RXB0DLC + CAN_RX_ADDR_STEP));
		local_CANBuffptr = &CAN_RX1_Buff;
		CAN_RX1_Full = 0;
	}
	local_u8DLC = MSPI_u32SendReceiveDataSync(CAN_SPI_INTF, 0);
	local_CANBuffptr->RTR = GET_BIT(local_u8DLC, 6);
	local_u8DLC &= 0x0F;
	local_CANBuffptr->DataLen = local_u8DLC;
	copy_pu8DataLen = local_u8DLC;
	for (u8 i = 0; i < local_u8DLC; i++)
	{
		local_CANBuffptr->Data[i] = MSPI_u32SendReceiveDataSync(CAN_SPI_INTF, 0);
		copy_pu8Data[i] = local_CANBuffptr->Data[i];
	}
	MGPIO_voidSetPinValue(CAN_CS_PIN, GPIO_HIGH);
	/*	Clearing interrupt flag		*/
	MGPIO_voidSetPinValue(CAN_CS_PIN, GPIO_LOW);
	MSPI_u32SendReceiveDataSync(CAN_SPI_INTF, CAN_CMD_BIT_MODIFY);
	MSPI_u32SendReceiveDataSync(CAN_SPI_INTF, CAN_CANSTAT);
	MSPI_u32SendReceiveDataSync(CAN_SPI_INTF, (u8)(1U << copy_u8RXBufferID));
	MSPI_u32SendReceiveDataSync(CAN_SPI_INTF, 0);
	MGPIO_voidSetPinValue(CAN_CS_PIN, GPIO_HIGH);
}

u8	 HCAN_u8GetRXStatus(u8 copy_u8RXBufferID)
{
	u8 local_u8Result;
	if (copy_u8RXBufferID == CAN_RX_0)
	{
		local_u8Result = CAN_RX0_Full;
	}
	else if (copy_u8RXBufferID == CAN_RX_1)
	{
		local_u8Result = CAN_RX1_Full;
	}
	return local_u8Result;
}

void HCAN_voidSendFrame(u8 copy_u8TXBufferID)
{
	u8 local_u8TempRTS = CAN_CMD_RTS;
	if (copy_u8TXBufferID <= CAN_TX_2)
	{
		SET_BIT(local_u8TempRTS, copy_u8TXBufferID);
		/*	Send Request to Send cmd	*/
		//MGPIO_voidSetPinValue(CAN_CS_PIN, GPIO_LOW);
		MSPI_u32SendReceiveDataSync(CAN_SPI_INTF, local_u8TempRTS);
		//MGPIO_voidSetPinValue(CAN_CS_PIN, GPIO_HIGH);
	}
}

void HCAN_voidEXTICallBack(void)
{
	static u8 local_u8Status;
	static u8 local_u8RXStatus;

	MSPI_u32SendReceiveDataSync(CAN_SPI_INTF, CAN_CMD_READ_STATUS);
	local_u8Status = MSPI_u32SendReceiveDataSync(CAN_SPI_INTF, 0);
	MSPI_u32SendReceiveDataSync(CAN_SPI_INTF, CAN_CMD_RX_STATUS);
	local_u8RXStatus = MSPI_u32SendReceiveDataSync(CAN_SPI_INTF, 0);

	if ((GET_BIT(local_u8RXStatus, 6) == 1) && (GET_BIT(local_u8Status, 0) == 1))
	{
		CAN_RX0_Full = 1;
	}
	if ((GET_BIT(local_u8RXStatus, 7) == 1) && (GET_BIT(local_u8Status, 1) == 1))
	{
		CAN_RX1_Full = 1;
	}
}

static void voidSetRXMask(u8 copy_u8MaskID, u8 copy_u8ExtIDEn, u32 copy_u32Mask)
{
	u8 local_u8Temp[4];
	u8 local_u8Addr;

	voidBuildMsgId(copy_u32Mask, copy_u8ExtIDEn, local_u8Temp);

	if (copy_u8MaskID == CAN_MASK_0)
	{
		local_u8Addr = CAN_RXM0SIDH;
	}
	else if (copy_u8MaskID == CAN_MASK_1)
	{
		local_u8Addr = CAN_RXM0SIDH + CAN_RX_MASK_STEP;
	}
	/*	Write Msg ID to CAN registers	*/
	//MGPIO_voidSetPinValue(CAN_CS_PIN, GPIO_LOW);
	voidWrite(local_u8Addr, 4, local_u8Temp);
	//MGPIO_voidSetPinValue(CAN_CS_PIN, GPIO_HIGH);
}

static void voidSetRXFilter(u8 copy_u8FilterID, u8 copy_u8ExtIDEn, u32 copy_u32Filter)
{
	u8 local_u8Temp[4];
	u8 local_u8Addr;

	voidBuildMsgId(copy_u32Filter, copy_u8ExtIDEn, local_u8Temp);

	// if (copy_u8ExtendedIDUsed == 0)
	// {
	// 	/*	SID3-10		*/
	// 	local_u8Temp[0] = (u8)(copy_u32Filter >> 3);
	// 	/*	SID0-2		*/
	// 	local_u8Temp[1] = (u8)((copy_u32Filter << 5) & 0xE0);
	// }
	// else if (copy_u8ExtendedIDUsed == 1)
	// {
	// 	/*	SID3-10		*/
	// 	local_u8Temp[0] = (u8)(copy_u32Filter >> 21);
	// 	/*	SID0-2		*/
	// 	local_u8Temp[1] = (u8)((copy_u32Filter >> 18) & 0xE0);
	// 	/*	Enable filter for Extended frame	*/
	// 	SET_BIT(local_u8Temp[1], 3);

	// 	if (GET_BIT(copy_u32Filter, 16) == 1)
	// 	{
	// 		SET_BIT(local_u8Temp[1], 0); 		/*	EID16		*/
	// 	}
	// 	if (GET_BIT(copy_u32Filter, 17) == 1)
	// 	{
	// 		SET_BIT(local_u8Temp[1], 1); 		/*	EID17		*/
	// 	}
	// 	local_u8Temp[2] = (u8)(copy_u32Filter >> 8);
	// 	local_u8Temp[3] = (u8)(copy_u32Filter);
	// }

	if ((copy_u8FilterID >= CAN_FILTER_0) && (copy_u8FilterID < CAN_FILTER_3))
	{
		local_u8Addr = CAN_RXM0SIDH + (copy_u8FilterID * CAN_RX_MASK_STEP);
	}
	else if ((copy_u8FilterID > CAN_FILTER_2) && (copy_u8FilterID <= CAN_FILTER_5))
	{
		local_u8Addr = CAN_RXM0SIDH + ((copy_u8FilterID + 1) * CAN_RX_MASK_STEP);
	}
	/*	Write Msg ID to CAN registers	*/
	//MGPIO_voidSetPinValue(CAN_CS_PIN, GPIO_LOW);
	voidWrite(local_u8Addr, 4, local_u8Temp);
	//MGPIO_voidSetPinValue(CAN_CS_PIN, GPIO_HIGH);
}

static void voidBuildMsgId(u32 copy_u32MsgID, u8 copy_u8ExtIDEn, u8 *copy_pu8RetBuff)
{
	/*	SID3-10		*/
	copy_pu8RetBuff[0] = (u8)(copy_u32MsgID >> 3);
	/*	SID0-2		*/
	copy_pu8RetBuff[1] = (u8)((copy_u32MsgID & 0x07) << 5);

	if (copy_u8ExtIDEn == 1)
	{
		/*	EID16-17	*/
		copy_pu8RetBuff[1] |= (u8)(((copy_u32MsgID >> 27) & 0x03) | (1 << 3));
		/*	EID0-7		*/
		copy_pu8RetBuff[2] = (u8)(copy_u32MsgID >> 19);
		/*	EID8-15		*/
		copy_pu8RetBuff[3] = (u8)(copy_u32MsgID >> 11);
	}
}

static void voidWrite(u8 copy_u8Addr, u8 copy_u8ByteCount, u8 *copy_pu8Data)
{
	MGPIO_voidSetPinValue(CAN_CS_PIN, GPIO_LOW);
	MSPI_u32SendReceiveDataSync(CAN_SPI_INTF, CAN_CMD_WRITE);
	MSPI_u32SendReceiveDataSync(CAN_SPI_INTF, copy_u8Addr);
	for (u8 i = 0; i < copy_u8ByteCount; i++)
	{
		MSPI_u32SendReceiveDataSync(CAN_SPI_INTF, copy_pu8Data[i]);
	}
	MGPIO_voidSetPinValue(CAN_CS_PIN, GPIO_HIGH);
}

static void voidRead(u8 copy_u8Addr, u8 copy_u8ByteCount, u8 *copy_pu8Data)
{
	MGPIO_voidSetPinValue(CAN_CS_PIN, GPIO_LOW);
	MSPI_u32SendReceiveDataSync(CAN_SPI_INTF, CAN_CMD_READ);
	MSPI_u32SendReceiveDataSync(CAN_SPI_INTF, copy_u8Addr);
	for (u8 i = 0; i < copy_u8ByteCount; i++)
	{
		copy_pu8Data[i] = MSPI_u32SendReceiveDataSync(CAN_SPI_INTF, 0);
	}
	MGPIO_voidSetPinValue(CAN_CS_PIN, GPIO_HIGH);
}