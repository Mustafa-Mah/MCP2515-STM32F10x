/****************************************************************************************/
/* Author   :   Mustafa Mahmoud Mustafa                                                 */
/* Date     :   8 May 2021                                                           	*/
/* Version  :   V01                                                                     */
/* Desc		:   MCP2515 Driver for STM32F10x                                            */
/****************************************************************************************/

#ifndef _CAN_CONFIG_H_
#define _CAN_CONFIG_H_

#define CAN_SPI_INTF						SPI_1
#define CAN_CS_PIN							GPIOA,PIN0

#define CAN_BRP								0
#define CAN_SJW								0b10
#define CAN_PRSEG							0b001
#define CAN_PHSEG1							0b110
#define CAN_SAM								0
#define CAN_BTLMODE							1
#define CAN_PHSEG2							0b010
#define CAN_WAKFIL							0
#define CAN_SOF								1
#define CAN_CLKPRE							0b00
#define CAN_CLKEN							0
#define CAN_OSM								0

#define CAN_RX_USE_EXTENDED_ID				0
#define CAN_RX_FILTER0						0x2U
#define CAN_RX_FILTER1						0x3U
#define CAN_RX_FILTER2						0x4U
#define CAN_RX_FILTER3						0x5U
#define CAN_RX_FILTER4						0x6U
#define CAN_RX_FILTER5						0x7U
#define CAN_RX_MASK0						0x000007FFU
#define CAN_RX_MASK1						0x000007FFU
#define CAN_RX0_INT_EN						1
#define CAN_RX1_INT_EN						1

#define CAN_TX0_PRIO						0b00
#define CAN_TX1_PRIO						0b01
#define CAN_TX2_PRIO						0b10

#endif // _CAN_CONFIG_H_