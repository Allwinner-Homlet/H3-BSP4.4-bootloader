/*
************************************************************************************************************************
*                                          Boot rom
*                                         Seucre Boot
*
*                             Copyright(C), 2006-2013, AllWinners Microelectronic Co., Ltd.
*											       All Rights Reserved
*
* File Name   : Base.h
*
* Author      : glhuang
*
* Version     : 0.0.1
*
* Date        : 2013.09.05
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
* glhuang       2013.09.05       0.0.1        build the file
*
************************************************************************************************************************
*/
#include "common.h"
#include "sbrom_toc.h"
#include "boot_type.h"
#include "openssl_ext.h"
#include "asm/arch/clock.h"
#include "asm/arch/ss.h"
#include "asm/arch/timer.h"
#include "asm/arch/uart.h"
#include "asm/arch/rtc_region.h"
#include "asm/arch/mmu.h"
#include "asm/arch/gic.h"
#include "private_toc.h"
#include "sbrom_toc.h"
#include "../libs/sbrom_libs.h"
#include <asm/arch/dram.h>
#include <private_toc.h>
#ifdef CONFIG_BOOT_A15
#include <asm/arch/cpu_switch.h>
#endif

extern void sid_read_rotpk(void *dst);
extern void sunxi_certif_mem_reset(void);
extern int sunxi_certif_probe_pubkey(X509 *x, sunxi_key_t *pubkey);
extern void ndump(u8 *buf, int count);

static void print_commit_log(void);
static int sbromsw_toc1_traverse(void);
//static int sbromsw_probe_fel_flag(void);
static int sbromsw_clear_env(void);
static int sunxi_root_certif_pk_verify(sunxi_certif_info_t *sunxi_certif, u8 *buf, u32 len);
static int load_toc1_handler(int boot_type, int start_sector);
static int sboot_check_uart_input(void);

extern __s32 boot_set_gpio(void  *user_gpio_list, __u32 group_count_max, __s32 set_gpio);

#ifdef SUNXI_OTA_TEST
static int sbromsw_print_ota_test(void);
#endif
sbrom_toc0_config_t *toc0_config = (sbrom_toc0_config_t *)CONFIG_TOC0_CONFIG_ADDR;
extern char sbromsw_hash_value[64];

void sbromsw_entry(void)
{
	toc0_private_head_t *toc0 = (toc0_private_head_t *)CONFIG_SBROMSW_BASE;
	uint dram_size;
	int  ret, flag;
	int boot_type;

	timer_init();
#ifndef CONFIG_ARCH_SUN8IW6P1
	set_pll();
#else
	set_pll_for_secure();
#endif
	sunxi_serial_init(toc0_config->uart_port, toc0_config->uart_ctrl, 2);
        set_debugmode_flag();
        print_commit_log();
	if( toc0_config->enable_jtag )
	{
		boot_set_gpio((normal_gpio_cfg *)toc0_config->jtag_gpio, 5, 1);
	}
	sboot_check_uart_input();
	printf("try to probe rtc region\n");
#ifdef SUNXI_OTA_TEST
	sbromsw_print_ota_test();
#endif
	flag = rtc_region_probe_fel_flag();
	printf("flag=0x%x\n", flag);
	if(flag == SUNXI_RUN_EFEX_FLAG)
	{
		printf("sbromsw_entry sbromsw_probe_fel_flag\n");
		rtc_region_clear_fel_flag();

		goto __sbromsw_entry_err0;
	}
#ifdef CONFIG_BOOT_A15
//	printf("toc0_config->boot_cpu=0x%x\n", toc0_config->boot_cpu);
//	if(toc0_config->boot_cpu)
//	{
//		flag = BOOT_A7_FLAG;
//	}
//	else
//	{
//		flag = BOOT_A15_FLAG;
//	}
/*
    boot_cpu  ����

	bit0~7                       bit8~15

	0:����Ҫ�����־λ           1:��ǰӦ���л�a15����
	1:֪ͨu-boot����             0:��ǰӦ���л�a7����

	ÿ�δ�brom��ȡ��boot_cpuֻ����0x100����0
*/

	if(flag == BOOT_A15_FLAG)
	{
		rtc_region_clear_fel_flag();
		if(toc0_config->boot_cpu == 0x00)
			toc0_config->boot_cpu = 0x101;   //a15��������Ҫ�����־λ

		switch_to_a15(toc0_config->a15_power_gpio);
	}
	else if(flag == BOOT_A7_FLAG)
	{
		rtc_region_clear_fel_flag();
		if(toc0_config->boot_cpu == 0x100)      //���ԭ����a15����
			toc0_config->boot_cpu = 0x01;       //a7��������Ҫ�����־λ
	}
	else
	{
		if(toc0_config->boot_cpu == 0x100)
		{
			switch_to_a15(toc0_config->a15_power_gpio);                //a15����������Ҫ�����־λ
		}
		else
		{
			toc0_config->boot_cpu = 0x0;    //a7����������Ҫ�����־λ
		}
	}
    //printf("toc0_config->boot_cpu=0x%x\n", toc0_config->boot_cpu);
#endif
	printf("try to setup mmu\n");
	//mmu init
	mmu_setup();
	printf("mmu setup ok\n");
	//dram init
	printf("try to init dram\n");
	dram_size = init_DRAM(0, (void *)toc0_config->dram_para);
	if (dram_size)
	{
		printf("init dram ok, size=%dM\n", dram_size);
	}
	else
	{
		printf("init dram fail\n");

		goto __sbromsw_entry_err;
	}
	printf("mmu resetup\n");
#ifndef CONFIG_OPTEE_SUPPORT
	mmu_resetup(dram_size, toc0_config->secure_dram_mbytes);
#endif
	printf("init heap\n");
	create_heap(CONFIG_HEAP_BASE, CONFIG_HEAP_SIZE);
	if(!toc0_config->secure_without_OS)
        {
	    printf("init gic\n");
            gic_init();
        }

	boot_type = toc0->platform[0] & 0x0f;
	printf("ss open\n");
	sunxi_ss_open();
	ret = load_toc1_handler(boot_type,UBOOT_START_SECTOR_IN_SDMMC);
#ifdef CONFIG_TOC1_BACKUP_MODE
	if(( ret !=0) &&
	   (boot_type == BOOT_FROM_SD0 ||boot_type == BOOT_FROM_SD2 ))//only sdmmc support backup
	{
		ret = load_toc1_handler(boot_type,UBOOT_START_SECTOR_BACKUP_IN_SDMMC);
	}
#endif
	printf("load status:%d\n",ret);

__sbromsw_entry_err:
#ifdef CONFIG_BOOT_A15
	if(!(toc0_config->boot_cpu & 0xff00))
	{
		sbromsw_clear_env();

		boot0_jump(SUNXI_FEL_ADDR_IN_SECURE);
	}
	else
	{
		rtc_region_set_flag(SUNXI_RUN_EFEX_FLAG);
		sbromsw_clear_env();

		watchdog_enable();
	}
#endif
__sbromsw_entry_err0:
	sbromsw_clear_env();

	boot0_jump(SUNXI_FEL_ADDR_IN_SECURE);
}

static int load_toc1_handler(int boot_type, int start_sector)
{
	int ret;
	printf("init flash\n");
	ret = sunxi_flash_init(boot_type,start_sector);		//��ʼ���ⲿ���ʣ�׼����ȡTOC1����
	if(ret)
	{
		printf("sbromsw_entry sunxi_flash_init failed\n");
		return -1;
	}

	ret = toc1_init();	//TOC1��ʼ�����ж�TOC1��ͷ���Ƿ�ϸ�
	if(ret)
	{
		printf("sbromsw_entry toc1_init failed\n");
		return -1;
	}
	ret = sbromsw_toc1_traverse();
	if(ret)
	{
		printf("sbromsw_entry sbromsw_toc1_traverse failed\n");
		return -1;
	}
	return 0;
}

static void print_commit_log(void)
{
        printf("sbrom commit : %s \n",sbromsw_hash_value);
        return ;
}

#define  SUNXI_X509_CERTIFF_MAX_LEN   (4096)

static int sbromsw_toc1_traverse(void)
{
	sbrom_toc1_item_group item_group;
	int ret;
	uint len, i;
	u8 buffer[SUNXI_X509_CERTIFF_MAX_LEN];

	sunxi_certif_info_t  root_certif;
	sunxi_certif_info_t  sub_certif;
	u8  hash_of_file[256];
	//u8  hash_in_certif[256];

	//u8  key_certif_extension[260];
	//u8  content_certif_key[520];
	int out_to_ns;
    int ready_out_to_ns = 0;
#ifdef CONFIG_OPTEE_SUPPORT
    uint optee_entry=0, uboot_entry=0;
#endif
	toc1_item_traverse();

	printf("probe root certif\n");

	memset(buffer, 0, SUNXI_X509_CERTIFF_MAX_LEN);
	len = toc1_item_read_rootcertif(buffer, SUNXI_X509_CERTIFF_MAX_LEN);
	if(!len)
	{
		printf("%s error: cant read rootkey certif\n", __func__);

		return -1;
	}

	if(sunxi_root_certif_pk_verify(&root_certif, buffer, len))
	{
		printf("certif invalid: root certif verify itself failed\n");

		return -1;
	}

	if(sunxi_certif_verify_itself(&root_certif, buffer, len))
	{
		printf("certif invalid: root certif verify itself failed\n");

		return -1;
	}
	do
	{
		memset(&item_group, 0, sizeof(sbrom_toc1_item_group));
		ret = toc1_item_probe_next(&item_group);
		if(ret < 0)
		{
			printf("sbromsw_toc1_traverse err in toc1_item_probe_next\n");

			return -1;
		}
		else if(ret == 0)
		{
			printf("sbromsw_toc1_traverse find out all items\n");
#ifdef CONFIG_OPTEE_SUPPORT
			printf("optee entry=0x%x\n", optee_entry);
			printf("uboot entry=0x%x\n", uboot_entry);

			if(optee_entry && uboot_entry)
				go_exec(optee_entry, uboot_entry, SECURE_SWITCH_NORMAL);
#endif
			return 0;
		}
		if(item_group.bin_certif)
		{
			memset(buffer, 0, SUNXI_X509_CERTIFF_MAX_LEN);
			len = toc1_item_read(item_group.bin_certif, buffer, SUNXI_X509_CERTIFF_MAX_LEN);
			if(!len)
			{
				printf("%s error: cant read content key certif\n", __func__);

				return -1;
			}
			//֤�����ݽ�����У�飬ȷ��û�б��滻
			if(sunxi_certif_verify_itself(&sub_certif, buffer, len))
			{
				printf("%s error: cant verify the content certif\n", __func__);

				return -1;
			}
//			printf("key n:\n");
//			ndump(sub_certif.pubkey.n, sub_certif.pubkey.n_len);
//			printf("key e:\n");
//			ndump(sub_certif.pubkey.e, sub_certif.pubkey.e_len);
			//ÿ������һ����Կ֤�飬���ڸ�֤����Ѱ��ƥ����Ŀ���Ҳ�������Ϊ�д���
			for(i=0;i<root_certif.extension.extension_num;i++)
			{
				if(!strcmp((const char *)root_certif.extension.name[i], item_group.bin_certif->name))
				{
					printf("find %s key stored in root certif\n", item_group.bin_certif->name);

					if(memcmp(root_certif.extension.value[i], sub_certif.pubkey.n+1, sub_certif.pubkey.n_len-1))
					{
						printf("%s key n is incompatible\n", item_group.bin_certif->name);
						printf(">>>>>>>key in rootcertif<<<<<<<<<<\n");
						ndump((u8 *)root_certif.extension.value[i], sub_certif.pubkey.n_len-1);
						printf(">>>>>>>key in certif<<<<<<<<<<\n");
						ndump((u8 *)sub_certif.pubkey.n+1, sub_certif.pubkey.n_len-1);

						return -1;
					}
					if(memcmp(root_certif.extension.value[i] + sub_certif.pubkey.n_len-1, sub_certif.pubkey.e, sub_certif.pubkey.e_len))
					{
						printf("%s key e is incompatible\n", item_group.bin_certif->name);
						printf(">>>>>>>key in rootcertif<<<<<<<<<<\n");
						ndump((u8 *)root_certif.extension.value[i] + sub_certif.pubkey.n_len-1, sub_certif.pubkey.e_len);
						printf(">>>>>>>key in certif<<<<<<<<<<\n");
						ndump((u8 *)sub_certif.pubkey.e, sub_certif.pubkey.e_len);

						return -1;
					}
					break;
				}
			}
			if(i==root_certif.extension.extension_num)
			{
				printf("cant find %s key stored in root certif", item_group.bin_certif->name);

				return -1;
			}
		}

		if(item_group.binfile)
		{
			//����bin�ļ����ݵ��ڴ�
			len = sunxi_flash_read(item_group.binfile->data_offset/512, (item_group.binfile->data_len+511)/512, (void *)item_group.binfile->run_addr);
			//len = sunxi_flash_read(item_group.binfile->data_offset/512, (item_group.binfile->data_len+511)/512, (void *)0x2a000000);
			if(!len)
			{
				printf("%s error: cant read bin file\n", __func__);

				return -1;
			}
			//�����ļ�hash
			memset(hash_of_file, 0, sizeof(hash_of_file));
			ret = sunxi_sha_calc(hash_of_file, sizeof(hash_of_file), (u8 *)item_group.binfile->run_addr, item_group.binfile->data_len);
			//ret = sunxi_sha_calc(hash_of_file, sizeof(hash_of_file), (u8 *)0x2a000000, item_group.binfile->data_len);
			if(ret)
			{
				printf("sunxi_sha_calc: calc sha256 with hardware err\n");

				return -1;
			}
			//ʹ������֤�����չ����ļ�hash���бȽ�
			//��ʼ�Ƚ��ļ�hash(С���˽׶μ���õ�)��֤��hash(PC�˼���õ�)
			if(memcmp(hash_of_file, sub_certif.extension.value[0], 32))
			{
				printf("hash compare is not correct\n");
				printf(">>>>>>>hash of file<<<<<<<<<<\n");
				ndump((u8 *)hash_of_file, 32);
				printf(">>>>>>>hash in certif<<<<<<<<<<\n");
				ndump((u8 *)sub_certif.extension.value[0], 32);

				return -1;
			}

			printf("ready to run %s\n", item_group.binfile->name);
            if(!toc0_config->secure_without_OS)
            {
                ready_out_to_ns = 1;
            }
            else
            {
                printf("secure_without_OS mode  \n");
                ready_out_to_ns = 0;
            }
#ifdef CONFIG_OPTEE_SUPPORT
            if(!strcmp(item_group.binfile->name, "optee"))
            {
            	optee_entry = item_group.binfile->run_addr;
            }
			else if(strcmp(item_group.binfile->name, "u-boot"))
			{
				out_to_ns = SECURE_SWITCH_OTHER;
			}
			else
			{
                if(!ready_out_to_ns)
					out_to_ns = SECURE_NON_SECUREOS;
                else
                    out_to_ns = SECURE_SWITCH_NORMAL;
                uboot_entry = item_group.binfile->run_addr;
			}
#else
			if(strcmp(item_group.binfile->name, "u-boot"))
			{
                out_to_ns = SECURE_SWITCH_OTHER;
			}
			else
			{
                if(!ready_out_to_ns)
					out_to_ns = SECURE_NON_SECUREOS;
                else
                    out_to_ns = SECURE_SWITCH_NORMAL;
			}
			toc0_config->next_exe_pa   = va2pa(item_group.binfile->run_addr);
			go_exec(item_group.binfile->run_addr, CONFIG_TOC0_CONFIG_ADDR, out_to_ns);
#endif

		}
	}
	while(1);

	return 0;
}

//static int sbromsw_probe_fel_flag(void)
//{
//	uint flag;
//
//	flag = rtc_region_probe_fel_flag();
//	rtc_region_clear_fel_flag();
//
//	return flag;
//}

#ifdef SUNXI_OTA_TEST
static int sbromsw_print_ota_test(void)
{
	printf("*********************************************\n");
	printf("*********************************************\n");
	printf("*********************************************\n");
	printf("*********************************************\n");
	printf("********[OTA TEST]:update toc0 sucess********\n");
	printf("*********************************************\n");
	printf("*********************************************\n");
	printf("*********************************************\n");
	printf("*********************************************\n");
	return 0;
}
#endif

static int sbromsw_clear_env(void)
{
#if defined(CONFIG_ARCH_SUN9IW1P1)
	mctl_rst_securout();
#endif
	gic_exit();
	reset_pll();
	mmu_turn_off();

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
#define RSA_BIT_WITDH 2048
static int sunxi_certif_pubkey_check( sunxi_key_t  *pubkey )
{
	char efuse_hash[256] , rotpk_hash[256];
	char all_zero[32];

	char pk[RSA_BIT_WITDH/8 * 2 + 256]; /*For the stupid sha padding */

	sid_read_rotpk(efuse_hash);
	memset(all_zero, 0, 32);
	if( ! memcmp(all_zero, efuse_hash,32 ) )
		return 0 ; /*Don't check if rotpk efuse is empty*/
	else{
		memset(pk, 0x91, sizeof(pk));
		char *align = (char *)(((u32)pk+31)&(~31));
		if( *(pubkey->n) ){
			memcpy(align, pubkey->n, pubkey->n_len);
			memcpy(align+pubkey->n_len, pubkey->e, pubkey->e_len);
		}else{
			memcpy(align, pubkey->n+1, pubkey->n_len-1);
			memcpy(align+pubkey->n_len-1, pubkey->e, pubkey->e_len);
		}

		if(sunxi_sha_calc( (u8 *)rotpk_hash, 32, (u8 *)align, RSA_BIT_WITDH/8*2 ))
		{
			printf("sunxi_sha_calc: calc  pubkey sha256 with hardware err\n");
			return -1;
		}

		if(memcmp(rotpk_hash, efuse_hash, 32)){
			printf("certif pk dump\n");
			ndump((u8 *)align , RSA_BIT_WITDH/8*2 );

			printf("calc certif pk hash dump\n");
			ndump((u8 *)rotpk_hash,32);

			printf("efuse pk dump\n");
			ndump((u8 *)efuse_hash,32);

			printf("sunxi_certif_pubkey_check: pubkey hash check err\n");
			return -1;
		}
		return 0 ;
	}

}
/*
************************************************************************************************************
*
*                                             function
*
*    name          :
*
*    parmeters     :  buf: ֤������ʼ   len�����ݳ���
*
*    return        :
*
*    note          :  ֤����У��
*
*
************************************************************************************************************
*/
static int sunxi_root_certif_pk_verify(sunxi_certif_info_t *sunxi_certif, u8 *buf, u32 len)
{
	X509 *certif;
	int  ret;

	//�ڴ��ʼ��
	sunxi_certif_mem_reset();
	//����֤��
	ret = sunxi_certif_create(&certif, buf, len);
	if(ret < 0)
	{
		printf("fail to create a certif\n");

		return -1;
	}
	//��ȡ֤�鹫Կ
	ret = sunxi_certif_probe_pubkey(certif, &sunxi_certif->pubkey);
	if(ret)
	{
		printf("fail to probe the public key\n");

		return -1;
	}
	ret = sunxi_certif_pubkey_check(&sunxi_certif->pubkey);
	if(ret){
		printf("fail to check the public key hash against efuse\n");

		return -1;
	}

	sunxi_certif_free(certif);

	return 0;
}
int sboot_check_uart_input(void)
{
	int c = 0;
	int i = 0;
	for(i = 0;i < 3;i++)
	{
		__msdelay(10);
		if(sunxi_serial_tstc())
		{
			printf("key press :");
			c = sunxi_serial_getc();
			printf("0x%x   \n",c-0x30); // ASCII to decimal digit
			break;
		}
	}

	if(c == '2')
	{
		printf("enter 0x%x,ready jump to fes\n", c-0x30);
		sbromsw_clear_env();
		boot0_jump(SUNXI_FEL_ADDR_IN_SECURE);
	}
	return 0;
}
