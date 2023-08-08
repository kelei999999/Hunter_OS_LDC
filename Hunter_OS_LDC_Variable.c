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
 * Created on: 2016年5月12日
 * Revision history: 2.0
 */

/*
 * This is a guard condition so that contents of this file are not included
 * more than once.
 */
#include "Hunter_OS_LDC.h"
/*
 * 如果外部没有什么一下工作组，那么模块会自动定义一下数组
 */
uint32 RAW_SYSDATA[System_Parameter_LEN];
const uint32 Lower_Limit[System_Parameter_LEN];
const uint32 Upper_Limit[System_Parameter_LEN];
const uint32 Defaults[System_Parameter_LEN];
const uint8 Attribute_Words[System_Parameter_LEN];
const uint8 Sign_Bit[System_Parameter_LEN];
//-----------------------------------------------------------------------------定义系统运行参数数组，数组的每一个成员的作用请参见《系统模拟量表》
struct system_parameter SYS_Parameter = system_parameter_DEFAULTS;
//-----------------------------------------------------------------------------
uint16 USER_Parameter_LENG = System_Parameter_LEN;								/*定义一个变量用于指示系统定义的参数阵列的总长度*/
const uint16 CAN_Debug_ID			= 0xAAAA;									/*本ID是固定调试ID,请根据实际情况修改,此ID仅用于CAN发送数据*/
volatile uint16 CAN_DebugUser_ID	= 0xAA00;									/*本ID用于区分在调试时的总线冲突,本ID在应用层可以修改*/

