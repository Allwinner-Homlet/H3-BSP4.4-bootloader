/*
************************************************************************************************************************
*                                                         eGON
*                                         the Embedded GO-ON Bootloader System
*
*                             Copyright(C), 2006-2008, SoftWinners Microelectronic Co., Ltd.
*											       All Rights Reserved
*
* File Name : Boot0_head.c
*
* Author : Gary.Wang
*
* Version : 1.1.0
*
* Date : 2007.11.06
*
* Description : This file defines the file head part of Boot0, which contains some important
*             infomations such as magic, platform infomation and so on, and MUST be allocted in the
*             head of Boot0.
*
* Others : None at present.
*
*
* History :
*
*  <Author>        <time>       <version>      <description>
*
* Gary.Wang       2007.11.06      1.1.0        build the file
*
************************************************************************************************************************
*/
#include "common.h"
#include <private_boot0.h>

extern char boot0_hash_value[64];
const boot0_file_head_t  BT0_head = {
	                                  {
	      /* jump_instruction */          ( 0xEA000000 | ( ( ( sizeof( boot0_file_head_t ) + sizeof(boot0_hash_value)+ sizeof( int ) - 1 ) / sizeof( int ) - 2 ) & 0x00FFFFFF ) ),
							   		      BOOT0_MAGIC,
							   		      STAMP_VALUE,
#ifdef ALIGN_SIZE_8K
									      0x2000,
#else
                                          0x4000,
#endif
							   		      sizeof( boot_file_head_t ),
							   		      {
							   		      	0, 0, 1, 0
							   		      },
							   		      CONFIG_BOOT0_RET_ADDR,
							   		      CONFIG_BOOT0_RUN_ADDR,
							   		      0,
							   		      {
							   		      	0, 0, '4','.','2','.','0',0
							   		      },
							 	      },

							 	  };



/*******************************************************************************
*
*                  ����Boot_file_head�е�jump_instruction�ֶ�
*
*  jump_instruction�ֶδ�ŵ���һ����תָ�( B  BACK_OF_Boot_file_head )������
*תָ�ִ�к󣬳�����ת��Boot_file_head�����һ��ָ�
*
*  ARMָ���е�Bָ��������£�
*          +--------+---------+------------------------------+
*          | 31--28 | 27--24  |            23--0             |
*          +--------+---------+------------------------------+
*          |  cond  | 1 0 1 0 |        signed_immed_24       |
*          +--------+---------+------------------------------+
*  ��ARM Architecture Reference Manual�����ڴ�ָ�������½��ͣ�
*  Syntax :
*  B{<cond>}  <target_address>
*    <cond>    Is the condition under which the instruction is executed. If the
*              <cond> is ommitted, the AL(always,its code is 0b1110 )is used.
*    <target_address>
*              Specified the address to branch to. The branch target address is
*              calculated by:
*              1.  Sign-extending the 24-bit signed(wro's complement)immediate
*                  to 32 bits.
*              2.  Shifting the result left two bits.
*              3.  Adding to the contents of the PC, which contains the address
*                  of the branch instruction plus 8.
*
*  �ɴ˿�֪����ָ���������8λΪ��0b11101010����24λ����Boot_file_head�Ĵ�С��
*̬���ɣ�����ָ�����װ�������£�
*  ( sizeof( boot_file_head_t ) + sizeof( int ) - 1 ) / sizeof( int )
*                                              ����ļ�ͷռ�õġ��֡��ĸ���
*  - 2                                         ��ȥPCԤȡ��ָ������
*  & 0x00FFFFFF                                ���signed-immed-24
*  | 0xEA000000                                ��װ��Bָ��
*
*******************************************************************************/

