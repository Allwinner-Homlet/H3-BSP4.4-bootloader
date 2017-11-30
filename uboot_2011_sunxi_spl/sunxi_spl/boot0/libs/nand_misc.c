/*
**********************************************************************************************************************
*
*						           the Embedded Secure Bootloader System
*
*
*						       Copyright(C), 2006-2014, Allwinnertech Co., Ltd.
*                                           All Rights Reserved
*
* File    :
*
* By      :
*
* Version : V2.00
*
* Date	  :
*
* Descript:
**********************************************************************************************************************
*/
#include "common.h"
#include <private_boot0.h>
#include <private_uboot.h>
#include <asm/arch/dram.h>
#include <asm/arch/clock.h>

extern const boot0_file_head_t  BT0_head;
/*
************************************************************************************************************
*
*                                             function
*
*    name          :
*
*    parmeters     :
*
*    return        :
*
*    note          :
*
*
************************************************************************************************************
*/
int BOOT_NandGetPara(void *param, uint size)
{
    memcpy( (void *)param, BT0_head.prvt_head.storage_data, size);

	return 0;
}
/*
************************************************************************************************************
*
*                                             function
*
*    name          :
*
*    parmeters     :
*
*    return        :
*
*    note          :
*
*
************************************************************************************************************
*/
__u8  *get_page_buf( void )
{

	return (__u8 *)(CONFIG_SYS_SDRAM_BASE + 1024 * 1024);
}

/*******************************************************************************
*��������: g_mod
*����ԭ�ͣ�uint32 g_mod( __u32 dividend, __u32 divisor, __u32 *quot_p )
*��������: ��nand flash��ĳһ�����ҵ�һ����ñ��ݽ������뵽RAM�С�����ɹ�����
*          ��OK�����򣬷���ERROR��
*��ڲ���: dividend          ���롣������
*          divisor           ���롣����
*          quot_p            �������
*�� �� ֵ: ����
*******************************************************************************/
__u32 g_mod( __u32 dividend, __u32 divisor, __u32 *quot_p )
{
	if( divisor == 0 )
	{
		*quot_p = 0;
		return 0;
	}
	if( divisor == 1 )
	{
		*quot_p = dividend;
		return 0;
	}

	for( *quot_p = 0; dividend >= divisor; ++(*quot_p) )
		dividend -= divisor;
	return dividend;
}


void set_dram_para(void *dram_addr , __u32 dram_size, __u32 boot_cpu)
{
	__dram_para_t   *dram_para = (__dram_para_t *)dram_addr;
	struct spare_boot_head_t  *uboot_buf = (struct spare_boot_head_t *)CONFIG_SYS_TEXT_BASE;
#if defined (CONFIG_ARCH_SUN7I) || defined(CONFIG_ARCH_SUN5I)
	dram_para->dram_size = dram_size;
#else
	dram_para->dram_para1 &= 0xffff0000;
	dram_para->dram_para1 |= (dram_size & 0xffff);
#endif
	memcpy((void *)uboot_buf->boot_data.dram_para, dram_addr, 32 * sizeof(int));
#ifdef CONFIG_BOOT_A15
	uboot_buf->boot_data.reserved[0] = boot_cpu;
#endif
	return;
}


void cpu_init_s(void)
{
	timer_init();

	set_pll();
}

