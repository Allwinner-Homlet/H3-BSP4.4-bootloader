/*
************************************************************************************************************************
*                                                         eGON
*                                         the Embedded GO-ON Bootloader System
*
*                             Copyright(C), 2006-2008, SoftWinners Microelectronic Co., Ltd.
*											       All Rights Reserved
*
* File Name : load_Boot1_from_nand.c
*
* Author : Gary.Wang
*
* Version : 1.1.0
*
* Date : 2007.10.14
*
* Description : This file provides a function "load_Boot1_from_nand" to load a good copy of Boot1
*             from outside nand flash chips to SRAM.
*
* Others : None at present.
*
*
* History :
*
*  <Author>        <time>       <version>      <description>
*
* Gary.Wang       2007.10.14      1.1.0        build the file
*
************************************************************************************************************************
*/
#include "common.h"
#include "spare_head.h"
#include "private_toc.h"
#include "asm/arch/nand_boot0.h"

extern int verify_addsum( void *mem_base, __u32 size );
/*******************************************************************************
*��������: load_Boot1_from_nand
*����ԭ�ͣ�int32 load_Boot1_from_nand( void )
*��������: ��һ�ݺõ�Boot1��nand flash�����뵽SRAM�С�
*��ڲ���: void
*�� �� ֵ: 0                         ���벢У��ɹ�
*          -1                        ���벢У��ʧ��
*��    ע:
*******************************************************************************/
int load_toc1_from_nand( void )
{
    __u32 i;
    __s32  status;
    __u32 length;
    __u32 read_blks;
	sbrom_toc1_head_info_t  *toc1_head;

	if(NF_open( ) == NF_ERROR)                         // ��nand flash
	{
		printf("fail in opening nand flash\n");

		return -1;
	}

    for( i = BOOT1_START_BLK_NUM;  i <= BOOT1_LAST_BLK_NUM;  i++ )
    {
    	if( NF_read_status( i ) == NF_BAD_BLOCK )		// �����ǰ���ǻ��飬�������һ��
    	{
    		printf("nand block %d is bad\n", i);
            continue;
			}
        /* ���뵱ǰ����ǰ��512�ֽڵ����ݵ�SRAM�У�Ŀ���ǻ�ȡ�ļ�ͷ */
        if( NF_read( i << ( NF_BLK_SZ_WIDTH - NF_SCT_SZ_WIDTH ), (void *)CONFIG_TOC1_STORE_IN_DRAM_BASE, 1 )  == NF_OVERTIME_ERR )
        {
		    printf("the first data is error\n");
			continue;
		}
		/* �쿴�Ƿ����ļ�ͷ */
		toc1_head = (sbrom_toc1_head_info_t *) CONFIG_TOC1_STORE_IN_DRAM_BASE;
		if(toc1_head->magic != TOC_MAIN_INFO_MAGIC)
		{
			printf("%s err: the toc1 head magic is invalid\n", __func__);
			continue;
		}
        length =  toc1_head->valid_len;
        if( ( length & ( ALIGN_SIZE - 1 ) ) != 0 )     // length������NF_SECTOR_SIZE�����
        {
            printf("the boot1 is not aligned by 0x%x\n", ALIGN_SIZE);
        	continue;
		}
        if( 1==load_uboot_in_one_block_judge(length) )
        {
        	/* ��һ����������Boot1�ı��� */
        	status = load_and_check_in_one_blk( i, (void *)CONFIG_TOC1_STORE_IN_DRAM_BASE, length, NF_BLOCK_SIZE );
        	if( status == ADV_NF_OVERTIME_ERR )            // ��������
        	{
        		continue;
        	}
        	else if( status == ADV_NF_OK )
        	{
                printf("Check is correct.\n");
                NF_close( );                        // �ر�nand flash
                return 0;
            }
        }
        else
        {
        	/* �Ӷ����������һ��Boot1�ı��� */
        	status = load_in_many_blks( i, BOOT1_LAST_BLK_NUM, (void*)CONFIG_TOC1_STORE_IN_DRAM_BASE,
        								length, NF_BLOCK_SIZE, &read_blks );
        	if( status == ADV_NF_LACK_BLKS )        // ��������
        	{
        		printf("ADV_NF_LACK_BLKS\n");
        		NF_close( );                        // �ر�nand flash
        		return -1;
        	}
        	else if( status == ADV_NF_OVERTIME_ERR )
        	{
        		printf("mult block ADV_NF_OVERTIME_ERR\n");
        		continue;
			}
            if( verify_addsum( (__u32 *)CONFIG_TOC1_STORE_IN_DRAM_BASE, length ) == 0 )
            {
                printf("The file stored in start block %u is perfect.\n", i );
                NF_close( );                        // �ر�nand flash
                return 0;
            }
        }
    }


	printf("Can't find a good Boot1 copy in nand.\n");
    NF_close( );                        // �ر�nand flash
    printf("Ready to quit \"load_Boot1_from_nand\".\n");
    return -1;
}

