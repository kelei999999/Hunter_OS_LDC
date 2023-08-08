/* Hunter.ORG all rights reserved.  You may use this software
 * and any derivatives exclusively with Hunter.ORG products.
 *
 * THIS SOFTWARE IS SUPPLIED BY Hunter.ORG "AS IS".  NO WARRANTIES, WHETHER
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH Hunter.ORG PRODUCTS, COMBINATION
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.
 *
 * IN NO EVENT WILL Hunter.ORG BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF Hunter.ORG HAS
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE
 * FULLEST EXTENT ALLOWED BY LAW, Hunter.ORG'S TOTAL LIABILITY ON ALL CLAIMS
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF
 * ANY, THAT YOU HAVE PAID DIRECTLY TO Hunter.ORG FOR THIS SOFTWARE.
 *
 * Hunter.ORG PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
 * TERMS.
 */

/*
 * File: Hunter_OS_LDC.c
 * Author: Hunter.ORG
 * Created on: 2016年5月12日
 * Revision history: 2.0
 */

/*
 * This is a guard condition so that contents of this file are not included
 * more than once.
 */

#include "Hunter_OS_LDC.h"
#ifdef CAN_Super_ID
#undef CAN_Super_ID
#endif
#define CAN_Super_ID			0xABAB
#ifdef CAN_Debug_ID1
#undef CAN_Debug_ID1
#endif
#define CAN_Debug_ID1			0xABAC
#ifdef UART_Debug_ID
#undef UART_Debug_ID
#endif
#define UART_Debug_ID			0xFB
static uint8 UARTDEBUGDATA[32] = {0XAA,0X55};
static uint8 UARTDEBUGDATAT[284] = {0XAA,0X55};
static volatile ldc_candata LDC_CANDATA;
static volatile ldc_uartdata LDC_UARTDATA;
static void OS_Dummy(void){}
static uint16 MyCrc16(uint8* array,uint16 length);
static uint8 CRC8(uint8 *buf,uint16 len);
static uint16 BCC(uint8 *data,uint8 length);
static int8 Parameter_Write(uint16 Member,uint8 Privilege,uint32 Value);
static uint32 Parameter_Read(uint16 Member);
static os_ldc_store Store_Write	= (os_ldc_store)OS_Dummy;			/*非易失性存储器写入程序*/
static os_ldc_can_sent CAN_Debug_Sent = (os_ldc_can_sent)OS_Dummy;		/*CAN发送程序*/
static os_ldc_uart_sent UART_Debug_Sent = (os_ldc_uart_sent)OS_Dummy;		/*UART发送程序*/
extern uint16 USER_Parameter_LENG;
struct system_parameter USER_Parameter = system_parameter_DEFAULTS;
static uint8 ID_Check_Sucess = 0;
//static uint8 Respons_Code = 0;
/****************************************************************************
 * @函数名		void (*Os_Debug_Init)(os_debug_store Store_srite,
 * 									  os_can_debug_sent CAN_debug_sent,
 * 									  os_uart_debug_sent UART_debug_sent);
 *----------------------------------------------------------------------------
 * @描述		Henter_OS_Debug调试协议初始化函数
 *----------------------------------------------------------------------------
 * @输入		Store_srite：用户程序的EEPROM写入程序
 *				CAN_debug_sent：用户程序的CAN发送程序
 *				UART_debug_sent：用户程序的UART发送程序
 *----------------------------------------------------------------------------
 * @输出        无
 *
 *----------------------------------------------------------------------------
 * @返回值		void
 *----------------------------------------------------------------------------
 * @日期		2017年5月9日
 *
 ****************************************************************************/
STATIC void LDC_Init(os_ldc_store Store_srite,os_ldc_can_sent CAN_debug_sent,os_ldc_uart_sent UART_debug_sent)
{
	Store_Write = Store_srite;
	UART_Debug_Sent = UART_debug_sent;
	CAN_Debug_Sent = CAN_debug_sent;
}
/****************************************************************************
 * @函数名		int8 CAN_Debug_Main(uint8 can_node,uint32 ID,uint8 *DATA)
 *----------------------------------------------------------------------------
 * @描述		Henter_OS_Debug can调试协议函数
 *----------------------------------------------------------------------------
 * @输入		can_node：CAN节点
 *				ID：CAN ID
 *				*DATA：指向CAN接收数据的指针
 *----------------------------------------------------------------------------
 * @输出        无
 *
 *----------------------------------------------------------------------------
 * @返回值
 *----------------------------------------------------------------------------
 * @日期		2017年5月9日
 *
 ****************************************************************************/
STATIC int8 CAN_Debug_Main(uint8 can_node,uint32 ID,uint8 *DATA)
{
	int8 state = 0;
	LDC_CANDATA.CAN_Address = ((((DATA[1] & 0x0F) << 8) | DATA[0]) & 0xFFFF);
	LDC_CANDATA.CAN_Function = ((DATA[1] >> 4) & 0x03);
	LDC_CANDATA.CAN_Length = ((DATA[1] >> 7) & 0x01);
	LDC_CANDATA.CAN_Parameter1 = (((DATA[3] << 8) | DATA[2]) & 0xFFFF);
	LDC_CANDATA.CAN_Parameter2 = (((DATA[5] << 8) | DATA[4]) & 0xFFFF);
	LDC_CANDATA.CAN_Password = (((DATA[7] << 8) | DATA[6]) & 0xFFFF);
	if((MyCrc16(DATA,6) == LDC_CANDATA.CAN_Password))
	{
		if((ID == CAN_Super_ID)||(ID == CAN_Debug_ID)||(ID == CAN_DebugUser_ID)||(ID == CAN_Debug_ID1))
		{
			if(LDC_CANDATA.CAN_Function == 1)
			{
				uint8 Data_Buffer[8];
				uint32 data = Parameter_Read(LDC_CANDATA.CAN_Address);
				Data_Buffer[0] = (uint8)(LDC_CANDATA.CAN_Address & 0xFF);
				Data_Buffer[1] = (((LDC_CANDATA.CAN_Address >> 8) & 0xF) | 0x10 | (LDC_CANDATA.CAN_Length << 7));
				Data_Buffer[2] = (uint8)((data >> 0) & 0xFF);
				Data_Buffer[3] = (uint8)((data >> 8) & 0xFF);
				Data_Buffer[4] = (uint8)((data >> 16) & 0xFF);
				Data_Buffer[5] = (uint8)((data >> 24) & 0xFF);
				data = MyCrc16(Data_Buffer,6);
				Data_Buffer[6]=((data >> 0) & 0xFF);
				Data_Buffer[7]=((data >> 8) & 0xFF);
				CAN_Debug_Sent(can_node,ID,1,0,8,Data_Buffer);
				state = 0;
			}
			else if((LDC_CANDATA.CAN_Function == 2)||(LDC_CANDATA.CAN_Function == 3))
			{
				if((ID == CAN_Debug_ID) || (ID == CAN_DebugUser_ID) || (ID == CAN_Debug_ID1))
				{
					state = Parameter_Write(LDC_CANDATA.CAN_Address,7,(LDC_CANDATA.CAN_Parameter2 << 16) | LDC_CANDATA.CAN_Parameter1);
				}
				else if(ID == CAN_Super_ID)//超级ID可以写入任何参数
				{
					state = Parameter_Write(LDC_CANDATA.CAN_Address,0xAB,(LDC_CANDATA.CAN_Parameter2 << 16) | LDC_CANDATA.CAN_Parameter1);
				}
			}
		}
	}
	else
	{
		state=-3;//crc校验错误
	}
	return state;
}
/****************************************************************************
 * @函数名		int8 UART_Debug_Main(uint8 node,uint8 Length,uint8 *DATA)
 *----------------------------------------------------------------------------
 * @描述		Henter_OS_Debug UART调试协议函数
 * 				程序中，功能码总共为3个，分别为0x53,0x56,0x57。.0x53代表调试主机需要读取数据
 * 				中心数据；0x56代表调试主机需要写入数据到数据中心并同时写入到EEPROM；0x57代表
 * 				调试主机需要写入数据到数据中心，但是仅写入RAM而不写入非易失性存储器
 *----------------------------------------------------------------------------
 * @输入		node:UART节点
 *				Length：数据长度
 *				*DATA：指向UART接收数据的指针
 *----------------------------------------------------------------------------
 * @输出        无
 *
 *----------------------------------------------------------------------------
 * @返回值		-1：调试协议头不对或者调试协议CRC校验错误，应用程序不应将数据销毁而应传递给下一层进行解析
 * 				-2：协议头正确，但无权操作
 *----------------------------------------------------------------------------
 * @日期		2017年5月9日
 *
 ****************************************************************************/
STATIC int8 UART_Debug_Main(uint8 node,uint8 Length,uint8 *DATA)
{
	//static uint8 Respons_Code_History[4] = {0};
	//static uint8 Respons_Code_Count = 0;
	uint8 ID_Check;
	uint16 CRC16_Check;
	int8 state;
	//---------------------------------------------------------第一步，验证协议头
	if(Length > 8)//小于8个字节的帧一定有问题
	{
		if(1 == ID_Check_Sucess)
		{
			ID_Check = CRC8(DATA,3);
			if(ID_Check == DATA[3])//校验通过
			{
				LDC_UARTDATA.loc_adress = UART_Debug_ID;
				LDC_UARTDATA.des_adress = DATA[0];
				LDC_UARTDATA.function = DATA[4];
				LDC_UARTDATA.RegCount = DATA[5];
				LDC_UARTDATA.RegStartAdress = (DATA[6] << 8) + DATA[7];
				if(0x53 == LDC_UARTDATA.function)//调试器需要读取数据中心的数据
				{
					LDC_UARTDATA.CRCDATA = (DATA[8] << 8) + DATA[9];
					CRC16_Check = MyCrc16(DATA,8);
					//---------------------------------------------
					if(CRC16_Check == LDC_UARTDATA.CRCDATA)
					{
						UARTDEBUGDATAT[0] = DATA[0];
					}
					else
					{
						state = -1;//调试协议头不对或者调试协议CRC校验错误，应用程序不应将数据销毁而应传递给下一层进行解析
					}
				}
				else if((0x56 == LDC_UARTDATA.function) || (0x57 == LDC_UARTDATA.function))//调试器需要写入数据到数据中心
				{
					if(LDC_UARTDATA.RegCount > 0)
					{
						LDC_UARTDATA.CRCDATA = (DATA[12 + (LDC_UARTDATA.RegCount - 1) * 4] << 8) + DATA[13 + (LDC_UARTDATA.RegCount - 1) * 4];
						CRC16_Check = MyCrc16(DATA,12 + (LDC_UARTDATA.RegCount - 1) * 4);
						//------------------------------------------
						if(CRC16_Check == LDC_UARTDATA.CRCDATA)
						{

						}
						else
						{
							state = -1;//调试协议头不对或者调试协议CRC校验错误，应用程序不应将数据销毁而应传递给下一层进行解析
						}
					}
					else
					{
						state = -1;//调试协议头不对或者调试协议CRC校验错误，应用程序不应将数据销毁而应传递给下一层进行解析
					}
				}
				else
				{
					state = -1;//调试协议头不对或者调试协议CRC校验错误，应用程序不应将数据销毁而应传递给下一层进行解析
				}
			}
			else
			{
				state = -1;//调试协议头不对或者调试协议CRC校验错误，应用程序不应将数据销毁而应传递给下一层进行解析
			}
		}
		else
		{
			state = -1;//调试协议头不对或者调试协议CRC校验错误，应用程序不应将数据销毁而应传递给下一层进行解析
		}
	}
	else
	{
		state = -1;//调试协议头不对或者调试协议CRC校验错误，应用程序不应将数据销毁而应传递给下一层进行解析
	}
	return state;
}
/*****************************************************************************
 * @函数名		void UART_Debug_DSO(uint8 UART_node,
 * 									real32 Parameter_1,
 * 									real32 Parameter_2,
 * 									real32 Parameter_3,
 * 									real32 Parameter_4)
 *---------------------------------------------------------------------------
 * @描述		本地数据中心(LDC)调试协议串口示波器数据发送程序，最多支持4个通道浮点数
 *---------------------------------------------------------------------------
 * @输入		UART_node：指定由哪个UART口发送
 *				Parameter_1：第一通道数据
 *				Parameter_2：第二通道数据
 *				Parameter_3：第三通道数据
 *				Parameter_4：第四通道数据
 *
 *----------------------------------------------------------------------------
 * @输出        无
 *----------------------------------------------------------------------------
 * @返回值		void
 *----------------------------------------------------------------------------
 * @日期		2017年5月9日
 *
 *****************************************************************************/
union
{
	uint32 Parameter_uint32;
	real32 Parameter_float;
}Par_temp;
real32 Parameter_temp;
STATIC void UART_DSO(uint8 UART_node,real32 Parameter_1,real32 Parameter_2,real32 Parameter_3,real32 Parameter_4)
{
	uint32 Temp;
	UARTDEBUGDATA[0] = 0xAA;
	UARTDEBUGDATA[1] = 0x55;
	Par_temp.Parameter_float = Parameter_1;
	UARTDEBUGDATA[2] = (Par_temp.Parameter_uint32 >> 24) & 0xff;
	UARTDEBUGDATA[3] = (Par_temp.Parameter_uint32 >> 16) & 0xff;
	UARTDEBUGDATA[4] = (Par_temp.Parameter_uint32 >> 8) & 0xff;
	UARTDEBUGDATA[5] = (Par_temp.Parameter_uint32 >> 0) & 0xff;
	Par_temp.Parameter_float = Parameter_2;
	UARTDEBUGDATA[6] = (Par_temp.Parameter_uint32 >> 24) & 0xff;
	UARTDEBUGDATA[7] = (Par_temp.Parameter_uint32 >> 16) & 0xff;
	UARTDEBUGDATA[8] = (Par_temp.Parameter_uint32 >> 8) & 0xff;
	UARTDEBUGDATA[9] = (Par_temp.Parameter_uint32 >> 0) & 0xff;
	Par_temp.Parameter_float = Parameter_3;
	UARTDEBUGDATA[10] = (Par_temp.Parameter_uint32 >> 24) & 0xff;
	UARTDEBUGDATA[11] = (Par_temp.Parameter_uint32 >> 16) & 0xff;
	UARTDEBUGDATA[12] = (Par_temp.Parameter_uint32 >> 8) & 0xff;
	UARTDEBUGDATA[13] = (Par_temp.Parameter_uint32 >> 0) & 0xff;
	Par_temp.Parameter_float = Parameter_4;
	UARTDEBUGDATA[14] = (Par_temp.Parameter_uint32 >> 24) & 0xff;
	UARTDEBUGDATA[15] = (Par_temp.Parameter_uint32 >> 16) & 0xff;
	UARTDEBUGDATA[16] = (Par_temp.Parameter_uint32 >> 8) & 0xff;
	UARTDEBUGDATA[17] = (Par_temp.Parameter_uint32 >> 0) & 0xff;
	Temp = BCC(UARTDEBUGDATA,18);
	UARTDEBUGDATA[18] = Temp >> 8;
	UARTDEBUGDATA[19] = Temp;
	UART_Debug_Sent(UART_node,UARTDEBUGDATA,20);
}
/*
 * 	定义调试框架程序的结构体
 */
volatile htos_ldc HTOS_LDC = {
									LDC_Init,
									CAN_Debug_Main,
									UART_Debug_Main,
									UART_DSO,
									Parameter_Write,
									Parameter_Read
								};
/*****************************************************************************
 * @函数名		uint8 BCC(uint8 *data,uint8 length)
 *---------------------------------------------------------------------------
 * @描述		计算校验和
 *---------------------------------------------------------------------------
 * @输入		*data：要校验的数据
 *				length：要校验的数据的长度
 *----------------------------------------------------------------------------
 * @输出        无
 *----------------------------------------------------------------------------
 * @返回值		校验和
 *----------------------------------------------------------------------------
 * @日期		2017年5月9日
 *
 *****************************************************************************/
//STATIC uint8 BCC(uint8 *data,uint8 length)
//{
//	uint8 out;//用于保存异或结果
//	uint8 i;
//	out=0x00;
//	for (i = 0;i < length;i ++)
//	{
//		out ^= data[i];
//	}
//	return out;
//}
STATIC uint16 BCC(uint8 *data,uint8 length)
{
	uint8 i;
	uint8 BCC_out = 0x00;//用于保存异或结果
	uint8 ACC_out = 0x00;//用于保存异或结果
	for (i = 0;i < length;i ++)
	{
		BCC_out ^= data[i];
		ACC_out += data[i];
	}
	return (((uint16)BCC_out << 8) + ACC_out);
}
/*****************************************************************************
 * @函数名		int8 Parameter_Write(uint16 Member,uint8 Privilege,uint32 Value)
 *---------------------------------------------------------------------------
 * @描述		本协议中对参数地址的写入行为
 *---------------------------------------------------------------------------
 * @输入		Member：希望被写入的成员地址；当成员地址的最高位为1时，代表请求为单独写入RAM空间，最高位为0时，将同时写入RAM和EEPROM
 *				Privilege：调用者的权限
 *					调用者权限赋予解释：0,1,2,3,4,5,6,7,8,9,A,B,C
 *					 参数表成员的权限属性，由对应的Attribute_Words[Member]决定
 * 					0,1,2,3			可读写RAM、EEPROM
 * 					4,5,6,7			可读，仅写入RAM
 * 					8,9,10,11		仅可读，不可写
 * 					>12				不可读写
 * 					当该成员权限属性为7时，如果操作权限为Privilege == 6，那么对应的功能将被拒绝。
 * 					举例说明：设成员0的权限为6，该成员仅可写入RAM，不可写入EEPROM。当操作员权限仅为Privilege == 5时，且提交写入请求时，LDC将拒绝写入RAM并返回权限错误
 * 					超级管理操作权限：
 * 						A1:当操作员权限为Privilege == A1时，可以读取和写入权限属性为7及以下的所有成员的RAM和EEPROM；
 * 						A5:当操作员权限为Privilege == A5时，可以读取和写入权限属性为11及以下的所有成员的RAM和EEPROM；
 * 						AB:当操作员权限为Privilege == AB时，可以读取和写入任意权限属性的所有成员的RAM和EEPROM；
 * 				Value：将要写入的数值
 *----------------------------------------------------------------------------
 * @输出        无
 *----------------------------------------------------------------------------
 * @返回值		0：写入成功；，-1：写入地址超过限制；-2：无权限写入；-5：数据超过上下限
 *----------------------------------------------------------------------------
 * @日期		2017年5月9日
 *
 *****************************************************************************/
//#define Parameter_Write(Member,Privilege,Value)		if(Privilege < Attribute_Words[Member]) RAW_SYSDATA[Member] = Value;
STATIC int8 Parameter_Write(uint16 Member,uint8 Privilege,uint32 Value)
{
	uint8 RAM_W = (Member & 0x8000) >> 15;
	Member &= 0xFFF;
	if(Member < USER_Parameter_LENG)//------------------------------地址没有超出了范围
	{
		if(1 == USER_Parameter.sign[Member])//目标值为有符号
		{
			if(((int32)Value >= (int32)USER_Parameter.lower[Member]) && ((int32)Value <= (int32)USER_Parameter.upper[Member]))
			{
				if(((USER_Parameter.attribute[Member] <= 3) && (Privilege >= USER_Parameter.attribute[Member])) || /*操作权限高于成员属性权限*/
				   ((USER_Parameter.attribute[Member] <= 7) && (Privilege == 0xA1)) || /*超级管理员1权限*/
				   ((USER_Parameter.attribute[Member] <= 11) && (Privilege == 0xA5)) || /*超级管理员2权限*/
				   (Privilege == 0xAB) /*超级管理员3权限*/
				   )
				{
					USER_Parameter.value[Member] = Value;
					if(0 == RAM_W)
					Store_Write(Member,USER_Parameter.value[Member]);
					return 0;
				}
				else if((USER_Parameter.attribute[Member] <= 7) && (Privilege >= USER_Parameter.attribute[Member])) /*操作权限高于成员属性权限*/
				{
					USER_Parameter.value[Member] = Value;
					return 0;
				}
				else
				{
					return -2;//无操作权限
				}
			}
			else
			{
				return -5;
			}
		}
		else//目标值为无符号
		{
			if((Value >= USER_Parameter.lower[Member]) && (Value <= USER_Parameter.upper[Member]))
			{
				if(((USER_Parameter.attribute[Member] <= 3) && (Privilege >= USER_Parameter.attribute[Member])) || /*操作权限高于成员属性权限*/
				   ((USER_Parameter.attribute[Member] <= 7) && (Privilege == 0xA1)) || /*超级管理员1权限*/
				   ((USER_Parameter.attribute[Member] <= 11) && (Privilege == 0xA5)) || /*超级管理员2权限*/
				   (Privilege == 0xAB) /*超级管理员3权限*/
				   )
				{
					USER_Parameter.value[Member] = Value;
					if(0 == RAM_W)
					Store_Write(Member,USER_Parameter.value[Member]);
					return 0;
				}
				else if((USER_Parameter.attribute[Member] <= 7) && (Privilege >= USER_Parameter.attribute[Member])) /*操作权限高于成员属性权限*/
				{
					USER_Parameter.value[Member] = Value;
					return 0;
				}
				else
				{
					return -2;//无操作权限
				}
			}
			else
			{
				return -5;
			}
		}
	}
	else//-----------------------------------------------------------地址超出了范围
	{
		return -1;
	}
}
/*****************************************************************************
 * @函数名		uint32 Parameter_Read(uint16 Member)
 *---------------------------------------------------------------------------
 * @描述		本协议中对参数地址的读取
 *---------------------------------------------------------------------------
 * @输入		Member：希望被读取的成员
 *----------------------------------------------------------------------------
 * @输出        无
 *----------------------------------------------------------------------------
 * @返回值		成员的值
 *----------------------------------------------------------------------------
 * @日期		2017年5月9日
 *
 *****************************************************************************/
//#define Parameter_Read(Member)		((const uint32)RAW_SYSDATA[Member])
//#define Parameter_Adress(Member)	(&RAW_SYSDATA[Member])
STATIC uint32 Parameter_Read(uint16 Member)
{
	if(Member < USER_Parameter_LENG)
		return RAW_SYSDATA[Member];
	else
		return 0;
}
//多项式值: 0XA565的CRC表
STATIC const uint16 crc16_tab[]={
0x0000, 0x4722, 0x8E44, 0xC966, 0x4E43, 0x0961, 0xC007, 0x8725,
0x9C86, 0xDBA4, 0x12C2, 0x55E0, 0xD2C5, 0x95E7, 0x5C81, 0x1BA3,
0x6BC7, 0x2CE5, 0xE583, 0xA2A1, 0x2584, 0x62A6, 0xABC0, 0xECE2,
0xF741, 0xB063, 0x7905, 0x3E27, 0xB902, 0xFE20, 0x3746, 0x7064,
0xD78E, 0x90AC, 0x59CA, 0x1EE8, 0x99CD, 0xDEEF, 0x1789, 0x50AB,
0x4B08, 0x0C2A, 0xC54C, 0x826E, 0x054B, 0x4269, 0x8B0F, 0xCC2D,
0xBC49, 0xFB6B, 0x320D, 0x752F, 0xF20A, 0xB528, 0x7C4E, 0x3B6C,
0x20CF, 0x67ED, 0xAE8B, 0xE9A9, 0x6E8C, 0x29AE, 0xE0C8, 0xA7EA,
0xFDD7, 0xBAF5, 0x7393, 0x34B1, 0xB394, 0xF4B6, 0x3DD0, 0x7AF2,
0x6151, 0x2673, 0xEF15, 0xA837, 0x2F12, 0x6830, 0xA156, 0xE674,
0x9610, 0xD132, 0x1854, 0x5F76, 0xD853, 0x9F71, 0x5617, 0x1135,
0x0A96, 0x4DB4, 0x84D2, 0xC3F0, 0x44D5, 0x03F7, 0xCA91, 0x8DB3,
0x2A59, 0x6D7B, 0xA41D, 0xE33F, 0x641A, 0x2338, 0xEA5E, 0xAD7C,
0xB6DF, 0xF1FD, 0x389B, 0x7FB9, 0xF89C, 0xBFBE, 0x76D8, 0x31FA,
0x419E, 0x06BC, 0xCFDA, 0x88F8, 0x0FDD, 0x48FF, 0x8199, 0xC6BB,
0xDD18, 0x9A3A, 0x535C, 0x147E, 0x935B, 0xD479, 0x1D1F, 0x5A3D,
0xA965, 0xEE47, 0x2721, 0x6003, 0xE726, 0xA004, 0x6962, 0x2E40,
0x35E3, 0x72C1, 0xBBA7, 0xFC85, 0x7BA0, 0x3C82, 0xF5E4, 0xB2C6,
0xC2A2, 0x8580, 0x4CE6, 0x0BC4, 0x8CE1, 0xCBC3, 0x02A5, 0x4587,
0x5E24, 0x1906, 0xD060, 0x9742, 0x1067, 0x5745, 0x9E23, 0xD901,
0x7EEB, 0x39C9, 0xF0AF, 0xB78D, 0x30A8, 0x778A, 0xBEEC, 0xF9CE,
0xE26D, 0xA54F, 0x6C29, 0x2B0B, 0xAC2E, 0xEB0C, 0x226A, 0x6548,
0x152C, 0x520E, 0x9B68, 0xDC4A, 0x5B6F, 0x1C4D, 0xD52B, 0x9209,
0x89AA, 0xCE88, 0x07EE, 0x40CC, 0xC7E9, 0x80CB, 0x49AD, 0x0E8F,
0x54B2, 0x1390, 0xDAF6, 0x9DD4, 0x1AF1, 0x5DD3, 0x94B5, 0xD397,
0xC834, 0x8F16, 0x4670, 0x0152, 0x8677, 0xC155, 0x0833, 0x4F11,
0x3F75, 0x7857, 0xB131, 0xF613, 0x7136, 0x3614, 0xFF72, 0xB850,
0xA3F3, 0xE4D1, 0x2DB7, 0x6A95, 0xEDB0, 0xAA92, 0x63F4, 0x24D6,
0x833C, 0xC41E, 0x0D78, 0x4A5A, 0xCD7F, 0x8A5D, 0x433B, 0x0419,
0x1FBA, 0x5898, 0x91FE, 0xD6DC, 0x51F9, 0x16DB, 0xDFBD, 0x989F,
0xE8FB, 0xAFD9, 0x66BF, 0x219D, 0xA6B8, 0xE19A, 0x28FC, 0x6FDE,
0x747D, 0x335F, 0xFA39, 0xBD1B, 0x3A3E, 0x7D1C, 0xB47A, 0xF358
};

/****************************************************************************
 * @函数名			uint16 MyCrc16_A565(uint8* array,uint8 length)
 *----------------------------------------------------------------------------
 * @描述			CRC校验函数
 *
 *----------------------------------------------------------------------------
 * @输入			array：校验的数组，length：数组元素的个数
 *
 *----------------------------------------------------------------------------
 * @输出			校验后的结果
 *
 *----------------------------------------------------------------------------
 * @返回值			校验值
 *
 *----------------------------------------------------------------------------
 * @日期			2017年5月9日
 *
 ****************************************************************************/
STATIC uint16 MyCrc16(uint8* array,uint16 length)
{
	uint16 crc = 0xffff;
	for(uint16 i = 0; i < length; i++)
	{
		//字节到字一是为了和CRC高字节异或运算，
		//另一个作用是扩展8个0.
		crc = crc ^ (uint16)array[i];
		crc = (crc >> 8) ^ crc16_tab[crc & 0x0FF];
	}
	return crc;
}
/******************************************************************
*  CRC8 码表
******************************************************************/
//生成多项式：X8+X5+X4+1=0x31 ，CRC8TAB[1]=生成多项式
STATIC const uint8 CRC8TAB[256]={
//0
0x00, 0x31, 0x62, 0x53, 0xC4, 0xF5, 0xA6, 0x97,
0xB9, 0x88, 0xDB, 0xEA, 0x7D, 0x4C, 0x1F, 0x2E,
//1
0x43, 0x72, 0x21, 0x10, 0x87, 0xB6, 0xE5, 0xD4,
0xFA, 0xCB, 0x98, 0xA9, 0x3E, 0x0F, 0x5C, 0x6D,
//2
0x86, 0xB7, 0xE4, 0xD5, 0x42, 0x73, 0x20, 0x11,
0x3F, 0x0E, 0x5D, 0x6C, 0xFB, 0xCA, 0x99, 0xA8,
//3
0xC5, 0xF4, 0xA7, 0x96, 0x01, 0x30, 0x63, 0x52,
0x7C, 0x4D, 0x1E, 0x2F, 0xB8, 0x89, 0xDA, 0xEB,
//4
0x3D, 0x0C, 0x5F, 0x6E, 0xF9, 0xC8, 0x9B, 0xAA,
0x84, 0xB5, 0xE6, 0xD7, 0x40, 0x71, 0x22, 0x13,
//5
0x7E, 0x4F, 0x1C, 0x2D, 0xBA, 0x8B, 0xD8, 0xE9,
0xC7, 0xF6, 0xA5, 0x94, 0x03, 0x32, 0x61, 0x50,
//6
0xBB, 0x8A, 0xD9, 0xE8, 0x7F, 0x4E, 0x1D, 0x2C,
0x02, 0x33, 0x60, 0x51, 0xC6, 0xF7, 0xA4, 0x95,
//7
0xF8, 0xC9, 0x9A, 0xAB, 0x3C, 0x0D, 0x5E, 0x6F,
0x41, 0x70, 0x23, 0x12, 0x85, 0xB4, 0xE7, 0xD6,
//8
0x7A, 0x4B, 0x18, 0x29, 0xBE, 0x8F, 0xDC, 0xED,
0xC3, 0xF2, 0xA1, 0x90, 0x07, 0x36, 0x65, 0x54,
//9
0x39, 0x08, 0x5B, 0x6A, 0xFD, 0xCC, 0x9F, 0xAE,
0x80, 0xB1, 0xE2, 0xD3, 0x44, 0x75, 0x26, 0x17,
//A
0xFC, 0xCD, 0x9E, 0xAF, 0x38, 0x09, 0x5A, 0x6B,
0x45, 0x74, 0x27, 0x16, 0x81, 0xB0, 0xE3, 0xD2,
//B
0xBF, 0x8E, 0xDD, 0xEC, 0x7B, 0x4A, 0x19, 0x28,
0x06, 0x37, 0x64, 0x55, 0xC2, 0xF3, 0xA0, 0x91,
//C
0x47, 0x76, 0x25, 0x14, 0x83, 0xB2, 0xE1, 0xD0,
0xFE, 0xCF, 0x9C, 0xAD, 0x3A, 0x0B, 0x58, 0x69,
//D
0x04, 0x35, 0x66, 0x57, 0xC0, 0xF1, 0xA2, 0x93,
0xBD, 0x8C, 0xDF, 0xFE, 0x79, 0x48, 0x1B, 0x2A,
//E
0xC1, 0xF0, 0xA3, 0x92, 0x05, 0x34, 0x67, 0x56,
0x78, 0x49, 0x1A, 0x2B, 0xBC, 0x8D, 0xDE, 0xEF,
//F
0x82, 0xB3, 0xE0, 0xD1, 0x46, 0x77, 0x24, 0x15,
0x3B, 0x0A, 0x59, 0x68, 0xFF, 0xCE, 0x9D, 0xAC
};
/******************************************************
*函数名称:CRC8
*输   入:buf 要校验的数据; len 校验数据的长
*输   出:校验值
*功   能:循环冗余校验-8
*******************************************************/
STATIC uint8 CRC8(uint8 *buf,uint16 len)
{
	unsigned char  crc=0;
	while ( len-- )
	{
	crc = CRC8TAB[crc^*buf];

	buf++;
	}

	return crc;
}
