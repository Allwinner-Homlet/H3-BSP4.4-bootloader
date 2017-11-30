#include <stdio.h>
#include <dlfcn.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "fwmdf_if.h"

#include <fcntl.h>
#include <unistd.h>
#include<ctype.h>
#include<sys/stat.h>
#if defined(__APPLE__) || defined(__FreeBSD__)
#include <copyfile.h>
#else
#include <sys/sendfile.h>
#endif

#define BUF_LEN	(32 * 1024)
#define SIZE_100M 104857600
#define SIZE_20M  (1024*1024*20)
#define VERSION	 "v1.0.0"
#define EXEC_EXPORT			"export"
#define EXEC_PACK			"pack"

void* handle_fwmdf  = NULL;
IMGMDF_IF * _iFWmdf = NULL;

//option
char szIn[1024] = {0};			//-in
char szOut[1024] = {0};			//-out
char szExec[1024] = {0};		//-exec
char szVer[1024] = {0};			//version
char szExecPath[1024] = {0};	//fwpacker path

typedef struct tag_STItem
{
	char	Name[128];
	char 	MainCode[32];
	char	SubCode[32];
	int		verify;
	char	VMainCode[32];
	char	VSubCode[32];
	int		need;
	char	Path[512];
} STItem;


STItem arPartitionMap[] =
{
    {"boot0_nd"	    , "BOOT    " , "BOOT0_0000000000", 0, "", "", 0, ""},
    {"boot0_sd"     , "12345678" , "1234567890BOOT_0", 0, "", "", 0, ""},
    {"toc0"         , "12345678" , "TOC0_00000000000", 0, "", "", 0, ""},
    {"toc1"	        , "12345678" , "TOC1_00000000000", 0, "", "", 0, ""},
    {"uboot"        , "12345678" , "UBOOT_0000000000", 0, "", "", 0, ""},
    {"boot_pkg"     , "12345678" , "BOOTPKG-00000000", 0, "", "", 0, ""},
    {"sys_config"   , "COMMON  " , "SYS_CONFIG100000", 0, "", "", 0, ""},
    {"config"       , "COMMON  " , "SYS_CONFIG_BIN00", 0, "", "", 0, ""},
    {"board"        , "COMMON  " , "BOARD_CONFIG_BIN", 0, "", "", 0, ""},
    {"sys_partition", "COMMON  " , "SYS_CONFIG000000", 0, "", "", 0, ""},
    {"sunxi"        , "COMMON  " , "DTB_CONFIG000000", 0, "", "", 0, ""},
    {"boot"	        , "RFSFAT16" , "BOOT_FEX00000000", 1, "RFSFAT16", "VBOOT_FEX0000000", 0, ""},
    {"recovery"	    , "RFSFAT16" , "RECOVERY_FEX0000", 1, "RFSFAT16", "VRECOVERY_FEX000", 0, ""},
    {"system"       , "RFSFAT16" , "SYSTEM_FEX000000", 1, "RFSFAT16", "VSYSTEM_FEX00000", 0, ""},
    {"systembak"    , "RFSFAT16" , "SYSTEMBAK_FEX000", 1, "RFSFAT16", "VSYSTEMBAK_FEX00", 0, ""}
};
int Usage()
{
	printf("\tUsage: %s -in src_file_path -out dest_file_path\n", "fwpacker");
	printf("\tOption:\n");
	printf("\t\t-%-16s \t%s | %s\n", "exec", EXEC_EXPORT, EXEC_PACK);
	for (int i = 0; i < sizeof (arPartitionMap) / sizeof (STItem); i++)
	{
		printf("\t\t-%-16s \t(depend on -exec)%s_file_path\n", arPartitionMap[i].Name, arPartitionMap[i].Name);
	}
	//printf("-code MAINCODE,SUBCODE\n");
	return 0x8001;
}

STItem* GetItem(const char* szItemName)
 {
	for (int i = 0; i < sizeof (arPartitionMap) / sizeof (STItem); i++)
	{
		if ( strcasecmp(arPartitionMap[i].Name, szItemName) == 0 )
		{
			return arPartitionMap + i;
		}
	}
	return NULL;
 }

 void SetUsing(STItem* pItem, const char* szPath, int need)
 {
	strcpy(pItem->Path, szPath);
	pItem->need = need;
 }

 int GetOption(int argc, char* argv[])
 {
	if (argc == 2 && (strcasecmp(argv[1], "-version") == 0))
	{
		printf("version:%s\n", VERSION);
		return 0x8002;
	}
	if (argc < 5)
	{
		return Usage();
	}

	for (int i = 1; i < argc; i++)
	{
		if (strcasecmp(argv[i], "-in") == 0)				strcpy(szIn, argv[++i]);
		else if (strcasecmp(argv[i], "-out") == 0)			strcpy(szOut, argv[++i]);
		else if (strcasecmp(argv[i], "-exec") == 0)			strcpy(szExec, argv[++i]);
		else if (strcasecmp(argv[i], "-toc0") == 0)			SetUsing(GetItem("toc0"), 		  argv[++i], 1);
		else if (strcasecmp(argv[i], "-toc1") == 0)			SetUsing(GetItem("toc1"), 		  argv[++i], 1);
		else if (strcasecmp(argv[i], "-boot0_sd") == 0)		SetUsing(GetItem("boot0_sd"), 	  argv[++i], 1);
		else if (strcasecmp(argv[i], "-boot0_nd") == 0)		SetUsing(GetItem("boot0_nd"), 	  argv[++i], 1);
		else if (strcasecmp(argv[i], "-uboot") == 0)		SetUsing(GetItem("uboot"), 		  argv[++i], 1);
		else if (strcasecmp(argv[i], "-boot_pkg") == 0)		SetUsing(GetItem("boot_pkg"), 	  argv[++i], 1);
		else if (strcasecmp(argv[i], "-sys_config") == 0)	SetUsing(GetItem("sys_config"),   argv[++i], 1);
		else if (strcasecmp(argv[i], "-config") == 0)		SetUsing(GetItem("config"),	      argv[++i], 1);
		else if (strcasecmp(argv[i], "-board") == 0)		SetUsing(GetItem("board"),        argv[++i], 1);
		else if (strcasecmp(argv[i], "-sys_partition") == 0)SetUsing(GetItem("sys_partition"),argv[++i], 1);
		else if (strcasecmp(argv[i], "-sunxi") == 0)		SetUsing(GetItem("sunxi"), 	      argv[++i], 1);
		else if (strcasecmp(argv[i], "-boot") == 0)			SetUsing(GetItem("boot"), 		  argv[++i], 1);
		else if (strcasecmp(argv[i], "-system") == 0)		SetUsing(GetItem("system"), 	  argv[++i], 1);
		else if (strcasecmp(argv[i], "-systembak") == 0)        SetUsing(GetItem("systembak"), 	  argv[++i], 1);
		else if (strcasecmp(argv[i], "-recovery") == 0)		SetUsing(GetItem("recovery"), 	  argv[++i], 1);
	}

	if (strlen(szExec) == 0)
	{
		printf("-exec is necessary.\n");
		return Usage();
	}

	if (strcasecmp(szExec, EXEC_EXPORT) != 0 && strcasecmp(szExec, EXEC_PACK) != 0)
	{
		printf("-exec %s not support!\n", szExec);
		return Usage();
	}

	if (strlen(szExec) == 0)
	{
		printf("-exec is necessary.\n");
		return Usage();
	}

	if (strlen(szIn) == 0)
	{
		printf("-in is necessary.\n");
		return Usage();
	}

	if (strcasecmp(szExec, EXEC_PACK) == 0 && strlen(szOut) == 0)
	{
		printf("-out is necessary when -exec pack\n");
		return Usage();
	}
	return 0;
 }
 LRESULT FWMdfCallback(WPARAM wParam, LPARAM lParam, LPCTSTR lpInfo)
 {
	return 0;
 }

 char* gettimestr()
{
	static char ss[200]  ;
	time_t timep;
	struct tm *p;
	time(&timep);
	p=localtime(&timep);
	sprintf(ss, "%04d%02d%02d%02d%02d%02d", (1900+p->tm_year),(1+p->tm_mon), p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec);
	return ss;
}

 u32 AddSum(char * buffer, u32 len, u32 org)
{
	u32 u32len 	= 0;
	u32 sum 	= org;
	u32 * pu32 	= (u32 *) buffer;
	u32 last 	= 0;
	u32 curlen 	= 0;

	//首先处理4字节对齐的数据
	u32len = len >> 2;
	for (u32 i = 0; i < u32len; i++)
	{
		sum += pu32[i];

		//printf("sum=%X %X\n", sum, pu32[i]);
	}

	curlen = u32len << 2;
	//如果有1 2 3字节的尾巴，则处理尾巴数据，按照lsb的格式
	//if ((len % 4) != 0)
	if ((len & 0x3) != 0)
	{
		memcpy(&last, buffer + curlen, len - curlen);
		sum += last;	//加上尾巴补全的u32数据
		//printf("sum=%X last=%X\n", sum, last);
	}

	return sum;
}



u32 new_AddSum(char * buffer, u32 len, u32 org)
{
	u32 u32len 	= 0;
	u32 sum 	= org;
	u32 * pu32 	= (u32 *) buffer;
	u32 last 	= 0;
	u32 curlen 	= 0;

	//???ȴ???4?ֽڶ?????????
	u32len = len >> 2;
	for (u32 i = 0; i < u32len; i++)
	{
		sum += pu32[i];

		//printf("sum=%X %X\n", sum, pu32[i]);
	}

	curlen = u32len << 2;
	//??????1 2 3?ֽڵ?β?ͣ???????β?????ݣ?????lsb?ĸ?ʽ
	//if ((len % 4) != 0)
	if ((len & 0x3) != 0)
	{
		memcpy(&last, buffer + curlen, len - curlen);
		sum += last;	//????β?Ͳ?ȫ??u32????
		//printf("sum=%X last=%X\n", sum, last);
	}

	return sum;
}

u32 min(u32 a, u32 b)
{
	if(a > b)
		return b;
	else
		return a;
}

int AddSumWithFile(const char* sfp, const char* dfp)
{
	u32 buflen = (32*1024);
	u32 addsum = 0;
	FILE* fp = fopen(sfp, "rb");
	if (!fp)
	{
		printf("Open src file[%s] failed!", sfp);
		return -1;
	}
	fseek(fp, 0, SEEK_END);
	u32 FileLength = ftell(fp);
	rewind(fp);


	char * buffer = (char *)malloc(buflen);
	if( buffer == NULL)
	{
		printf("malloc buffer fail!\n");
		return -1;
	}

	int read_total = 0;
	while(read_total < FileLength)
	{
		u32 dwRead = 0;
		u32 read = min(buflen, (FileLength - read_total));
		memset(buffer, 0, buflen);
		dwRead = fread(buffer, 1, read, fp );
		if( dwRead < 0)
		{
			printf("read file fail!\n");
			return -1;
		}
		read_total += dwRead;
		addsum = new_AddSum(buffer, dwRead, addsum);
	//	printf("dwRead = %ld\n", dwRead);
	//	printf("addsum = %ld\n", addsum);
	}

	fclose(fp);
/*
	char buff[BUF_LEN] = {0};
	int nRead = fread(buff, BUF_LEN, 1, fp);
	while (nRead)
	{
		crc = AddSum(buff, nRead, crc);
		nRead = fread(buff, BUF_LEN, 1, fp);
	}

*/

	fp = fopen(dfp, "wb+");
	if (!fp)
	{
		printf("Open dest file [%s] failed, ", dfp);
		return -2;
	}

	fwrite(&addsum, 4, 1, fp);
	//printf("addsum = %p\n\n",addsum);
	fclose(fp);
	return 0;
}




 int SaveItemsWithFile(HIMAGE hImage)
 {

	for (int i = 0; i < sizeof (arPartitionMap) / sizeof (STItem); i++)
	{
		if (arPartitionMap[i].need == 1)
		{
			//find item
			int nIndex = _iFWmdf->GetItemIndex(hImage, arPartitionMap[i].MainCode, arPartitionMap[i].SubCode);
			if (nIndex == -1)
			{
				printf("Not such Item with name [%s] in firmware file.\n", arPartitionMap[i].Name);
				return -1;
			}
			printf("Find item=%s\n", arPartitionMap[i].Name);
			//set item
			if ( 0 != _iFWmdf->SetImgItemWithFile(hImage, nIndex, arPartitionMap[i].Path))
			{
				printf("Set item with file [%s] failed!", arPartitionMap[i].Path);
				return -2;
			}

			//set verify
			if (arPartitionMap[i].verify)
			{
				char szTmpFile[1024] = {0};
				sprintf(szTmpFile, "./checksum_%s", gettimestr());
				if (0 != AddSumWithFile(arPartitionMap[i].Path, szTmpFile))
				{
					printf("Create verify file for [%s] Error!\n", arPartitionMap[i].Path);
					return -3;
				}
				int new_file = 0;
				if ((new_file = open(arPartitionMap[i].Path, O_RDWR | O_CREAT)) == -1)
				{
					printf("create file fail\n\n");
					return -1;
				}

				nIndex = _iFWmdf->GetItemIndex(hImage, arPartitionMap[i].VMainCode, arPartitionMap[i].VSubCode);
				if (nIndex == -1)
				{
					printf("Not Verify Item with name [%s] in fireware file.\n", arPartitionMap[i].Name);
					return -4;
				}

				if ( 0 != _iFWmdf->SetImgItemWithFile(hImage, nIndex, szTmpFile))
				{
					printf("Set verify item with file [%s] failed!", szTmpFile);
					return -5;
				}
                                remove(szTmpFile);
			}


		}
	}
	return 0;
 }


 int CopyFile(const char* source, const char* destination, BOOL unuse)
 {
	FILE* fps = fopen(source, "rb");
	if (!fps)
	{
		printf("failed to open source file [%s]\n", source);
		return -1;
	}
	FILE* fpd = fopen(destination, "wb+");
	if (!fpd)
	{
		printf("failed to open destination file [%s]\n", destination);
		fclose(fps);
		return -2;
	}

	char* szBuff = new char[SIZE_20M];
	int nRead = fread(szBuff, 1, SIZE_20M, fps);
	while (nRead)
	{

		fwrite(szBuff, 1, nRead, fpd);
		nRead = fread(szBuff, 1, SIZE_20M, fps);
	}
	delete []szBuff;
	fflush(fpd);
	fclose(fpd);
	fclose(fps);
	return 0;
 }
 int ExportItemsToFile(HIMAGE hImage)
 {

	for (int i = 0; i < sizeof (arPartitionMap) / sizeof (STItem); i++)
	{
		if (arPartitionMap[i].need == 1)
		{
			//find item
			int nIndex = _iFWmdf->GetItemIndex(hImage, arPartitionMap[i].MainCode, arPartitionMap[i].SubCode);
			printf("%s %s\n\n",arPartitionMap[i].MainCode,arPartitionMap[i].SubCode);
			if (nIndex == -1)
			{
				printf("Not such Item with name [%s] in firmware file.\n", arPartitionMap[i].Name);
				return -1;
			}
			printf("Find item=%s\n", arPartitionMap[i].Name);
			__int64 nSize = _iFWmdf->GetItemSize(hImage, nIndex);
			if (nSize > SIZE_100M)
			{
				//the buff content tmp file path
				char szTmpFilePath[1024] = {0};
				_iFWmdf->GetImgItem(hImage, nIndex, (unsigned char*)szTmpFilePath, 1024);
				CopyFile(szTmpFilePath, arPartitionMap[i].Path, 0);
			}
			else
			{
				unsigned char* szBuff = new unsigned char [nSize];
				_iFWmdf->GetImgItem(hImage, nIndex, szBuff, nSize);
				FILE* fp = fopen(arPartitionMap[i].Path, "wb+");
				if (!fp)
				{
					printf("failed to create file with path [%s]\n", arPartitionMap[i].Path);
					return -9;
				}
				fwrite(szBuff, 1, nSize, fp);
				fflush(fp);
				fclose(fp);
			}
		}
	}
	return 0;
 }


 int LoadImageLibrary(const char* szRoot)
 {

	char szPath[1024] = {0};
	strcpy(szPath, szRoot);
	char* p = strrchr(szPath, '/');
	if (p)
	{
		strcpy(p + 1, "FWmdf.so");
	}
	else
	{
		strcpy(szPath, "./FWmdf.so");
	}
	handle_fwmdf = dlopen(szPath, RTLD_LAZY);
	if (!handle_fwmdf)
	{
		printf("Load FWmdf.so failed, err info= %s\n", dlerror());
		return -1;
	}

	//load function
	void* (*pfnGetInterface)(const char*);
	pfnGetInterface = (void* (*)(const char*))dlsym(handle_fwmdf, "GetInterface");
	char* errorInfo = dlerror();
	if (errorInfo != NULL)
	{
		printf("Load function failed. err info=%s\n", errorInfo);
		return -1;
	}

	_iFWmdf = (IMGMDF_IF*)pfnGetInterface(NULL);
	if (!_iFWmdf)
	{
		printf("Get FW interface failed.");
		return -2;
	}

	return 0;
 }




int main(int argc, char* argv[])
{
	if (0 != LoadImageLibrary((const char*)argv[0]))
	{
		printf("Load Image Library Failed!\n");
		return -1;
	}

	if (0 != GetOption(argc, argv))
	{
		return -2;
	}

	_iFWmdf->SetCallback(FWMdfCallback);
	HIMAGE hImage = _iFWmdf->LoadImage(szIn);
	if (!hImage)
	{
		printf("Load firmware file failed, Error format!\n");
		return -3;
	}

	if (strcasecmp(szExec, EXEC_PACK) == 0)
	{
		if (0 != SaveItemsWithFile(hImage))
		{
			printf("Save Item Failed!\n");
			return -4;
		}

		if (0 != _iFWmdf->SaveImg(hImage, szOut))
		{
			printf("Save firmware to path [%s] failed!", szOut);
			return -5;
		}

	}
	else if (strcasecmp(szExec, EXEC_EXPORT) == 0)
	{
		if (0 != ExportItemsToFile(hImage))
		{
			printf("Export Item Failed!\n");
			return -6;
		}
	}

	_iFWmdf->Close(hImage);

	printf("DONE!\n");

	return 0;
}
