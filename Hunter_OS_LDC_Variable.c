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
 * File: Hunter_OS_LDC_Variable.c
 * Author: Hunter.ORG
 * Created on: 2016��5��12��
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
uint32 RAW_SYSDATA[System_Parameter_LEN];
const uint32 Lower_Limit[System_Parameter_LEN];
const uint32 Upper_Limit[System_Parameter_LEN];
const uint32 Defaults[System_Parameter_LEN];
const uint8 Attribute_Words[System_Parameter_LEN];
const uint8 Sign_Bit[System_Parameter_LEN];
//-----------------------------------------------------------------------------����ϵͳ���в������飬�����ÿһ����Ա��������μ���ϵͳģ������
struct system_parameter SYS_Parameter = system_parameter_DEFAULTS;
//-----------------------------------------------------------------------------
uint16 USER_Parameter_LENG = System_Parameter_LEN;								/*����һ����������ָʾϵͳ����Ĳ������е��ܳ���*/
const uint16 CAN_Debug_ID			= 0xAAAA;									/*��ID�ǹ̶�����ID,�����ʵ������޸�,��ID������CAN��������*/
volatile uint16 CAN_DebugUser_ID	= 0xAA00;									/*��ID���������ڵ���ʱ�����߳�ͻ,��ID��Ӧ�ò�����޸�*/

