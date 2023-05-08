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
 * Created on: 2014��5��12��
 * Revision history: 2.0
 */

/*
 * This is a guard condition so that contents of this file are not included
 * more than once.
 */

/*
 *
 * Hunter_OS_LDC��Hunter_OS�ײ�ı����������ģ�ͬʱҲ��Ӧ�ó���Ļ������ݽṹ��Ӧ�ñ�
 * �ܹ�ʱ����Ҫ�û�����
 * const uint32 Lower_Limit[System_Parameter_LEN]��
 * const uint32 Upper_Limit[System_Parameter_LEN]��
 * const uint32 Defaults[System_Parameter_LEN]��
 * const uint8 Attribute_Words[System_Parameter_LEN]��
 * const uint8 Sign_Bit[System_Parameter_LEN]��
 * uint32 RAW_SYSDATA[System_Parameter_LEN]
 * 6�����顣
 * ϵͳ��ܶ���������һ���ṹ��
 * struct system_parameter
 * {
 *	uint32 *value;				//��ǰֵ��ָ��RAW_SYSDATA[]
 *	const uint32 *lower; 		//���ޣ�ָ��Lower_Limit[]
 *	const uint32 *upper;		//���ޣ�ָ��Upper_Limit[]
 *	const uint32 *defaultValue;	//Ĭ��ֵ��ָ��Defaults[]
 *	const uint8 *pp;			//���ԣ�ָ��Attribute_Words[]
 *	const uint8 *sign;			//���ţ�ָ��Sign_Bit[]
 * };
 *
 */
#ifndef HUNTER_OS_LDC_H_
#define HUNTER_OS_LDC_H_
//----------------------------------------------����Ϊϵͳͷ�ļ�
#include "Typedef.h"
//----------------------------------------------������������(LDC)��ܲ�������
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
 * 	��ܽṹ������ָ��
 */
struct system_parameter
{
	uint32 *value;					//��ǰֵ
	const uint32 *lower; 			//����
	const uint32 *upper;			//����
	const uint32 *defaultValue;		//Ĭ��ֵ
	const uint8 *attribute;			//����(Ȩ��)
	const uint8 *sign;				//����ָʾ
};
/*
 * 	�����ܽṹ��ֻ������ָ��
 */
struct csystem_parameter
{
	const uint32 *value;			//��ǰֵ
	const uint32 *lower; 			//����
	const uint32 *upper;			//����
	const uint32 *defaultValue;		//Ĭ��ֵ
	const uint8 *attribute;			//����(Ȩ��)
	const uint8 *sign;				//����ָʾ
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
 * ����ģʽ��ֹ����Compatible==1ʱ��ʹ�ܼ���ģʽ��ϵͳ���Ⱥ���������Ϊȫ�ֺ������û���ʹ�ú���ʱ��Ҫע��
 * �������Ʋ�����ϵͳ�ڲ�������ͬ�����رռ���ģʽ������ϵͳ�ڲ�������Ϊ�ֲ��������������ⲿ���á�
 */
#define	Compatible						0
#if (Compatible == 0)
	#define STATIC						static
#else
	#define STATIC
#endif
/*
 * ***************************************************************************CAN Debug�����ṹ�塣
 * ʹ��˵����
 * 1������Hunter_OS_LDC_Variable.c�ļ��н���Ӧ���û�����������ȷ�ĳ�ʼ��
 * 2���ٵ���HTOS_LDC.Os_LDC_Init()���г�ʼ���������Ӧ�Ĵ洢����ͷ��ͳ���
 * 3����CAN���յ��ж��е���HTOS_LDC.Os_CAN_Debug_Main()���������Ӧ������
 * 4����UART���յ��ж��е���HTOS_LDC.Os_UART_Debug_Main()���������Ӧ������
 */
typedef int8 (*os_ldc_store)(uint16 address,uint32 data);
typedef int8 (*os_ldc_can_sent)(uint8 ModuleNode,uint32 ID,uint8 IDE,uint8 RTR,uint8 DLC,uint8* DATA);
typedef int8 (*os_ldc_uart_sent)(uint8 Node,uint8 *TransData,uint16 Tx_Length);
typedef struct
{
	/****************************************************************************
	 * @������		void (*Os_LDC_Init)(os_debug_store Store_write,
	 * 									  os_can_debug_sent CAN_debug_sent,
	 * 									  os_uart_debug_sent UART_debug_sent);
	 *----------------------------------------------------------------------------
	 * @����		������������(LDC)����ϵͳ��ʼ������ָ��
	 *----------------------------------------------------------------------------
	 * @����		Store_write���û������EEPROMд�����
	 *				CAN_debug_sent���û������CAN���ͳ���
	 *				UART_debug_sent���û������UART���ͳ���
	 *----------------------------------------------------------------------------
	 * @���        ��
	 *
	 *----------------------------------------------------------------------------
	 * @����ֵ		void
	 *----------------------------------------------------------------------------
	 * @����		2017��5��9��
	 *
	 ****************************************************************************/
	void (*Os_LDC_Init)(os_ldc_store Store_write,os_ldc_can_sent CAN_sent,os_ldc_uart_sent UART_sent);
	/****************************************************************************
	 * @������		int8 (*Os_LDC_CAN_Debug)(uint8 can_node,uint8 *DATA);
	 *----------------------------------------------------------------------------
	 * @����		������������(LDC)CAN���������
	 *----------------------------------------------------------------------------
	 * @����		can_node��CAN���͵Ľڵ��
	 *				*DATA��ָ��CAN�ж��н��յ��������ָ�룬��Э����CAN���ߵ�8���ֽڶ�ʹ�õ���
	 *
	 *----------------------------------------------------------------------------
	 * @���		Э���Ӧ�Ĺ��ܺ��ڼ䷢���Ĵ���
	 *
	 *----------------------------------------------------------------------------
	 * @����ֵ		state ��ʾЭ��������״̬��state=0Ϊ��ȷ��state=-1��ʾE2д����
	 *				state=-5��ʾ����ֵ��Χ���ԣ�state=-3��ʾ��Կ����ȷ��state=-4��ʾͨ��������
	 *----------------------------------------------------------------------------
	 * @����		2017��5��9��
	 *
	 ****************************************************************************/
	int8 (*Os_LDC_CAN_Debug)(uint8 can_node,uint32 ID,uint8 *DATA);
	/****************************************************************************
	 * @������		int8 Os_LDC_UART_Debug(uint8 node,uint8 Length,uint8 *DATA)
	 *----------------------------------------------------------------------------
	 * @����		������������(LDC)UART����ϵͳ������
	 * 				�����У��������ܹ�Ϊ3�����ֱ�Ϊ0x53,0x56,0x57��.0x53�������������Ҫ��ȡ����
	 * 				�������ݣ�0x56�������������Ҫд�����ݵ��������Ĳ�ͬʱд�뵽EEPROM��0x57����
	 * 				����������Ҫд�����ݵ��������ģ����ǽ�д��RAM����д�����ʧ�Դ洢��
	 *----------------------------------------------------------------------------
	 * @����		node:UART�ڵ�
	 *				Length�����ݳ���
	 *				*DATA��ָ��UART�������ݵ�ָ��
	 *----------------------------------------------------------------------------
	 * @���        ��
	 *
	 *----------------------------------------------------------------------------
	 * @����ֵ		-1������Э��ͷ���Ի��ߵ���Э��CRCУ�����Ӧ�ó���Ӧ���������ٶ�Ӧ���ݸ���һ����н���
	 * 				-2��Э��ͷ��ȷ������Ȩ����
	 *----------------------------------------------------------------------------
	 * @����		2017��5��9��
	 *
	 ****************************************************************************/
	int8 (*Os_LDC_UART_Debug)(uint8 node,uint8 Length,uint8 *DATA);/**/
	/*****************************************************************************
	 * @������		void Os_UART_DSO(uint8 UART_node,
	 * 									real32 Parameter_1,
	 * 									real32 Parameter_2,
	 * 									real32 Parameter_3,
	 * 									real32 Parameter_4)
	 *---------------------------------------------------------------------------
	 * @����		������������(LDC)����Э�鴮��ʾ�������ݷ��ͳ������֧��4��ͨ��������
	 *---------------------------------------------------------------------------
	 * @����		UART_node��ָ�����ĸ�UART�ڷ���
	 *				Parameter_1����һͨ������
	 *				Parameter_2���ڶ�ͨ������
	 *				Parameter_3������ͨ������
	 *				Parameter_4������ͨ������
	 *
	 *----------------------------------------------------------------------------
	 * @���        ��
	 *----------------------------------------------------------------------------
	 * @����ֵ		void
	 *----------------------------------------------------------------------------
	 * @����		2017��5��9��
	 *
	 *****************************************************************************/
	void (*Os_LDC_UART_DSO)(uint8 UART_node,real32 Parameter_1,real32 Parameter_2,real32 Parameter_3,real32 Parameter_4);
	/*****************************************************************************
	 * @������		int8 Os_LDC_ParameterWrite(uint16 Member,uint8 Privilege,uint32 Value)
	 *---------------------------------------------------------------------------
	 * @����		������������(LDC)������ַ��д����Ϊ
	 *---------------------------------------------------------------------------
	 * @����		Member��ϣ����д��ĳ�Ա��ַ������Ա��ַ�����λΪ1ʱ����������Ϊ����д��RAM�ռ䣬���λΪ0ʱ����ͬʱд��RAM��EEPROM
	 *				Privilege�������ߵ�Ȩ��
	 *					������Ȩ�޸�����ͣ�0,1,2,3,4,5,6,7,8,9,A,B,C
	 *					 �������Ա��Ȩ�����ԣ��ɶ�Ӧ��Attribute_Words[Member]����
	 * 					0,1,2,3			�ɶ�дRAM��EEPROM
	 * 					4,5,6,7			�ɶ�����д��RAM
	 * 					8,9,10,11		���ɶ�������д
	 * 					>12				���ɶ�д
	 * 					���ó�ԱȨ������Ϊ7ʱ���������Ȩ��ΪPrivilege == 6����ô��Ӧ�Ĺ��ܽ����ܾ���
	 * 					����˵�������Ա0��Ȩ��Ϊ6���ó�Ա����д��RAM������д��EEPROM��������ԱȨ�޽�ΪPrivilege == 5ʱ�����ύд������ʱ��LDC���ܾ�д��RAM������Ȩ�޴���
	 * 					�����������Ȩ�ޣ�
	 * 						A1:������ԱȨ��ΪPrivilege == A1ʱ�����Զ�ȡ��д��Ȩ������Ϊ7�����µ����г�Ա��RAM��EEPROM��
	 * 						A5:������ԱȨ��ΪPrivilege == A5ʱ�����Զ�ȡ��д��Ȩ������Ϊ11�����µ����г�Ա��RAM��EEPROM��
	 * 						AB:������ԱȨ��ΪPrivilege == ABʱ�����Զ�ȡ��д������Ȩ�����Ե����г�Ա��RAM��EEPROM��
	 * 				Value����Ҫд�����ֵ
	 *----------------------------------------------------------------------------
	 * @���        ��
	 *----------------------------------------------------------------------------
	 * @����ֵ		0��д��ɹ�����-1��д���ַ�������ƣ�-2����Ȩ��д�룻-5�����ݳ���������
	 *----------------------------------------------------------------------------
	 * @����		2017��5��9��
	 *
	 *****************************************************************************/
	int8 (*Os_LDC_ParameterWrite)(uint16 Member,uint8 Privilege,uint32 Value);
	/*****************************************************************************
	 * @������		uint32 Os_LDC_ParameterRead(uint16 Member)
	 *---------------------------------------------------------------------------
	 * @����		������������(LDC)�Բ�����ַ�Ķ�ȡ
	 *---------------------------------------------------------------------------
	 * @����		Member��ϣ������ȡ�ĳ�Ա
	 *----------------------------------------------------------------------------
	 * @���        ��
	 *----------------------------------------------------------------------------
	 * @����ֵ		��Ա��ֵ
	 *----------------------------------------------------------------------------
	 * @����		2017��5��9��
	 *
	 *****************************************************************************/
	uint32 (*Os_LDC_ParameterRead)(uint16 Member);
}htos_ldc;
extern volatile htos_ldc HTOS_LDC;
typedef struct
{
	uint16  		CAN_Address;						/*��ַ*/
	uint16  		CAN_Function;						/*����*/
	uint16  		CAN_Length;							/*����*/
	uint16  		CAN_Parameter1;						/*����1(��������)*/
	uint16  		CAN_Parameter2;						/*����2(��������)*/
	uint16  		CAN_Password;						/*У����*/
	uint32  		CAN_ID;								/*ID��*/
}ldc_candata;
typedef	struct
{
	uint8 loc_adress;				/*������ַ*/
	uint8 des_adress;				/*�ӻ���ַ*/
	uint8 function;					/*������*/
	uint16 RegStartAdress;			/*�Ĵ�����ʼ��ַ*/
	uint16 RegCount;				/*Ҫ�����ļĴ�����Ŀ*/
	uint16 *data;					/*ָ�����ݵ�ָ��*/
	uint16 CRCDATA;					/*crcУ����*/
}ldc_uartdata;
#endif /* HUNTER_OS_H_ */

