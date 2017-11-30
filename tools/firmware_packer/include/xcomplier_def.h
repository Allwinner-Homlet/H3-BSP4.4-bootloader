//----------------------------------------------------------------------------------------------------------//
//                                                                                                          //
//                                           eStudio System                                            			//
//                                                                                                          //
//                              	 (c) Copyright 2010-2018, Sam China                                     	//
//                                                                                                          //
//                                           All Rights Reserved                                            //
//                                                                                                          //
// File    : xcomplier_def.h                                                                                //
// By      : Sam                                                                                        		//
// Version : V1.00                                                                                          //
// Time    : 2010-12-03																																											//
//----------------------------------------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------------------------------------//
//                                                                                                          //
// HISTORY                                                                                                  //
//                                                                                                          //
// 1 2010-12-03 10:05:00                                                                                    //
//                                                                                                          //
//                                                                                                          //
//                                                                                                          //
//----------------------------------------------------------------------------------------------------------//
#ifndef _X_COMPLIER_DEF_H__HEADER_
#define _X_COMPLIER_DEF_H__HEADER_
/*************************************************************/ 
/*define for Linux
**************************************************************/
#ifndef _WINDOWS

//------------------------------------------------//
// macro
//-----------------------------------------------//
#define MAX_PATH              260
#define TRUE                  1
#define FALSE                 0
#define INVALID_HANDLE_VALUE -1
#define MB_OK                 0
#define IN
#define OUT

/*************************************************/
// varant
/*************************************************/
typedef const char    *LPCSTR, *PCSTR;
typedef const char    *LPCTSTR, *PCTSTR;
typedef char          *LPTSTR, *PTSTR;
typedef char          *LPSTR, *PSTR;
typedef bool           BOOL;
typedef void          *HMODULE;
typedef void          *HINSTANCE, *LPVOID;
typedef unsigned int   UINT;
typedef unsigned int   DWORD,*LPDWORD;
typedef DWORD *PDWORD;
typedef wchar_t        WCHAR;
typedef unsigned char *LPBYTE, BYTE;
typedef long            HANDLE;
typedef unsigned char  byte;
typedef byte* PBYTE;

typedef unsigned short int WORD;
typedef int			   LRESULT;
typedef long		   HRESULT;
typedef unsigned long long 	__u64;
typedef unsigned long long	u64;
typedef unsigned long long	uint64;
typedef __u64 LONGLONG;
typedef long LONG;
typedef long* PLONG;
typedef const void* LPCVOID;
typedef void* LPVOID;
typedef __u64 ULONG64;
typedef int INT;
typedef unsigned int u32;
typedef unsigned short u16;
typedef byte u8;
typedef int s32;

typedef struct _SECURITY_ATTRIBUTES
{
	DWORD nLength;
	/* [size_is] */ LPVOID lpSecurityDescriptor;
	BOOL bInheritHandle;
} SECURITY_ATTRIBUTES;

typedef struct _SECURITY_ATTRIBUTES *PSECURITY_ATTRIBUTES;

typedef struct _SECURITY_ATTRIBUTES *LPSECURITY_ATTRIBUTES;

typedef char CHAR;
typedef struct _SYSTEMTIME
    {
    WORD wYear;
    WORD wMonth;
    WORD wDayOfWeek;
    WORD wDay;
    WORD wHour;
    WORD wMinute;
    WORD wSecond;
    WORD wMilliseconds;
    } 	SYSTEMTIME;

typedef struct _SYSTEMTIME *PSYSTEMTIME;

typedef struct _SYSTEMTIME *LPSYSTEMTIME;

typedef struct _FILETIME {
    DWORD dwLowDateTime;
    DWORD dwHighDateTime;
} FILETIME, *PFILETIME, *LPFILETIME;

typedef struct _WIN32_FIND_DATA {
    DWORD dwFileAttributes;
    FILETIME ftCreationTime;
    FILETIME ftLastAccessTime;
    FILETIME ftLastWriteTime;
    DWORD nFileSizeHigh;
    DWORD nFileSizeLow;
    DWORD dwReserved0;
    DWORD dwReserved1;
    CHAR   cFileName[ MAX_PATH ];
    CHAR   cAlternateFileName[ 14 ];
#ifdef _MAC
    DWORD dwFileType;
    DWORD dwCreatorType;
    WORD  wFinderFlags;
#endif
} WIN32_FIND_DATA, *PWIN32_FIND_DATA, *LPWIN32_FIND_DATA;

typedef struct _OVERLAPPED {
    __u64 Internal;
    __u64 InternalHigh;
    union {
        struct {
            DWORD Offset;
            DWORD OffsetHigh;
        };

        void* Pointer;
    };

    HANDLE  hEvent;
} OVERLAPPED, *LPOVERLAPPED;




/*************************************************************/
// Function
/************************************************************/
#include <dlfcn.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
//#include <iostream>

typedef off64_t __int64;
//---------------------------------------------------------------//
// max min
//---------------------------------------------------------------//

#define max(a,b)    (((a) > (b)) ? (a) : (b))
#define min(a,b)    (((a) < (b)) ? (a) : (b))

//---------------------------------------------------------------//
// 输入输出
//---------------------------------------------------------------//
static void DebugFunction(bool bFlag, const char* str)
{
		if(bFlag == FALSE)
					return;
		else
			printf("%s", str);
}

#define AfxMessageBox(...)      (printf(__VA_ARGS__))
#define TRACE(...)              (printf(__VA_ARGS__))
#define OutputDebugString(...)  (printf(__VA_ARGS__))
#define Msg(...)                (printf(__VA_ARGS__))

static void MessageBox(void* hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType)
{
		printf("%s\n", lpText);
}

//-----------------------------------------------------------// 
//  Library Operator
//----------------------------------------------------------//
#define __declspec(dllexport)
                     
#define GetProcAddress dlsym
//#define ::GetProcAddress dlsym
static HMODULE LoadLibrary(const char *lpFileName)
{
	HMODULE hMod;
	hMod = dlopen(lpFileName, RTLD_LAZY);
	if(hMod == NULL)
		printf("Error %s", dlerror());
	return hMod;
}
static int FreeLibrary(HMODULE hModule)
{
	return dlclose(hModule);
}

//---------------------------------------------------------//
//   memory
//---------------------------------------------------------//
static void ZeroMemory(void *Buf, unsigned int nSize)
{
	memset(Buf, 0, nSize);
}

//---------------------------------------------------------//
// 字符串处理
//---------------------------------------------------------//
#define _T                                          //宽字符
#define _tcschr strchr                              //返回指向某字符指针
#define _tcsstr strstr                              //返回字符串指针
#define _TUCHAR unsigned char
#define _ttoi atoi                                  //转换成int
#define _istdigit isdigit                           
#define _stprintf sprintf                           //格式化
#define lstrlen _tcslen
#define _tcslen strlen                              //求长度
#define _vstprintf vsprintf
#define _tcscpy strcpy
#define _tcscmp strcmp
#define _tcscat strcat
#define _getcwd getcwd                              
#define TCHAR char
#define stricmp strcasecmp                          //忽略大小写的比较

static unsigned int _tclen(const char *_cpc)        //求字符长度
{
		return (_cpc,1); 
}
static char* _tcsinc(const char *current)           //求下一个字符指针
{
		return (char*)current+1;
}	

static char* strlwr(char *str)
{
    char *p=str;
    while(*p)
    {
        *p = tolower(*p);
        p++;
    }
    return str;
}


static char* strupr(char *str)
{
	  char *p = str;
		while(*p)
		{
			  *p = toupper(*p);
			  p++;
		}
		return str;
}

//------------------------------------------------------------//
// File Operator
//-------------------------------------------------------------//
#define GENERIC_READ                     (0x80000000L)
#define GENERIC_WRITE                    (0x40000000L)
#define GENERIC_EXECUTE                  (0x20000000L)
#define GENERIC_ALL                      (0x10000000L)

#define CREATE_NEW          1
#define CREATE_ALWAYS       2
#define OPEN_EXISTING       3
#define OPEN_ALWAYS         4
#define TRUNCATE_EXISTING   5

#define FILE_SHARE_READ                 0x00000001  
#define FILE_SHARE_WRITE                0x00000002  
#define FILE_SHARE_DELETE               0x00000004  
#define FILE_ATTRIBUTE_NORMAL           0x00000080  

/*
static void GetCurrentDirectory(int nLen, char *szPath)
{
		getcwd(szPath, nLen);
		return;
}

static int SetCurrentDirectory(const char *szPath)
{
		return chdir(szPath);
}
*/

#define GetCurrentDirectory(nLen, szPath) (getcwd(szPath, nLen))
#define SetCurrentDirectory(szPath)       (chdir(szPath))

static unsigned int GetModuleFileName(void* hInstance, char* szPath, int nLen)  //获取进程路径
{
		unsigned int nRet;
		
		nRet = readlink("/proc/self/exe", szPath, nLen);
		if(nRet<0 || nRet >= nLen)
		{
				printf("failed\n");
				return 0;
		}
		szPath[nLen]='\0';
		
		return nLen;
}

static void GetModuleDirectory(char *szPath)        //获取进程所在目录
{
	    char szExePath[MAX_PATH];
		memset(szExePath, 0, MAX_PATH);
		GetModuleFileName(NULL, szExePath, MAX_PATH);
		*(strrchr(szExePath, '/')) = 0;
		strncpy(szPath, szExePath, strlen(szExePath)+1);
		return;
}

static __int64 GetFileLength(signed char *filename)
{
		/*FILE *fp = NULL;
		fp = fopen((const char *)filename, "rb");
		if (fp == NULL)
		{
			return 0;
		}
//		long position = ftell(fp);
		long position = 0;
		fseek(fp, 0, SEEK_END);
		unsigned int len = (unsigned int)ftell(fp) - position;
		fclose(fp);
		*/
		__int64 nRet = 0;
	    struct stat buf;  
		if(stat((const char*)filename, &buf)<0)  
		{  
			return 0;  
		}  
		nRet  =buf.st_size; 
	//	printf("%s Len: 0x%x\n", filename, nRet);

		return nRet;
}

static HANDLE CreateFile(LPCTSTR lpFileName, DWORD dwAccess, 
		DWORD dwShareMode, void* lpSa, DWORD dwCreation, 
		DWORD dwFlag, HANDLE hTemplate)
{
	HANDLE hFile = -1;
	printf("FilePath: %s\n", lpFileName);
	if(dwCreation == OPEN_EXISTING)  //open_exsiting
	{
		if(dwAccess == GENERIC_READ)
			hFile = (HANDLE)open(lpFileName, O_RDONLY);
		else if(dwAccess == GENERIC_WRITE)
			hFile =  (HANDLE)open(lpFileName, O_WRONLY);
		else if(dwAccess == GENERIC_WRITE | GENERIC_READ)
			hFile =  (HANDLE)open(lpFileName, O_RDWR);
		else if(dwAccess == GENERIC_ALL)
			hFile =  (HANDLE)open(lpFileName, O_RDWR);
	}
	else if(dwCreation == 1) //create_new
	{

	}
	else if(dwCreation == 2) //create_always
	{

	}
	else if(dwCreation == 4) //open_always
	{

	}

	return hFile;
}

static DWORD GetFileSize(HANDLE hFile,LPDWORD lpSizeHigh)
{
		struct stat buf;
		if(fstat(hFile, &buf) < 0)
		{
				*lpSizeHigh = 0;
				return -1;
		}
		*lpSizeHigh = 0;
		return buf.st_size;
}

static BOOL WriteFile(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped )
{
	int nRet = write((int)hFile, lpBuffer, nNumberOfBytesToWrite);
	if(nRet == -1)
	{

		printf("The File Handle = %ld err = %s \n", hFile, strerror(errno));
		*lpNumberOfBytesWritten = 0;
		return 0;
	}
	else
	{

		*lpNumberOfBytesWritten = nRet;
		return 1;		
	}
}

static BOOL ReadFile(HANDLE hFile, LPVOID lpBuffer, DWORD nBytesToRead,
										LPDWORD lpBytesRead, void *lpOverlapped)
{
			int nRet = read((int)hFile, lpBuffer, nBytesToRead);
			if(nRet == -1)
			{
					*lpBytesRead = 0;
					return 0;
			}
			else
			{
					*lpBytesRead = nRet;
					return 1;		
			}
}
											    
static void CloseHandle(HANDLE hFile)
{
		close((int)hFile);
		return;
}

static DWORD GetLastError()
{
		return errno;
}
// -----------------------------------------------------//
// Others
//------------------------------------------------------//


// -----------------------------------------------------//
// temp file
//------------------------------------------------------//
static unsigned int GetTempPath(unsigned int nBufferLength, char* lpBuffer)
{
		strcpy(lpBuffer, "./");
}

static unsigned int GetTempFileName(const char* lpPathName, const char* lpPrefixString, 
														unsigned int uUnique, char* lpTempFileName)
{
		strcpy(lpTempFileName, lpPathName);
		strcat(lpTempFileName, "/");
		strcat(lpTempFileName, lpPrefixString);
		strcat(lpTempFileName, ".tmp");
		
		creat(lpTempFileName, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH);
}

static  BOOL SystemTimeToFileTime( const SYSTEMTIME *lpSystemTime, LPFILETIME lpFileTime)
{
    DWORD dwLowDateTime;
    DWORD dwHighDateTime;

	int y,m,d,h, minute, sec;
	
	WORD tem;
	
	WORD date;
	WORD dtm;

	
	y = lpSystemTime->wYear - 1980;// .GetYear() - 1980;
	
	m = lpSystemTime->wMonth;// .GetMonth();
	//rw.DIR_WrtDate&0x00f0)>>4;
	d = lpSystemTime->wDay;// .GetDay();
	//rw.DIR_WrtDate&0x000f;
	h = lpSystemTime->wHour;// .GetHour();
	//rw.DIR_WrtTime&0xf800)>>8;
	minute = lpSystemTime->wMinute;// .GetMinute();
	//rw.DIR_WrtTime&0x07e0)>>5;
	sec = lpSystemTime->wSecond;// .GetSecond();
	//rw.DIR_WrtTime&0x001f;

	date = d;
	tem = m;
	tem = tem<<5;
	date = date | tem;
	
	tem = y;
	tem = tem<<9;
	date =date | tem;

	dtm = sec;
	tem = minute;
	tem = tem<<5;
	dtm = dtm | tem;

	tem = h;
	tem  = tem<<11;
	dtm = dtm | tem;

	memcpy(&lpFileTime->dwHighDateTime, &date, 2);
	memcpy(&lpFileTime->dwLowDateTime, &dtm, 2);

	return TRUE;
}

static BOOL LocalFileTimeToFileTime(const FILETIME *lpLocalFileTime,LPFILETIME lpFileTime)
{
	memcpy(lpFileTime, lpLocalFileTime, sizeof(FILETIME));
	return TRUE;
}

static  BOOL CreateDirectory(LPCSTR lpPathName,void* pParam)
{
	int status = mkdir(lpPathName, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	if(status == -1)
	{
		printf("CreateDirectory :%s Failed.\n", lpPathName);
		perror("CreateDir Error");
		return FALSE;
	}
	else
		return TRUE;
}

// confirm the path file path 
static void VerifyLinuxPathFormat(LPTSTR lpPath)
{
	int nLength = strlen(lpPath);
	int nPos = 0;
	while(nPos < nLength)
	{
		if(lpPath[nPos] == '\\')
		{
			lpPath[nPos] = '/';
		}
		nPos ++;
	}

}

/***********************************************************/
//  Class
/**********************************************************/
#include "BCString.h"
typedef CBCString CString;

#endif //_WINDOWS

/****************************************************************/

#endif //_X_COMPLIER_DEF_H__HEADER_
