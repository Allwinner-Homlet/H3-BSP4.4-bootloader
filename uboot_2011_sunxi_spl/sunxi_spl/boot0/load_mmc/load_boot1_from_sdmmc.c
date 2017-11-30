/*
************************************************************************************************************************
*                                                         eGON
*                                         the Embedded GO-ON Bootloader System
*
*                             Copyright(C), 2006-2009, SoftWinners Microelectronic Co., Ltd.
*											       All Rights Reserved
*
* File Name   : load_boot1_from_sdmmc.c
*
* Author      : Gary.Wang
*
* Version     : 1.1.0
*
* Date        : 2009.12.08
*
* Description :
*
* Others      : None at present.
*
*
* History     :
*
*  <Author>        <time>       <version>      <description>
*
* Gary.Wang      2009.12.08       1.1.0        build the file
*
************************************************************************************************************************
*/
#include "common.h"
#include "spare_head.h"
#include "private_boot0.h"
#include "private_uboot.h"
#include <asm/arch/mmc_boot0.h>

extern __s32 check_magic( __u32 *mem_base, const char *magic );
extern __s32 check_sum( __u32 *mem_base, __u32 size );

extern const boot0_file_head_t  BT0_head;


typedef struct _boot_sdcard_info_t
{
	__s32               card_ctrl_num;                //�ܹ��Ŀ��ĸ���
	__s32				boot_offset;                  //ָ��������֮���߼�����������Ĺ���
	__s32 				card_no[4];                   //��ǰ�����Ŀ���, 16-31:GPIO��ţ�0-15:ʵ�ʿ����������
	__s32 				speed_mode[4];                //�����ٶ�ģʽ��0�����٣�����������
	__s32				line_sel[4];                  //�������ƣ�0: 1�ߣ�������4��
	__s32				line_count[4];                //��ʹ���ߵĸ���
	__s32  	        	sdc_2xmode[4];
	__s32		        ddrmode[4];
        __s32                   sdc_f_max[4];        
}
boot_sdcard_info_t;

extern const boot0_file_head_t  BT0_head;
/*******************************************************************************
*��������: load_boot1_from_sdmmc
*����ԭ�ͣ�int32 load_boot1_from_sdmmc( __u8 card_no )
*��������: ��һ�ݺõ�Boot1��sdmmc flash�����뵽SRAM�С�
*��ڲ���: void
*�� �� ֵ: OK                         ���벢У��ɹ�
*          ERROR                      ���벢У��ʧ��
*��    ע:
*******************************************************************************/
__s32 load_boot1_from_sdmmc( char *buf)
{
    __u32  length;
    __s32  card_no, i;
	struct spare_boot_head_t  *bfh = (struct spare_boot_head_t *) CONFIG_SYS_TEXT_BASE;;
	boot_sdcard_info_t  *sdcard_info = (boot_sdcard_info_t *)buf;

	i = BT0_head.boot_head.platform[0];
	printf("card boot number = %d\n", i);

	//for(i=0;i<4;i++)
	{
		/* open sdmmc */
		card_no = i;
		printf("card no is %d\n", card_no);
		if(card_no < 0)
		{
			printf("bad card number %d in card boot\n", card_no);

			goto __card_op_fail__;
		}
		printf("sdcard %d line count %d\n", card_no, sdcard_info->line_sel[i] );
		if(!sdcard_info->line_sel[i])
		{
			sdcard_info->line_sel[i] = 4;
		}
		if( sunxi_mmc_init( card_no, sdcard_info->line_sel[i], BT0_head.prvt_head.storage_gpio, 16 ,(void *)(sdcard_info)) == -1)   //���ٿ���4������
		{
			printf("Fail in Init sdmmc.\n");
			goto __card_op_fail__;
		}
		printf("sdcard %d init ok\n", card_no);
#ifndef CONFIG_SUNXI_SECURE_SYSTEM
		/* load 1k uboot head */
	    if( mmc_bread( card_no, UBOOT_START_SECTOR_IN_SDMMC, 1024/512, (void *)CONFIG_SYS_TEXT_BASE ) != (1024/512))
		{
			printf("Fail in reading uboot head.\n");
			goto __card_op_fail__;
		}
		/* check head */
		if( check_magic( (__u32 *)CONFIG_SYS_TEXT_BASE, UBOOT_MAGIC ) != 0 )
		{
			printf("ERROR! NOT find the head of uboot.\n");
			goto __card_op_fail__;
		}
		/* check length */
	    length =  bfh->boot_head.length;
		printf("The size of uboot is %x.\n", length );
	    if( ( length & ( 8 * 1024 - 1 ) ) != 0 )
	    {
	    	printf("boot0 length is NOT align.\n");
	    	goto __card_op_fail__;
	    }
	    if( mmc_bread( card_no, UBOOT_START_SECTOR_IN_SDMMC, length/512, (void *)CONFIG_SYS_TEXT_BASE )!= (length/512))
		{
			printf("Fail in reading uboot head.\n");
			goto __card_op_fail__;
		}
		/* ���У��� */
	    if( check_sum( (__u32 *)CONFIG_SYS_TEXT_BASE, length ) != 0 )
	    {
	        printf("Fail in checking uboot.\n");
	       	goto __card_op_fail__;
	    }
#else
		uint start_sector = UBOOT_START_SECTOR_IN_SDMMC - (UBOOT_START_SECTOR_PRE_IN_SDMMC - UBOOT_START_SECTOR_IN_SDMMC);

		do
		{
			start_sector += UBOOT_START_SECTOR_PRE_IN_SDMMC - UBOOT_START_SECTOR_IN_SDMMC;

			if(start_sector > UBOOT_START_SECTOR_PRE_IN_SDMMC)
			{
				printf("read all u-boot blk failed\n");

				goto __card_op_fail__;
			}
			/* load 1k uboot head */
		    if( mmc_bread( card_no, start_sector, 1024/512, (void *)CONFIG_SYS_TEXT_BASE ) != (1024/512))
			{
				printf("Fail in reading uboot head.\n");
				continue;
			}
			/* check head */
			if( check_magic( (__u32 *)CONFIG_SYS_TEXT_BASE, UBOOT_MAGIC ) != 0 )
			{
				printf("ERROR! NOT find the head of uboot.\n");
				continue;
			}
			/* check length */
		    length =  bfh->boot_head.length;
			printf("The size of uboot is %x.\n", length );
		    if( ( length & ( 8 * 1024 - 1 ) ) != 0 )
		    {
		    	printf("boot0 length is NOT align.\n");
		    	continue;
		    }
		    if( mmc_bread( card_no, start_sector, length/512, (void *)CONFIG_SYS_TEXT_BASE )!= (length/512))
			{
				printf("Fail in reading uboot head.\n");
				continue;
			}
			/* ���У��� */
		    if( check_sum( (__u32 *)CONFIG_SYS_TEXT_BASE, length ) != 0 )
		    {
		        printf("Fail in checking uboot.\n");
		       	continue;
		    }
		    break;
		}
		while(1);
#endif
		if(i == 0)
		{
			bfh->boot_data.storage_type = 1;
		}
		else
		{
			bfh->boot_data.storage_type = 2;
			set_mmc_para(2,(void *)&BT0_head.prvt_head.storage_data);
		}
		printf("Succeed in loading uboot from sdmmc flash.\n");

		sunxi_mmc_exit( card_no, BT0_head.prvt_head.storage_gpio, 16 );

		return 0;

__card_op_fail__:
		sunxi_mmc_exit(card_no, BT0_head.prvt_head.storage_gpio, 16 );
	}

	return -1;
}

int load_boot1(void)
{
	memcpy((void *)DRAM_PARA_STORE_ADDR, (void *)BT0_head.prvt_head.dram_para, SUNXI_DRAM_PARA_MAX * 4);

	return load_boot1_from_sdmmc((char *)BT0_head.prvt_head.storage_data);
}
