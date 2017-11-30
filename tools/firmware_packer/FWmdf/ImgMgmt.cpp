// ImgMgmt.cpp: implementation of the CImgMgmt class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "ImgMgmt.h"

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include<ctype.h>
#include<sys/stat.h>
#include <time.h>
#if defined(__APPLE__) || defined(__FreeBSD__)
#include <copyfile.h>
#else
#include <sys/sendfile.h>
#endif
#include <errno.h>
#define	RAM_PART_SCT_SIZE	0X200			// sector size : 512 byte 	
#define	RAM_PART_SCT_BITS	9				// sector size bits : 9 	

#define SIZE_100M 104857600

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//CString GetFileName(LPCTSTR lpFilePath);
void  GetFileName(LPCTSTR lpFilePath, LPTSTR lpFille, DWORD dwLen);

__int64 Get64bitLen(u32 lLow, u32 lHi);
HANDLE	FsOpenFile(LPCSTR lpFileName, BOOL bCreate);
DWORD	FsSeek(HANDLE hf, __int64 distance, DWORD dwMoveMethod);
DWORD	FsSeek_Gig(HANDLE hf, long distance, long distanceHigh, DWORD dwMoveMethod );
__int64 GetFileLength(const char * filename);
static void Get32BitLen(__int64 nLen, u32* nLow, u32* nHi);

#ifndef _WIN32
BOOL ReadFile(HANDLE hd, void* buff, int nByteToRead, DWORD* nBytesRead, void* unuse)
{
	*nBytesRead = fread(buff, nByteToRead, 1, hd);
	return (*nBytesRead) == nByteToRead;
}
BOOL WriteFile(HANDLE hd, LPBYTE buff, int nByteToWrite, DWORD* nByteWrite, void* unuse)
{
	*nByteWrite = fwrite(buff, 1, nByteToWrite, hd);
	return (*nByteWrite) == nByteToWrite;
}
void DeleteFile(const char* szPath)
{
	remove(szPath);
}
char *strupr(char *str) 
{ 
	char *ptr = str; 

	while (*ptr != '\0') { 
		if (islower(*ptr)) 
			*ptr = toupper(*ptr); 
		ptr++; 
	} 

	return str; 
}
void CloseHandle(HANDLE hd)
{
	fclose((FILE*)hd);
}




int CopyFile(const char* source, const char* destination, BOOL unuse)
{
	int input, output;
	if ((input = open(source, O_RDONLY)) == -1)
	{
		return -1;
	}
	if ((output = open(destination, O_RDWR | O_CREAT)) == -1)
	{
		close(input);
		return -1;
	}

	//Here we use kernel-space copying for performance reasons
#if defined(__APPLE__) || defined(__FreeBSD__)
	//fcopyfile works on FreeBSD and OS X 10.5+
	int result = fcopyfile(input, output, 0, COPYFILE_ALL);
#else
	//sendfile will work with non-socket output (i.e. regular file) on Linux 2.6.33+
	off_t bytesCopied = 0;
	struct stat fileinfo = {0};
	fstat(input, &fileinfo);
	int result = sendfile(output, input, &bytesCopied, fileinfo.st_size);
#endif

	close(input);
	close(output);

	return result;
}

char* gettimestr()
{
	static char ss[200]  ;
	time_t timep;  
	struct tm *p;  
	time(&timep);  
	p=localtime(&timep); /*取得当地时间*/  
	sprintf(ss, "%04d%02d%02d%02d%02d%02d", (1900+p->tm_year),(1+p->tm_mon), p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec);  
	return ss;
}

#endif


CImgMgmt::CImgMgmt()
{
//	ZeroMemory(&_ImgHead, sizeof(ImageHead_t));
//	_pImgHandle = NULL;
//	_ImgItems.reserve(1024);
//	_ImgItemMems.reserve(1024);
	
/*	HMODULE hmod = LoadLibrary("EPDKReg.dll");

	if(hmod!= NULL)
	{		
		pLoadPlugin LoadPlugin = (pLoadPlugin)GetProcAddress(hmod, "LoadPlugin");
		if(LoadPlugin)
		
		_pPluginFun = (AW_PLGIN_FUN_REGTRAN*)LoadPlugin();
	}
*/		
}

CImgMgmt::~CImgMgmt()
{
//	Release();
}

HIMAGE CImgMgmt::LoadImg(LPCSTR lpFileName)
{
	ImageHead_t ImageHead;
	char	seed[]	= "img";
	char key_buff[MAX_KEY_SIZE];
	u32 key_len = MAX_KEY_SIZE;
	DWORD dwLenRead;

	if (NULL == lpFileName)
		return NULL;

	if (0 == strlen(lpFileName))
		return NULL;
	
	if(_pCallbackFun)
	{
		_pCallbackFun(FWMDF_INFO_LOADING, 0, "");
	}

//	Release(); // 释放所有资源

	IMAGE_HANDLE* _pImgHandle = (IMAGE_HANDLE *)malloc(sizeof(IMAGE_HANDLE));
	if (NULL == _pImgHandle)
		return NULL;
	memset(_pImgHandle, 0, sizeof(IMAGE_HANDLE));
	//------------------------------------------------
	//初始化解密接口
	//------------------------------------------------
	for (u32 i = 0; i< IF_CNT; i++)
	{
		_pImgHandle->_rc_if_decode[i].handle 		= NULL;
		_pImgHandle->_rc_if_decode[i].Initial		= Initial;
		_pImgHandle->_rc_if_decode[i].EnDecode	= Decode;
		_pImgHandle->_rc_if_decode[i].UnInitial	= UnInitial;
		memset(key_buff, i, key_len);		//前面的数据初始化为0 note
		key_buff[key_len - 1] = seed[i];	//最后一个字节修改    note

		_pImgHandle->_rc_if_decode[i].handle = _pImgHandle->_rc_if_decode[i].Initial(key_buff, &key_len);
		if (NULL == _pImgHandle->_rc_if_decode[i].handle)
		{
			free(_pImgHandle); 
			return NULL;
		}
	}
	//------------------------------------------------
	//打开img文件
	//------------------------------------------------	
	_pImgHandle->_hf = FsOpenFile(lpFileName, FALSE); 
	if (NULL == _pImgHandle->_hf)
	{
		free(_pImgHandle); 
		return NULL;
	}
	
	if(_pCallbackFun)
	{
		_pCallbackFun(FWMDF_INFO_LOADING, 5 / 2, "");
	}
	//------------------------------------------------
	//读img头
	//------------------------------------------------
	//fread(&ImageHead, sizeof(ImageHead_t), 1, _pImgHandle->fp);
	ReadFile(_pImgHandle->_hf, &ImageHead, sizeof(ImageHead_t), &dwLenRead, NULL);
	u8 * pHead = (u8 *) &ImageHead;
	u8 * pHeadDecode = (u8 *)&_pImgHandle->_ImageHead;

	if(_pCallbackFun)
	{
		_pCallbackFun(FWMDF_INFO_LOADING, 10 / 2, "");
	}
	BOOL bEncrypt = FALSE;

	if(memcmp(ImageHead.magic, IMAGE_MAGIC, 8) == 0)
	{
		bEncrypt = FALSE;
	}
	else
		bEncrypt = TRUE;


	if(bEncrypt)
	{
		for (int i = 0; i < sizeof(ImageHead_t); i+= ENCODE_LEN)
		{
			u8 * pin = pHead + i;
			u8 * pout= pHeadDecode + i;
			if (OK  != _pImgHandle->_rc_if_decode[HEAD_ID].EnDecode(_pImgHandle->_rc_if_decode[HEAD_ID].handle, pin , pout))
			{
				//fclose(_pImgHandle->fp);
				CloseHandle(_pImgHandle->_hf);
				free(_pImgHandle);
				return NULL;
			}
		}
		_pImgHandle->_bUseEncypt = TRUE;
	}
	else
	{
		memcpy(&_pImgHandle->_ImageHead, &ImageHead, sizeof(ImageHead_t));
		_pImgHandle->_bUseEncypt = FALSE;
	}
	//------------------------------------------------
	//比较magic
	//------------------------------------------------
	if (memcmp(_pImgHandle->_ImageHead.magic, IMAGE_MAGIC, 8) != 0)
	{
	//	fclose(_pImgHandle->fp);
		CloseHandle(_pImgHandle->_hf);
		free(_pImgHandle);
		return NULL;
	}
	if(_pImgHandle->_ImageHead.version < IMAGE_HEAD_VERSION)
	{
		CloseHandle(_pImgHandle->_hf);
		free(_pImgHandle);
		OPCallback(FWMDF_INFO_ERROR,FWMDF_ERROR_VERSION);
		return NULL;
	}
	//Msg("itemcount=%d", pImage->ImageHead.itemcount);
	u32 ItemCnt = _pImgHandle->_ImageHead.itemcount;
	u32 ItemTableSize =  ItemCnt* sizeof(ImageItem_t);

	if(_pCallbackFun)
	{
		_pCallbackFun(FWMDF_INFO_LOADING, 15/ 2, "");
	}

	_pImgHandle->_pItemTable = (ImageItem_t*)malloc(ItemTableSize);
	if (NULL == _pImgHandle->_pItemTable)
	{
	//	fclose(_pImgHandle->fp);
		CloseHandle(_pImgHandle->_hf);
		free(_pImgHandle);
		return NULL;
	}
	
	//------------------------------------------------
	//read ItemTable
	//------------------------------------------------
	u8 * ItemTableBuf = (u8 *)malloc(ItemTableSize);//用于解密table的buffer
	if (NULL == ItemTableBuf)
	{
	//	fclose(_pImgHandle->fp);
		CloseHandle(_pImgHandle->_hf);
		free(_pImgHandle->_pItemTable);
		free(_pImgHandle);
		return NULL;
	}
#ifndef _WIN32
	fseek(_pImgHandle->_hf, _pImgHandle->_ImageHead.itemoffset, SEEK_SET);
#else
	SetFilePointer(_pImgHandle->_hf, _pImgHandle->_ImageHead.itemoffset, NULL, SEEK_SET);//yorick
#endif

	//fread(ItemTableBuf, ItemTableSize, 1, _pImgHandle->fp);
	//fseek(_pImgHandle->fp, 0, SEEK_CUR);
	ReadFile(_pImgHandle->_hf, ItemTableBuf, ItemTableSize, &dwLenRead, NULL);
	//------------------------------------------------
	// decode ItemTable
	//------------------------------------------------
	u8 * pItemTableDecode = (u8 *)_pImgHandle->_pItemTable;
	//_ImgItems.resize(ItemCnt);

	if(_pImgHandle->_bUseEncypt)
	{
		for (int i = 0; i < ItemTableSize; i+= ENCODE_LEN)
		{
			u8 * pin = ItemTableBuf + i;
			u8 * pout= pItemTableDecode + i;
			if (OK  != _pImgHandle->_rc_if_decode[TABLE_ID].EnDecode(_pImgHandle->_rc_if_decode[TABLE_ID].handle, pin , pout))
			{
				free(ItemTableBuf);
			//	fclose(_pImgHandle->fp);
				CloseHandle(_pImgHandle->_hf);
				free(_pImgHandle->_pItemTable);
				free(_pImgHandle);
				return NULL;
			}
		}
	}
	else
	{
		memcpy(_pImgHandle->_pItemTable, ItemTableBuf, ItemTableSize);
	}

#ifdef _DEBUG
	//CreateDirectory("c:\\temp\\RES", NULL);
#endif
	_pImgHandle->_pItemMem = (ImgMemItem*)malloc(ItemCnt* sizeof(ImgMemItem));
	//_ImgItemMems.resize(ItemCnt);

	int stlen = sizeof(ImageItem_t);
//////////////////////////////////////////////////////////////////////////
// 统计大小
	__int64 nTotalLen = 0;
	
	for(int i = 0 ; i< ItemCnt; i++)
	{
		ImageItem_t& item = _pImgHandle->_pItemTable[i];
		char szInfo[MAX_PATH] = "";
		__int64 nItemLen = Get64bitLen(item.datalenLo, item.datalenHi);
		nTotalLen += nItemLen;
		//sprintf(szInfo, "%s offset:0x%x length = 0x%x\n", item.subType, item.offsetLo, item.datalenLo);
		//OutputDebugString(szInfo);
	}

//////////////////////////////////////////////////////////////////////////

	
	double nStep = 85.0 / nTotalLen;

	__int64  nPos = 0;

	for(int i = 0; i < ItemCnt; i++)
	{
#ifdef _WIN32
		MSG msgTmp;
		while (PeekMessage(&msgTmp, NULL, 0, 0, PM_REMOVE))
		{
			if (msgTmp.message == WM_QUIT)
				 return FALSE;

			TranslateMessage(&msgTmp);
			DispatchMessage(&msgTmp);
		}
#endif
		ImageItem_t& item = _pImgHandle->_pItemTable[i];
		ImgMemItem& itemMem = _pImgHandle->_pItemMem[i];
		
		memcpy(&item,pItemTableDecode+i*sizeof(ImageItem_t), sizeof(ImageItem_t));

		__int64 dwLen = Get64bitLen(item.datalenLo, item.datalenHi);
		__int64 dwFileLen = Get64bitLen(item.filelenLo, item.filelenHi);
	
	
		char szInfo[MAX_PATH];
		sprintf(szInfo, "GetITem [%s] [%s] Len = (%d, %d)\n",
						item.mainType, item.subType,
						item.filelenLo, item.filelenHi);
		
		OutputDebugString(szInfo);

		if(dwLen > SIZE_100M)
		{
			TCHAR szTempPath [MAX_PATH];
			TCHAR szTempFile [MAX_PATH];
			char temp_path[MAX_PATH] = "";
#ifdef _WIN32
			DWORD dwResult=::GetTempPath(MAX_PATH, szTempPath);
			UINT nResult=GetTempFileName(szTempPath, "~ex", 0, szTempFile);	
			sprintf(temp_path, "%s", szTempFile);
#else
			strcpy(szTempPath, "./");
			sprintf(temp_path, "%s%s~ex", szTempPath, gettimestr()); //这就不允许Insert了，只能add
#endif
			
			ZeroMemory(&itemMem, sizeof(ImgMemItem));
			itemMem.nLength = dwLen;// Get64bitLen(item.filelenLo, item.filelenHi);//dwLen;
			itemMem.pBuffer = (LPBYTE)malloc(MAX_PATH);
			memcpy(itemMem.pBuffer,temp_path, MAX_PATH);
			
			__int64 nOffset64 = Get64bitLen(item.offsetLo, item.offsetHi);
			FsSeek(_pImgHandle->_hf, nOffset64, SEEK_SET);
			
		
			__int64 nReadLen = dwLen;
			
			__int64 n20M = 1024*1024*20;
			LPBYTE lpBuff = new BYTE[n20M];
			LPBYTE lpOutBuff = new BYTE[n20M];
			DWORD dwLRead;
			HANDLE hf = FsOpenFile(temp_path, TRUE);
                       if (!hf)
			{
				printf("open file [%s] failed! err=%d", temp_path, errno);
				return NULL;
			}
			__int64  nItemFilePos = 0;
			while(nReadLen >= ENCODE_LEN)
			{
				int nThisRead = min(n20M, nReadLen);
				ReadFile(_pImgHandle->_hf, lpBuff, nThisRead, &dwLRead, NULL);

				if(_pImgHandle->_bUseEncypt)
				{
					for (int j = 0; j < nThisRead; j+= ENCODE_LEN)
					{
						u8 * pin = lpBuff + j;
						u8 * pout= lpOutBuff + j;
						if (OK  != _pImgHandle->_rc_if_decode[DATA_ID].EnDecode(_pImgHandle->_rc_if_decode[DATA_ID].handle, pin , pout))
						{
							//free(lpBuff);
							delete []lpBuff;
							//fclose(_pImgHandle->fp);
							CloseHandle(_pImgHandle->_hf);
							//free(lpOutBuff);
							delete []lpOutBuff;
							free(_pImgHandle);		
							return NULL;
						}
					}
				}
				else
				{
					memcpy(lpOutBuff,lpBuff, nThisRead);
				}
				nPos += nThisRead;
				if(_pCallbackFun)
				{
					_pCallbackFun(FWMDF_INFO_LOADING, (15 + nStep * nPos) / 2, "");
				}
				if((nItemFilePos + nThisRead) < dwFileLen)
					WriteFile(hf, lpOutBuff, nThisRead, &dwLRead, NULL);							
				else
					WriteFile(hf, lpOutBuff, dwFileLen - nItemFilePos, &dwLRead, NULL);					

				nReadLen -= nThisRead;
				nItemFilePos += dwLRead;
#ifdef _WIN32
				MSG msgTmp;
				while (PeekMessage(&msgTmp, NULL, 0, 0, PM_REMOVE))
				{
					if (msgTmp.message == WM_QUIT)
						 return FALSE;

					TranslateMessage(&msgTmp);
					DispatchMessage(&msgTmp);
				}
#endif

			}	
			delete []lpBuff;
			delete []lpOutBuff;
			lpBuff = NULL;
			lpOutBuff = NULL;
			CloseHandle(hf);
			continue;
		}
//////////////////////////////////////////////////////////////////////////
// 少于100M的文件用原来的方法读取
		LPBYTE lpBuff = new BYTE[dwLen];
		
		ZeroMemory(&itemMem, sizeof(ImgMemItem));
		itemMem.nLength = dwLen;//Get64bitLen(item.filelenLo, item.filelenHi);
	
	//	int nRealLen = (dwLen + 1024 -1) / 1024 * 1024;
		itemMem.pBuffer = (BYTE*)malloc(dwLen);
		memset(itemMem.pBuffer, 0xcd, dwLen);

		LPBYTE lpOutBuff = _pImgHandle->_pItemMem[i].pBuffer;

		//fseek(_pImgHandle->fp, item.offset, SEEK_SET);
		//fread(lpBuff, dwLen, 1, _pImgHandle->fp);
		__int64 nOffset64 = Get64bitLen(item.offsetLo, item.offsetHi);
		FsSeek(_pImgHandle->_hf, nOffset64, SEEK_SET);
		ReadFile(_pImgHandle->_hf, lpBuff,dwLen, &dwLenRead, NULL);

		char szName[260] = "";
		//memcpy(szName, item.name, 260);

		GetFileName((char*)item.name, szName, 260);

		strcpy(itemMem.szName, szName);
		if(_pImgHandle->_bUseEncypt)
		{		
			for (int j = 0; j < dwLen; j+= ENCODE_LEN)
			{
				u8 * pin = lpBuff + j;
				u8 * pout= lpOutBuff + j;
				if (OK  != _pImgHandle->_rc_if_decode[DATA_ID].EnDecode(_pImgHandle->_rc_if_decode[DATA_ID].handle, pin , pout))
				{
					//free(lpBuff);
					delete []lpBuff;
					//fclose(_pImgHandle->fp);
					CloseHandle(_pImgHandle->_hf);
					free(lpOutBuff);//?
					free(_pImgHandle);
					return NULL;
				}
			}			
		}
		else
		{
			memcpy(lpOutBuff, lpBuff, dwLen);
		}
		nPos += dwLen;
		if(_pCallbackFun)
		{
			_pCallbackFun(FWMDF_INFO_LOADING, (15 + nStep * nPos)/ 2, "");
		}
		//free(lpBuff);
		delete []lpBuff;
		lpBuff = NULL;
#ifdef _DEBUG
/*		CString csTmpPath;				
		csTmpPath.Format("c:\\temp\\RES\\%s", szName);
		FILE* ftmp = fopen(csTmpPath, "wb");
		fwrite(lpOutBuff, dwLen, 1, ftmp);
		fclose(ftmp);
		TRACE("%d %s\n",i,  _ImgItems.at(i).name);
*/
#endif
			
	}
	free(ItemTableBuf);
	if(_pCallbackFun)
	{
		_pCallbackFun(FWMDF_INFO_LOADING, 100 / 2, "");
	}
	return (HIMAGE)_pImgHandle;
}

void CImgMgmt::OPCallback(WPARAM wParam, LPARAM lParam)
{
	if(_pCallbackFun)
	{
		_pCallbackFun(wParam, lParam, "");
	}
}

LRESULT CImgMgmt::SaveImg(HIMAGE hImage, LPCSTR lpFileName)
{
	u32		dwRet = OK;
	u32		ret = OK;
	ImageHeadAttr_t ImageHeadAttr;
	__int64		offset;
	__int64		length;
	__int64		totallength;
	u32		i;
	u8		buffer_encode[ALIGN_SIZE];
	char	seed[]	= "img";
	HANDLE  fp = NULL;
	f32  * 	p = NULL;
	f32 	f = 0.0;

	IMAGE_HANDLE* _pImgHandle = (IMAGE_HANDLE*)hImage;
	DWORD dwRead;
	RC_ENDECODE_IF_t rc_if_encode[IF_CNT];
	char key_buff[MAX_KEY_SIZE];
	u32 key_len = MAX_KEY_SIZE;

	__int64 nProcessPos = 0; // 保存进度， 在回调函数中使用
		 
	
//	if (NULL == g_pImageItem || g_ImageHead.itemcount == 0)
//		return __LINE__;

	//------------------------------------------------
	//初始化加密接口
	//------------------------------------------------
	for ( i = 0; i< IF_CNT; i++)
	{
		rc_if_encode[i].handle 		= NULL;
		rc_if_encode[i].Initial		= Initial;
		rc_if_encode[i].EnDecode	= Encode;
		rc_if_encode[i].UnInitial	= UnInitial;
		memset(key_buff, i, key_len);		//注意了 note
		key_buff[key_len - 1] = seed[i];	//note

		rc_if_encode[i].handle = rc_if_encode[i].Initial(key_buff, &key_len);
		if (NULL == rc_if_encode[i].handle)
			return __LINE__;
	}

		
	OPCallback(FWMDF_INFO_SAVING, 0);
	offset = 0;
	totallength = 0;
	length = 0;
	//------------------------------------------------
	//创建 image 文件
	//------------------------------------------------
	//fp = fopen((const char *)lpFileName, "wb+");
	fp = FsOpenFile((const char*)lpFileName, TRUE);
	if (fp == NULL)
	{
	//	Err("BuildImage", __FILE__, __LINE__, "create image file failed");		
		return __LINE__;
	}
	
	offset += ALIGN_SIZE;

	offset += _pImgHandle->_ImageHead.itemcount * sizeof(ImageItem_t);// for item data offset


	//------------------------------------------------
	//write item table
	//------------------------------------------------
	for (i = 0; i < _pImgHandle->_ImageHead.itemcount; i++)
	{
		ImgMemItem& item = _pImgHandle->_pItemMem[i];
		ImageItem_t& tmp = _pImgHandle->_pItemTable[i];

		__int64 nDataLen = 0;

		length = item.nLength;

		if (length == 0 )
		{
	//		Msg("error:空文件%s", _ImgItems.at(i).name);
			return __LINE__;
		}
		//Msg("[%d]%s len=%d\n", i, g_pImageItem[i].name, length);
		tmp.version = IMAGE_ITEM_VERSION;		//
		tmp.size = sizeof(ImageItem_t);			//
		tmp.mainType;							//
		tmp.subType;							//

		tmp.attr;								//属性,加密，压缩等 note
		
		//rc6加密算法16字节对齐
		__int64 encode_data_len = ((length + ENCODE_LEN - 1) / ENCODE_LEN ) * ENCODE_LEN;
	
		Get32BitLen(encode_data_len, &_pImgHandle->_pItemTable[i].datalenLo, &_pImgHandle->_pItemTable[i].datalenHi);
		//Get32BitLen(length, &_pImgHandle->_pItemTable[i].filelenLo, &_pImgHandle->_pItemTable[i].filelenHi);
		Get32BitLen(offset, &_pImgHandle->_pItemTable[i].offsetLo, &_pImgHandle->_pItemTable[i].offsetHi);
		
	
		/*
		Get32BitLen(encode_data_len, &_ImgItems.at(i).datalenLo, &_ImgItems.at(i).datalenHi);
		Get32BitLen(length, &_ImgItems.at(i).filelenLo, &_ImgItems.at(i).filelenHi);
		Get32BitLen(offset, &_ImgItems.at(i).offsetLo, &_ImgItems.at(i).offsetHi);
		*/
		
		//当前文件按照align对齐后的下一个文件的offset
		offset += ((encode_data_len + _pImgHandle->_ImageHead.align - 1) / _pImgHandle->_ImageHead.align) * _pImgHandle->_ImageHead.align;
		/*
		if ((encode_data_len % g_ImageHead.align ) == 0)
			offset += encode_data_len;
		else
			offset += (encode_data_len / g_ImageHead.align + 1) * g_ImageHead.align;
		*/

		tmp.checksum= 0;		///校验和
		tmp.name;				///文件名称
	

		nDataLen = Get64bitLen(tmp.datalenLo, tmp.datalenHi);
		totallength  += nDataLen;

#ifdef _WIN32
		u32 address = (u32)_pImgHandle->_pItemTable[i].res;
#else
		u8* address = (u8*)_pImgHandle->_pItemTable[i].res;
#endif
		p = (f32 *)address;					///保留
		for (int k = 0; k < IMAGE_ITEM_RCSIZE/4; k++)		//
		{
			f    = _pImgHandle->_pItemTable[i].subType[ min(0, k / 13)];
			f   *= (f32)(_pImgHandle->_pItemTable[i].filelenLo + _pImgHandle->_pItemTable[i].mainType[min(0, k / 25)]);
			p[k] = (f32)(f / ((f32 )k + 3.114));
		}
	}
	Get32BitLen(totallength, &(_pImgHandle->_ImageHead.lenLo), &(_pImgHandle->_ImageHead.lenHi));
	OPCallback(FWMDF_INFO_SAVING, 5);
	//Msg("fpos=%x\n", ftell(fp));
	
	//------------------------------------------------
	//refresh image head
	//------------------------------------------------
	//_pImgHandle->ImageHead.len = offset;
	Get32BitLen(offset, &_pImgHandle->_ImageHead.lenLo, &_pImgHandle->_ImageHead.lenHi);
	//------------------------------------------------
	//加密img头
	//------------------------------------------------
	u8 * pHead =(u8 *)&_pImgHandle->_ImageHead;
	for (i = 0; i < ALIGN_SIZE; i++)
	{
		buffer_encode[i] = i*i;	//randbuffer
	}
	if(_pImgHandle->_bUseEncypt)
	{
		for (i = 0; i < sizeof(ImageHead_t); i+= ENCODE_LEN)
		{
			u8 * pin = pHead + i;
			u8 * pout= buffer_encode + i;
			ret = rc_if_encode[HEAD_ID].EnDecode(rc_if_encode[HEAD_ID].handle, pin , pout);
			if (OK != ret)
			{
				return ret;
			}
		}
	}
	else
	{
		memcpy(buffer_encode, pHead, sizeof(ImageHead_t));
	}
	//------------------------------------------------
	//write head encode data
	//------------------------------------------------
	//if (1 != fwrite(buffer_encode, ALIGN_SIZE, 1, fp))	//加密img头数据写入文件
	if (1 != WriteFile(fp, buffer_encode, ALIGN_SIZE, &dwRead, NULL))	//加密img头数据写入文件
	{
		CloseHandle(fp);
		printf("Error:image write header failed! ALIGN_SIZE= %d, write=%d\n", ALIGN_SIZE, dwRead);
//		Msg("error:image head write failed!");
		return __LINE__;
	}

	//------------------------------------------------
	//加密item表
	//------------------------------------------------
	u32  table_len = _pImgHandle->_ImageHead.itemcount * sizeof(ImageItem_t);
	u8 * ptable_encode = (u8 *) malloc(table_len);

/*	for(i = 0; i < _pImgHandle->ImageHead.itemcount; i++) // 将vector中的数组还原到buff中去
	{
		memcpy(&_pImgHandle->ItemTable[i], &_ImgItems.at(i), sizeof(ImageItem_t));
	}
*/	
	u8 * ptable = (u8 *)_pImgHandle->_pItemTable;
	if (NULL == ptable_encode)
		return __LINE__;
	//Msg("table_len = %d", table_len);
	if(_pImgHandle->_bUseEncypt)
	{
		for (i = 0; i < table_len; i+= ENCODE_LEN)
		{
			u8 * pin = ptable + i;
			u8 * pout= ptable_encode + i;
			ret = rc_if_encode[TABLE_ID].EnDecode(rc_if_encode[TABLE_ID].handle, pin , pout);
			if (OK != ret)
			{
				return ret;
			}
			//Msg("encode table i=%d", i);
		}
	}
	else
	{
		memcpy(ptable_encode, ptable, table_len);
	}
	//------------------------------------------------
	//加密table数据写入文件
	//------------------------------------------------
//	fwrite(ptable_encode, table_len, 1, fp);
	WriteFile(fp, ptable_encode, table_len, &dwRead, NULL);
	//fseek(fp, 0, SEEK_CUR);
	free(ptable_encode);
	ptable_encode = NULL;

	OPCallback(FWMDF_INFO_SAVING, 10);
	//Msg("fpos=%x\n", ftell(fp));
	//Msg("itemcount=%d sizeof(ImageItem_t)=%x\n", g_ImageHead.itemcount, sizeof(ImageItem_t));
	
	//////////////////////////////////////////////////////////////////////////
// 统计大小
	__int64 nTotalLen = 0;
	
	for(i = 0 ; i< _pImgHandle->_ImageHead.itemcount; i++)
	{
		ImageItem_t& item = _pImgHandle->_pItemTable[i];
		__int64 nItemLen = Get64bitLen(item.datalenLo, item.datalenHi);
		nTotalLen += nItemLen;
	}

	double fStep = 90.0 / nTotalLen;
	
	//------------------------------------------------
	//逐个写入文件
	//------------------------------------------------
	for (i = 0; i < _pImgHandle->_ImageHead.itemcount; i++)
	{		
		ImageItem_t* item = &(_pImgHandle->_pItemTable[i]);
		ImgMemItem& itemMem = _pImgHandle->_pItemMem[i];
		if (NULL == item)
		{
//			Msg("error: %d!", __LINE__);
			return __LINE__;
		}
#ifdef _WIN32
		MSG msgTmp;
		while (PeekMessage(&msgTmp, NULL, 0, 0, PM_REMOVE))
		{
			if (msgTmp.message == WM_QUIT)
				 return FALSE;

			TranslateMessage(&msgTmp);
			DispatchMessage(&msgTmp);
		}
#endif
		u8  buffer[ENCODE_LEN];
		u8  buffer_en[ENCODE_LEN];		
		__int64 nLenTmp = Get64bitLen(item->datalenLo, item->datalenHi);
		__int64 item_total_len = ((nLenTmp + _pImgHandle->_ImageHead.align - 1) / _pImgHandle->_ImageHead.align) * _pImgHandle->_ImageHead.align;
		__int64 length = Get64bitLen(item->filelenLo,item->filelenHi);		
    	FsSeek_Gig(fp, item->offsetLo, item->offsetHi, SEEK_SET);
		if(length > SIZE_100M)
		{
			char szTmpPath[MAX_PATH] = "";
			memcpy(szTmpPath, itemMem.pBuffer, MAX_PATH);
			HANDLE hf = FsOpenFile(szTmpPath, FALSE);
			
			__int64 n20M = 1024*1024*20;
			LPBYTE lpReadBuff = new BYTE[n20M];
			LPBYTE lpOutBuff = new BYTE[n20M];
			__int64 nReadLen = length;
			DWORD dwLRead;
			
			while(nReadLen > 0)
			{
				LPBYTE lpTmpBuff = itemMem.pBuffer;
				__int64 nThisRead = min(n20M, nReadLen);

				if(nThisRead == nReadLen)
				{
					memset(lpReadBuff, 0x00, (nThisRead + ENCODE_LEN -1)/ENCODE_LEN *ENCODE_LEN );
				}

				ReadFile(hf, lpReadBuff, nThisRead, &dwLRead, NULL);
				if(nThisRead == nReadLen)
				{
					nThisRead = (nThisRead+ ENCODE_LEN -1)/ENCODE_LEN *ENCODE_LEN;
				}
				if(_pImgHandle->_bUseEncypt)
				{			
					for (int j = 0; j < nThisRead; j+= ENCODE_LEN)
					{
						BYTE* pIn = lpReadBuff + j;
						BYTE* pOut = lpOutBuff + j;
						
						memset(pOut, 0, ENCODE_LEN);				

						ret = rc_if_encode[DATA_ID].EnDecode(rc_if_encode[DATA_ID].handle, pIn , pOut);
						if (OK != ret)
						{
						//	Msg("error: %d!", __LINE__);
							delete []lpReadBuff;
							delete []lpOutBuff;
							lpReadBuff = NULL;
							lpOutBuff = NULL;
							CloseHandle(hf);
							return ret;
						}			
					}					
				}
				else
				{
					memcpy(lpOutBuff, lpReadBuff, nThisRead);
				}
#ifdef _WIN32
				MSG msgTmp;
				while (PeekMessage(&msgTmp, NULL, 0, 0, PM_REMOVE))
				{
					if (msgTmp.message == WM_QUIT)
						 return FALSE;

					TranslateMessage(&msgTmp);
					DispatchMessage(&msgTmp);
				}
#endif
				WriteFile(fp, lpOutBuff, nThisRead, &dwLRead, NULL);
			
				nProcessPos+=nThisRead;
				OPCallback(FWMDF_INFO_SAVING, 10 + fStep *  nProcessPos);
				nReadLen-=nThisRead;		

			}
			CloseHandle(hf);
			delete []lpReadBuff;
			lpReadBuff = NULL;
			delete []lpOutBuff;
			lpOutBuff = NULL;
			continue;
		}
		//------------------------------------------------
		//
		//------------------------------------------------
		LPBYTE lpTmpBuff = itemMem.pBuffer;
		LPBYTE lpOutBuff = new BYTE[itemMem.nLength];
		int nCurPos = 0;
		if(_pImgHandle->_bUseEncypt)
		{			
			for(int j = 0; j < itemMem.nLength; j+=ENCODE_LEN)
			{
				/*memset(buffer, 0, ENCODE_LEN);
				memset(buffer_en, 0, ENCODE_LEN);
				memcpy(buffer, lpTmpBuff, ENCODE_LEN);
				lpTmpBuff+=ENCODE_LEN;
				*/
				BYTE* pIn = lpTmpBuff+j;
				BYTE* pOut = lpOutBuff + j;

				memset(pOut, 0, ENCODE_LEN);

				ret = rc_if_encode[DATA_ID].EnDecode(rc_if_encode[DATA_ID].handle, pIn , pOut);
				if (OK != ret)
				{
					delete []lpOutBuff;
					lpOutBuff = NULL;
					return ret;
				}					
			}
			
		}
		else
		{
			memcpy(lpOutBuff, lpTmpBuff, itemMem.nLength);
		}
	
		//DWORD dwPos = SetFilePointer(fp, 0, NULL, SEEK_CUR);
	
		if (1 != WriteFile(fp, lpOutBuff, itemMem.nLength, &dwRead, NULL))
		{
			delete []lpOutBuff;
			lpOutBuff = NULL;
			return __LINE__;
		}
		nProcessPos+=itemMem.nLength;
		OPCallback(FWMDF_INFO_SAVING, 10 + fStep *  nProcessPos);
		delete []lpOutBuff;
		lpOutBuff = NULL;		

	
	
	
	}
	//Msg("%d ", __LINE__);
	//------------------------------------------------
	//no extern data
	//------------------------------------------------
	

	//------------------------------------------------
	//close image file
	//------------------------------------------------
	//fclose(fp);
	CloseHandle(fp);

	//------------------------------------------------
	//释放空间
	//------------------------------------------------

	//------------------------------------------------
	//关闭加密接口
	//------------------------------------------------
	for ( i = 0; i< IF_CNT; i++)
	{
		if (OK != rc_if_encode[i].UnInitial(rc_if_encode[i].handle))
			return ret;
	}
	
//	OPCallback(FWMDF_INFO_SAVING, 100);
	return 0;
}

void CImgMgmt::Release(HIMAGE hImage)
{
	IMAGE_HANDLE* _pImgHandle = (IMAGE_HANDLE*)hImage;
	if(_pImgHandle)
	{
		int ns = _pImgHandle->_ImageHead.itemcount;
		for(int i = 0; i < ns; i++)
		{
			ImgMemItem& item = _pImgHandle->_pItemMem[i];
			if(item.nLength > SIZE_100M) //删除临时文件夹
			{
				char szTmpPath[MAX_PATH] = "";
				memcpy(szTmpPath, item.pBuffer, MAX_PATH);
#ifdef _WIN32
				DeleteFile(szTmpPath);
#else
				remove(szTmpPath);
#endif
			}
			else
				free(item.pBuffer);
		}
		free(_pImgHandle->_pItemMem);
		free(_pImgHandle->_pItemTable);
#ifndef _WIN32
		fclose(_pImgHandle->_hf);
#else
		CloseHandle(_pImgHandle->_hf);
#endif
		for (int i = 0; i< IF_CNT; i++)
		{
			_pImgHandle->_rc_if_decode[i].UnInitial(_pImgHandle->_rc_if_decode[i].handle);
		}
		free(_pImgHandle);
		_pImgHandle = NULL;
	}
//	_ImgItemMems.clear();
}


int  CImgMgmt::EnumItem(HIMAGE hImage, LPCSTR lpFileName, DWORD& dwLen) //枚举文件，返回索引和长度
{
	IMAGE_HANDLE* _pImageHandle = (IMAGE_HANDLE*)hImage;
	int ns = _pImageHandle->_ImageHead.itemcount;
	for(int i = 0; i < ns; i++)
	{
		ImgMemItem& item = _pImageHandle->_pItemMem[i];
		if(strcmp(item.szName, lpFileName) == 0)
		{
			dwLen = item.nLength;
			return i;
		}
	}

	return -1;
}

int CImgMgmt::GetItemIndex(HIMAGE hImage , LPCSTR szMainType, LPCTSTR szSubType)
{

	IMAGE_HANDLE* _pImageHandle = (IMAGE_HANDLE*)hImage;
	int ns = _pImageHandle->_ImageHead.itemcount;
	char szRealMainTyp[32];
	char szRealSubTyp[32];

	char szItemMain[32];
	char szItemSub[32];
	strcpy(szRealSubTyp, szSubType);
	strcpy(szRealMainTyp, szMainType);
	strupr(szRealSubTyp);
	strupr(szRealMainTyp);
	for(int i = 0; i < ns; i++)
	{
		ImageItem_t& item = _pImageHandle->_pItemTable[i];
		ZeroMemory(szItemMain, 32);
		ZeroMemory(szItemSub, 32);
		memcpy(szItemMain, item.mainType, 8);
		memcpy(szItemSub, item.subType, 16);
		strupr(szItemMain);
		strupr(szItemSub);
		if(
			(memcmp(szItemMain, szRealMainTyp, MAINTYPE_LEN) == 0)
			&& (memcmp(szItemSub, szRealSubTyp, SUBTYPE_LEN) == 0)
			)
		{
		
			return i;
		}
	}
	return -1;
}

int CImgMgmt::GetItemIndex(HIMAGE hImage , LPCSTR lpFileName)
{
	IMAGE_HANDLE* _pImageHandle = (IMAGE_HANDLE*)hImage;
	int ns = _pImageHandle->_ImageHead.itemcount;
	for(int i = 0; i < ns; i++)
	{
		ImgMemItem& item = _pImageHandle->_pItemMem[i];
		if(strcmp(item.szName, lpFileName) == 0)
		{
		
			return i;
		}
	}

	return -1;
}

__int64 CImgMgmt::GetItemSize(HIMAGE hImage , int nIndex)
{
	IMAGE_HANDLE* _pImageHandle = (IMAGE_HANDLE*)hImage;
	//ImgMemItem& item = _pImageHandle->_pItemMem[nIndex];
	ImageItem_t& item = _pImageHandle->_pItemTable[nIndex];
	return Get64bitLen(item.filelenLo, item.filelenHi);

}

LRESULT CImgMgmt::GetImgItem(HIMAGE hImage, int nIndex, LPBYTE lpBuff, DWORD dwLen)    //获取文件内容
{
	IMAGE_HANDLE* _pImageHandle = (IMAGE_HANDLE*)hImage;
	int ns = _pImageHandle->_ImageHead.itemcount;
	if(nIndex >= ns || nIndex < 0)
		return __LINE__;
	
	ImgMemItem& item = _pImageHandle->_pItemMem[nIndex];
	if(dwLen > item.nLength)
		return __LINE__;

	if(item.nLength > SIZE_100M)
	{
		memcpy(lpBuff, item.pBuffer, MAX_PATH);	
		return 0;
	}

	memcpy(lpBuff, item.pBuffer, item.nLength);
	
	return 0;
}

LRESULT CImgMgmt::SetImgItem(HIMAGE hImage, int nIndex, LPBYTE lpBuff, DWORD dwLen)	//设置文件
{
	if(dwLen > SIZE_100M)
		return __LINE__;
	IMAGE_HANDLE* _pImageHandle = (IMAGE_HANDLE*)hImage;
	int nAligned = _pImageHandle->_ImageHead.align;
	int ns = _pImageHandle->_ImageHead.itemcount;

	if(nIndex >= ns || nIndex < 0)
		return __LINE__;
	
	ImgMemItem& item = _pImageHandle->_pItemMem[nIndex];
	ImageItem_t& itemTab = _pImageHandle->_pItemTable[nIndex];

	if(dwLen <= item.nLength)
	{
		ZeroMemory(item.pBuffer, item.nLength);
		memcpy(item.pBuffer, lpBuff, dwLen);		
		//item.nLength = ((dwLen + ENCODE_LEN  - 1) / ENCODE_LEN ) * ENCODE_LEN;
		item.nLength = ((dwLen + nAligned - 1) / nAligned) * nAligned;
		itemTab.filelenLo = dwLen;
		itemTab.filelenHi = 0;
	}
	else
	{
		free(item.pBuffer);
		//item.nLength = ((dwLen + ENCODE_LEN - 1) / ENCODE_LEN ) * ENCODE_LEN;
		item.nLength = ((dwLen + nAligned - 1) / nAligned) * nAligned;
		item.pBuffer = (BYTE*)malloc(item.nLength);
		memset(item.pBuffer, 0x00, item.nLength);
		memcpy(item.pBuffer, lpBuff, dwLen);
		itemTab.filelenLo = dwLen;
		itemTab.filelenHi = 0;	
	}

	return 0;
}


LRESULT CImgMgmt::SetImgItem(HIMAGE hImage, int nIndex, LPCTSTR lpFileName)
{
	__int64 nFileLen = GetFileLength(lpFileName) ; 

	IMAGE_HANDLE* _pImageHandle = (IMAGE_HANDLE*)hImage;
	int nAligned = _pImageHandle->_ImageHead.align;
	
	int ns = _pImageHandle->_ImageHead.itemcount;
 
	if(nIndex >= ns || nIndex < 0)
		return __LINE__;
		
	ImageItem_t& itemTab = _pImageHandle->_pItemTable[nIndex];
	ImgMemItem& item = _pImageHandle->_pItemMem[nIndex];
 
	
	if( nFileLen < SIZE_100M)
	{
#ifdef _WIN32
		SECURITY_ATTRIBUTES sa;
		int nAlian = _pImageHandle->_ImageHead.align;
		sa.nLength = sizeof(sa);
		sa.lpSecurityDescriptor = NULL;
		sa.bInheritHandle = TRUE;
		HANDLE hd = CreateFile(lpFileName, GENERIC_READ, FILE_SHARE_DELETE,&sa, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
#else
		HANDLE hd = fopen(lpFileName, "rb");
#endif // _WIN32
		
		//__int64 dwBufferLen = ((nFileLen + ENCODE_LEN - 1) / ENCODE_LEN ) * ENCODE_LEN;
		
		__int64 dwBufferLen = ((nFileLen +  nAligned - 1) / nAligned ) * nAligned;
		LPBYTE lpBuffer = new BYTE[dwBufferLen];
		memset(lpBuffer, 0x00, dwBufferLen);
		DWORD dwRead;
		ReadFile(hd, lpBuffer, nFileLen, &dwRead, NULL);
		CloseHandle(hd);		
		
		if(item.nLength & 0xffffffff00000000)
		{
			DeleteFile((char*)item.pBuffer);		
		}
		else
		{
			free(item.pBuffer);
			item.pBuffer = (BYTE*)malloc(dwBufferLen);
			memcpy(item.pBuffer, lpBuffer, dwBufferLen);
		}
		item.nLength = dwBufferLen;
	//	itemTab.filelen = nFileLen;
	//	itemTab.datalen = dwBufferLen;
		Get32BitLen(nFileLen, &itemTab.filelenLo, &itemTab.filelenHi);
		Get32BitLen(dwBufferLen, &itemTab.datalenLo, &itemTab.datalenHi);
	}	
	else
	{		
		BOOL bTheSameFile = FALSE;
				
		if(item.nLength >= SIZE_100M)// & 0xffffffff00000000)
		{
			if(strcmp((char*)item.pBuffer, lpFileName) == 0) 
				bTheSameFile = TRUE;
 
			if(!bTheSameFile)
			{
				DeleteFile((char*)item.pBuffer);
				CopyFile(lpFileName, (char*)item.pBuffer, FALSE); 
			}			
		
			//__int64 nDataLen = (nFileLen  + ENCODE_LEN - 1) / ENCODE_LEN * ENCODE_LEN ;
			__int64 nDataLen = ((nFileLen + nAligned - 1) / nAligned) * nAligned;
			Get32BitLen(nFileLen, &itemTab.filelenLo, &itemTab.filelenHi);
			Get32BitLen(nDataLen, &itemTab.datalenLo, &itemTab.datalenHi);
			item.nLength = nDataLen;
 
			return 0;

		}
		else
		{
			free(item.pBuffer);
			item.pBuffer = (BYTE*)malloc(MAX_PATH);
			ZeroMemory(item.pBuffer, MAX_PATH);
		}
		TCHAR szTempFile [MAX_PATH] = {0};
#ifdef _WIN32
		TCHAR szTempPath [MAX_PATH];
		DWORD dwResult=::GetTempPath(MAX_PATH, szTempPath);
		
		UINT nResult=GetTempFileName(szTempPath, "~ex", 0, szTempFile);
#else
		sprintf(szTempFile, "./tmp_file_%s", gettimestr());
#endif
		if(CopyFile(lpFileName, szTempFile, FALSE) == FALSE)
		{
			TCHAR szInfo[MAX_PATH];
			sprintf(szInfo, ("#error Temp dir's space is not enough \r\n"));
			OutputDebugString(szInfo);
		}
		memcpy(item.pBuffer, szTempFile, strlen(szTempFile));
		//__int64 nDataLen = (nFileLen  + ENCODE_LEN - 1) / ENCODE_LEN * ENCODE_LEN ;
		__int64 nDataLen = ((nFileLen +  nAligned - 1) / nAligned ) * nAligned;
		item.nLength = nDataLen;
		Get32BitLen(nFileLen, &itemTab.filelenLo, &itemTab.filelenHi);
		Get32BitLen(nDataLen, &itemTab.datalenLo, &itemTab.datalenHi);
	}
	return 0;
}

LRESULT CImgMgmt::SetCallback(pOPCallback pFunf)
{
	_pCallbackFun = pFunf;
	return 0;
}

//////////////////////////////////////////////////////////////////////////
//
void  GetFileName(LPCTSTR lpFilePath, LPTSTR lpFille, DWORD dwLen)
//CString GetFileName(LPCTSTR lpFilePath)
{
	//CString csRet = lpFilePath;
	strcpy(lpFille, lpFilePath);
	int nLen = strlen(lpFilePath);
	int nIndex = nLen - 1;
	while(nIndex>=0)
	{
		if(lpFilePath[nIndex] == '\\' || lpFilePath[nIndex] == '/')
		{
			break;
		}
		nIndex--;
	}
	if(nIndex == -1)
		return ;

	strcpy(lpFille, lpFilePath + nIndex + 1);
	//csRet = csRet.Right(nLen - nIndex - 1);
	//return csRet;

}

typedef void* (*pGetInterface)(const char * plug_guid);


HANDLE FsOpenFile(LPCSTR lpFileName, BOOL bCreate)
{
#ifdef _WIN32
	SECURITY_ATTRIBUTES sa;
	ZeroMemory(&sa, sizeof(SECURITY_ATTRIBUTES));
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	DWORD dwCreatFlag = OPEN_EXISTING;
	if(bCreate)
		dwCreatFlag = CREATE_ALWAYS;
	HANDLE hFile = CreateFile(lpFileName, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_DELETE|FILE_SHARE_READ, &sa, dwCreatFlag, FILE_ATTRIBUTE_NORMAL, NULL);
	
	if(hFile == (HANDLE)-1)
		return NULL;
	return hFile;
#else
	HANDLE hFile = NULL;
        if (bCreate)
        {
            hFile = fopen(lpFileName, "wb+");
        }
        else
        {
            char command[1024] = {0};
            sprintf(command, "%s", "chmod 664 ");
            strcat(command, lpFileName);
            system(command);
            hFile = fopen(lpFileName, "rb+");
        }

	return hFile;
#endif
}


__int64 GetFileLength(const char * filename)
{
#ifdef _WIN32
	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(sa);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;
	HANDLE hd = CreateFile(filename, GENERIC_READ, FILE_SHARE_DELETE,&sa, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	// #ifdef  _DEBUG
	// 	DWORD len;
	// 	GetFileSize(hd, &len);
	// 	return len;
	// #else
	LARGE_INTEGER lgPt;
	GetFileSizeEx(hd,&lgPt);
	return lgPt.QuadPart;
#else
	HANDLE hd = fopen(filename, "rb");
	
	fseek(hd, 0, SEEK_END);
	__int64 nLen = ftello64(hd);
	fseek(hd, 0, SEEK_SET);
	return nLen;
#endif // _WIN32
	
//#endif
}

DWORD FsSeek(HANDLE hf, __int64 distance, DWORD dwMoveMethod )
{
	long lowLen = (long)(distance&0xffffffff);
	long highLen = (long)((distance&0xffffffff)>>32);
	
#ifndef _WIN32
	return fseeko64((FILE*)hf, distance, dwMoveMethod);
#else
	return SetFilePointer(hf, lowLen, &highLen, dwMoveMethod);
#endif
}

DWORD FsSeek_Gig(HANDLE hf, long distance, long distanceHigh, DWORD dwMoveMethod )
{
	long lowLen = distance;
	long highLen = distanceHigh;
#ifndef _WIN32
	return fseeko64((FILE*)hf, distance, dwMoveMethod);
#else
	return SetFilePointer(hf, lowLen, &highLen, dwMoveMethod);
#endif
}

__int64 Get64bitLen(u32 lLow, u32 lHi)
{
	__int64 nRet = lHi;
	nRet = nRet << 32;
	nRet |= lLow;
	if(nRet < 0)//?
		nRet = 0;
	return nRet;
}

static void Get32BitLen(__int64 nLen, u32* nLow, u32* nHi)
{
	*nLow = (u32)(nLen & 0x0000ffffffff);
	*nHi = (u32)((nLen & 0xffffffff00000000) >> 32);
}

LRESULT CImgMgmt::AddImgItem(HIMAGE hImage, LPCTSTR lpMainType, LPCTSTR lpSubType, LPCTSTR lpName, LPCTSTR lpFileName)
{ 
	int nIndex = GetItemIndex(hImage,lpMainType, lpSubType);

	if(nIndex != -1)
	{
		SetImgItem(hImage, nIndex, lpFileName);
		return 0;
	}

	IMAGE_HANDLE* pHandle = (IMAGE_HANDLE*)hImage;
	
	pHandle->_ImageHead.itemcount++;

	int nItemCnt = pHandle->_ImageHead.itemcount;
	ImageItem_t* pItemBuff = (ImageItem_t*)malloc(nItemCnt * sizeof(ImageItem_t));
	ImgMemItem* pItemMemBuff = (ImgMemItem*)malloc(nItemCnt* sizeof(ImgMemItem));
	memcpy(pItemMemBuff, pHandle->_pItemMem, sizeof(ImgMemItem) * (nItemCnt -1));
	memcpy(pItemBuff, pHandle->_pItemTable, sizeof(ImageItem_t) * (nItemCnt -1));

	free(pHandle->_pItemMem);
	free(pHandle->_pItemTable);

	pHandle->_pItemTable = pItemBuff;
	pHandle->_pItemMem = pItemMemBuff;


	ImgMemItem& item = pHandle->_pItemMem[nItemCnt -1];
	ImageItem_t& itemTab = pHandle->_pItemTable[nItemCnt - 1];
	ZeroMemory(&item, sizeof(ImgMemItem));
	ZeroMemory(&itemTab, sizeof(ImageItem_t));
		
	__int64 nFileLen = GetFileLength(lpFileName) ;
	__int64 dwBufferLen = (nFileLen + 511) / 512 * 512;

	if( nFileLen < SIZE_100M)
	{
#ifdef _WIN32
		SECURITY_ATTRIBUTES sa;
		sa.nLength = sizeof(sa);
		sa.lpSecurityDescriptor = NULL;
		sa.bInheritHandle = TRUE;
		HANDLE hd = CreateFile(lpFileName, GENERIC_READ, FILE_SHARE_DELETE,&sa, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
#else
		HANDLE hd = fopen(lpFileName, "rb");

#endif
	//	DWORD dwBufferLen = (nFileLen + 511) / 512 * 512;
		LPBYTE lpBuffer = new BYTE[dwBufferLen];
		memset(lpBuffer, 0x00, dwBufferLen);
		DWORD dwRead;
		ReadFile(hd, lpBuffer, nFileLen, &dwRead, NULL);
		CloseHandle(hd);		
		
		item.pBuffer = (BYTE*)malloc(dwBufferLen);
		memcpy(item.pBuffer, lpBuffer, dwBufferLen);		
	
	}	
	else
	{		
	
		TCHAR szTempPath [MAX_PATH];
		TCHAR szTempFile [MAX_PATH] = {0};

#ifdef _WIN32
		DWORD dwResult=::GetTempPath(MAX_PATH, szTempPath);
		UINT nResult=GetTempFileName(szTempPath, "~ex", 0, szTempFile);	
		sprintf(temp_path, "%s", szTempFile);
#else
		strcpy(szTempPath, "./");
		sprintf(szTempFile, "%s%s~ex", szTempPath, gettimestr()); //这就不允许Insert了，只能add 
#endif
		if(CopyFile( lpFileName, szTempFile,FALSE) == FALSE)
		{
			TCHAR szInfo[MAX_PATH];
			sprintf(szInfo, ("#error Temp dir's space is not enough \r\n"));
			OutputDebugString(szInfo);
		}
		item.pBuffer = (BYTE*)malloc(MAX_PATH);
		ZeroMemory(item.pBuffer, MAX_PATH);
		memcpy(item.pBuffer, szTempFile, strlen(szTempFile));
		Get32BitLen(nFileLen, &itemTab.filelenLo, &itemTab.filelenLo);
		item.nLength = nFileLen;
	//	itemTab.filelen = nFileLen;
	}

	strcpy(item.szName, lpName);
	memcpy(itemTab.name, lpName, strlen(lpName));
	
	item.nLength = dwBufferLen;
	//itemTab.filelen = nFileLen;
	//itemTab.datalen = dwBufferLen;
	Get32BitLen(nFileLen, &itemTab.filelenLo, &itemTab.filelenHi);
	Get32BitLen(dwBufferLen, &itemTab.datalenLo, &itemTab.datalenHi);
	
	u32 encode_data_len = ((dwBufferLen + ENCODE_LEN - 1) / ENCODE_LEN ) * ENCODE_LEN;
	
	
	memcpy(itemTab.subType, lpSubType, 16);
	memcpy(itemTab.mainType, lpMainType, 8);
	//	strcpy(itemTab.name, lpName);
	
	
	//Msg("[%d]%s len=%d\n", i, itemTab.name, length);
	itemTab.version = IMAGE_ITEM_VERSION;		//
	itemTab.size = sizeof(ImageItem_t);			//
	
	
	ImageItem_t& lastTab = pHandle->_pItemTable[nItemCnt - 2];
	__int64 nOffset = Get64bitLen(lastTab.offsetLo, lastTab.offsetHi);
		
	nOffset += ((encode_data_len + pHandle->_ImageHead.align - 1) / pHandle->_ImageHead.align) * pHandle->_ImageHead.align;
	
	Get32BitLen(nOffset, &itemTab.offsetLo, &itemTab.offsetHi);
	//itemTab.offset  = nOffset;					//文件起始位置偏移量
		
	itemTab.checksum= 0;		///校验和

#ifdef _WIN32
	u32 address = (u32)itemTab.res;
#else
	u32 address;
	memcpy(&address, itemTab.res, sizeof (u32));
#endif
	f32 *p = (f32 *)address;					///保留
	s8 f;
	for (int k = 0; k < IMAGE_ITEM_RCSIZE/4; k++)
	//for (int k = 0; k < 179; k++)		//171
	{
		f    = itemTab.subType[ min(0, k / 13)];
		f   *= (f32)(itemTab.filelenLo + itemTab.mainType[min(0, k / 25)]);
		p[k] = (f32)(f / ((f32 )k + 3.114));
	}
		


/*



	typedef struct tag_IMAGE_HANDLE
	{		
		HANDLE 				_hf;					//文件句柄
		ImageHead_t			_ImageHead;				//img头信息
		RC_ENDECODE_IF_t	_rc_if_decode[IF_CNT];	//解密接口
		ImageItem_t*		_pItemTable;			//item信息表
		ImgMemItem*			_pItemMem;				//item内存内容
}IMAGE_HANDLE;

*/
	return 0;
}


LRESULT CImgMgmt::GetItemMainSubType(HIMAGE hImage,int nIndex, LPSTR lpMainName, LPSTR lpSubName)
{
	IMAGE_HANDLE* _pImgHandle = (IMAGE_HANDLE*)hImage;

	if(nIndex >=_pImgHandle->_ImageHead.itemcount)
		return __LINE__;
	char szMain[9]= "";
	char szSub[17] = "";
	memcpy(szMain, _pImgHandle->_pItemTable[nIndex].mainType,8);
	memcpy(szSub, _pImgHandle->_pItemTable[nIndex].subType,16);
	strcpy(lpMainName, szMain);
	strcpy(lpSubName, szSub);
	return 0;

}

int		CImgMgmt::GetItemCnt(HIMAGE hImage)
{
	IMAGE_HANDLE* _pImgHandle = (IMAGE_HANDLE*)hImage;

	return _pImgHandle->_ImageHead.itemcount;

}
