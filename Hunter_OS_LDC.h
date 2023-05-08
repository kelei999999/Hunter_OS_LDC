/* Kelei999999(WangLiang) all rights reserved.  You may use this software
 * and any derivatives exclusively with Kelei999999(WangLiang) products.
 *
 * THIS SOFTWARE IS SUPPLIED BY Kelei999999(WangLiang) "AS IS".  NO WARRANTIES, WHETHER
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH Kelei999999(WangLiang) PRODUCTS, COMBINATION
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.
 *
 * IN NO EVENT WILL Kelei999999(WangLiang) BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF Kelei999999(WangLiang) HAS
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE
 * FULLEST EXTENT ALLOWED BY LAW, Kelei999999(WangLiang)'S TOTAL LIABILITY ON ALL CLAIMS
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF
 * ANY, THAT YOU HAVE PAID DIRECTLY TO Kelei999999(WangLiang) FOR THIS SOFTWARE.
 *
 * Kelei999999(WangLiang) PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
 * TERMS.
 */

/*
 * File: Hunter_OS_LDC.h
 * Author: Kelei999999(WangLiang)
 * Created on: 2014年5月12日
 * Revision history: 2.0
 */

/*
 * This is a guard condition so that contents of this file are not included
 * more than once.
 */

/*
 *
 * Hunter_OS_LDC是Hunter_OS底层的本地数据中心，同时也是应用程序的基础数据结构。应用本
 * 架构时，需要用户定义
 * const uint32 Lower_Limit[System_Parameter_LEN]，
 * const uint32 Upper_Limit[System_Parameter_LEN]，
 * const uint32 Defaults[System_Parameter_LEN]，
 * const uint8 Attribute_Words[System_Parameter_LEN]，
 * const uint8 Sign_Bit[System_Parameter_LEN]，
 * uint32 RAW_SYSDATA[System_Parameter_LEN]
 * 6个数组。
 * 系统框架定义了如下一个结构体
 * struct system_parameter
 * {
 *	uint32 *value;				//当前值，指向RAW_SYSDATA[]
 *	const uint32 *lower; 		//下限，指向Lower_Limit[]
 *	const uint32 *upper;		//上限，指向Upper_Limit[]
 *	const uint32 *defaultValue;	//默认值，指向Defaults[]
 *	const uint8 *pp;			//属性，指向Attribute_Words[]
 *	const uint8 *sign;			//符号，指向Sign_Bit[]
 * };
 *
 */
#ifndef HUNTER_OS_LDC_H_
#define HUNTER_OS_LDC_H_
//----------------------------------------------以下为系统头文件
#include "Typedef.h"
//----------------------------------------------本地数据中心(LDC)框架参数定义
#ifndef System_Parameter_LEN
	#define System_Parameter_LEN			800
#endif
#define 	system_parameter_DEFAULTS 			{RAW_SYSDATA,Lower_Limit,Upper_Limit,Defaults,Attribute_Words,Sign_Bit}
#define		EEPROMINITCode				0x5555
#define		EEPROMINITCode1				0xAAAAAAAA
#define		EEPROMINITCode2				0x13572468
#define		EEPROMINIT					(System_Parameter_LEN + 0)
#define		EEPROMINIT1					(System_Parameter_LEN + 1)
#define		EEPROMINIT2					(System_Parameter_LEN + 2)
/*
 * 	框架结构体数组指针
 */
struct system_parameter
{
	uint32 *value;					//当前值
	const uint32 *lower; 			//下限
	const uint32 *upper;			//上限
	const uint32 *defaultValue;		//默认值
	const uint8 *attribute;			//属性(权限)
	const uint8 *sign;				//符号指示
};
/*
 * 	定义框架结构体只读数组指针
 */
struct csystem_parameter
{
	const uint32 *value;			//当前值
	const uint32 *lower; 			//下限
	const uint32 *upper;			//上限
	const uint32 *defaultValue;		//默认值
	const uint8 *attribute;			//属性(权限)
	const uint8 *sign;				//符号指示
};
extern uint32 RAW_SYSDATA[System_Parameter_LEN];
extern const uint32 Lower_Limit[System_Parameter_LEN];
extern const uint32 Upper_Limit[System_Parameter_LEN];
extern const uint32 Defaults[System_Parameter_LEN];
extern const uint8 Attribute_Words[System_Parameter_LEN];
extern const uint8 Sign_Bit[System_Parameter_LEN];
extern struct system_parameter USER_Parameter;
extern const uint16 CAN_Debug_ID;
extern volatile uint16 CAN_DebugUser_ID;
//----------------------------------------------
/*
 * 兼容模式禁止，当Compatible==1时，使能兼容模式。系统调度函数均声明为全局函数，用户在使用函数时需要注意
 * 函数名称不能与系统内部名称相同。当关闭兼容模式后，所由系统内部函数均为局部函数，不允许外部调用。
 */
#define	Compatible						0
#if (Compatible == 0)
	#define STATIC						static
#else
	#define STATIC
#endif
/*
 * ***************************************************************************CAN Debug函数结构体。
 * 使用说明：
 * 1、先在Hunter_OS_LDC_Variable.c文件中将对应的用户变量进行正确的初始化
 * 2、再调用HTOS_LDC.Os_LDC_Init()进行初始化，填入对应的存储程序和发送程序
 * 3、在CAN接收的中断中调用HTOS_LDC.Os_CAN_Debug_Main()，并填入对应的数据
 * 4、在UART接收的中断中调用HTOS_LDC.Os_UART_Debug_Main()，并填入对应的数据
 */
typedef int8 (*os_ldc_store)(uint16 address,uint32 data);
typedef int8 (*os_ldc_can_sent)(uint8 ModuleNode,uint32 ID,uint8 IDE,uint8 RTR,uint8 DLC,uint8* DATA);
typedef int8 (*os_ldc_uart_sent)(uint8 Node,uint8 *TransData,uint16 Tx_Length);
typedef struct
{
	/****************************************************************************
	 * @函数名		void (*Os_LDC_Init)(os_debug_store Store_write,
	 * 									  os_can_debug_sent CAN_debug_sent,
	 * 									  os_uart_debug_sent UART_debug_sent);
	 *----------------------------------------------------------------------------
	 * @描述		本地数据中心(LDC)调试系统初始化函数指针
	 *----------------------------------------------------------------------------
	 * @输入		Store_write：用户程序的EEPROM写入程序
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
	void (*Os_LDC_Init)(os_ldc_store Store_write,os_ldc_can_sent CAN_sent,os_ldc_uart_sent UART_sent);
	/****************************************************************************
	 * @函数名		int8 (*Os_LDC_CAN_Debug)(uint8 can_node,uint8 *DATA);
	 *----------------------------------------------------------------------------
	 * @描述		本地数据中心(LDC)CAN诊断主程序
	 *----------------------------------------------------------------------------
	 * @输入		can_node：CAN发送的节点号
	 *				*DATA：指向CAN中断中接收到的数组的指针，本协议中CAN总线的8个字节都使用到了
	 *
	 *----------------------------------------------------------------------------
	 * @输出		协议对应的功能和期间发生的错误
	 *
	 *----------------------------------------------------------------------------
	 * @返回值		state 表示协议所处的状态，state=0为正确，state=-1表示E2写错误，
	 *				state=-5表示参数值范围不对，state=-3表示密钥不正确，state=-4表示通信填充错误
	 *----------------------------------------------------------------------------
	 * @日期		2017年5月9日
	 *
	 ****************************************************************************/
	int8 (*Os_LDC_CAN_Debug)(uint8 can_node,uint32 ID,uint8 *DATA);
	/****************************************************************************
	 * @函数名		int8 Os_LDC_UART_Debug(uint8 node,uint8 Length,uint8 *DATA)
	 *----------------------------------------------------------------------------
	 * @描述		本地数据中心(LDC)UART调试系统主程序
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
	int8 (*Os_LDC_UART_Debug)(uint8 node,uint8 Length,uint8 *DATA);/**/
	/*****************************************************************************
	 * @函数名		void Os_UART_DSO(uint8 UART_node,
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
	void (*Os_LDC_UART_DSO)(uint8 UART_node,real32 Parameter_1,real32 Parameter_2,real32 Parameter_3,real32 Parameter_4);
	/*****************************************************************************
	 * @函数名		int8 Os_LDC_ParameterWrite(uint16 Member,uint8 Privilege,uint32 Value)
	 *---------------------------------------------------------------------------
	 * @描述		本地数据中心(LDC)参数地址的写入行为
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
	int8 (*Os_LDC_ParameterWrite)(uint16 Member,uint8 Privilege,uint32 Value);
	/*****************************************************************************
	 * @函数名		uint32 Os_LDC_ParameterRead(uint16 Member)
	 *---------------------------------------------------------------------------
	 * @描述		本地数据中心(LDC)对参数地址的读取
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
	uint32 (*Os_LDC_ParameterRead)(uint16 Member);
}htos_ldc;
extern volatile htos_ldc HTOS_LDC;
typedef struct
{
	uint16  		CAN_Address;						/*地址*/
	uint16  		CAN_Function;						/*功能*/
	uint16  		CAN_Length;							/*长度*/
	uint16  		CAN_Parameter1;						/*参数1(参数低字)*/
	uint16  		CAN_Parameter2;						/*参数2(参数高字)*/
	uint16  		CAN_Password;						/*校验码*/
	uint32  		CAN_ID;								/*ID号*/
}ldc_candata;
typedef	struct
{
	uint8 loc_adress;				/*本机地址*/
	uint8 des_adress;				/*从机地址*/
	uint8 function;					/*功能码*/
	uint16 RegStartAdress;			/*寄存器起始地址*/
	uint16 RegCount;				/*要操作的寄存器数目*/
	uint16 *data;					/*指向数据的指针*/
	uint16 CRCDATA;					/*crc校验码*/
}ldc_uartdata;
#endif /* HUNTER_OS_H_ */

