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
 * File: Hunter_OS_LDC_Variable.c
 * Author: Kelei999999(WangLiang)
 * Created on: 2014��5��12��
 * Revision history: 2.0
 */

/*
 * This is a guard condition so that contents of this file are not included
 * more than once.
 */
#include "Hunter_OS_LDC.h"
/*
 * ����ⲿû��ʲôһ�¹����飬��ôģ����Զ�����һ������
 */
#ifdef parameter_Declare
	uint32 RAW_SYSDATA[System_Parameter_LEN];
	const uint32 Lower_Limit[System_Parameter_LEN];
	const uint32 Upper_Limit[System_Parameter_LEN];
	const uint32 Defaults[System_Parameter_LEN];
	const uint8 Attribute_Words[System_Parameter_LEN];
	const uint8 Sign_Bit[System_Parameter_LEN];
#endif
struct system_parameter USER_Parameter = system_parameter_DEFAULTS;				/*����һ��ָ��ָ��ϵͳ����Ĳ�������*/
uint16 USER_Parameter_LENG = System_Parameter_LEN;						/*����һ����������ָʾϵͳ����Ĳ������е��ܳ���*/
const uint16 CAN_Debug_ID			= 0xAAAA;							/*��ID�ǹ̶�����ID,�����ʵ������޸�,��ID������CAN��������*/
volatile uint16 CAN_DebugUser_ID	= 0xAA00;								/*��ID���������ڵ���ʱ�����߳�ͻ,��ID��Ӧ�ò�����޸�*/

