// DosDebug.cpp: implementation of the CDosDebug class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "DosDebug.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDosDebug::CDosDebug()
{
	m_fStdOut = NULL;
    m_hStdOut = NULL;
}

CDosDebug::~CDosDebug()
{

}


//创建一个Console窗口，指明宽度和高度，如果fname不为空则同时将输出写入一个文件。
void CDosDebug::StartDosWin(char *fname, int width, int height)
{
#ifdef _DEBUG	
	if (!AllocConsole())//分配
	{
		::AfxMessageBox("创建控制台失败");
		return ;
	}
	
	SetConsoleTitle("调试跟踪窗口");

	m_hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);//指明句柄为标准输出HANDLE
	COORD co = {width,height};
	SetConsoleScreenBufferSize(m_hStdOut, co);//指明缓冲区大小

	if(fname)
		m_fStdOut = fopen(fname, "w");

#else
#ifdef RELEASE_OPEN
	if (!AllocConsole())//分配
	{
		::AfxMessageBox("创建控制台失败");
		return ;
	}
	
	SetConsoleTitle("调试跟踪窗口");

	m_hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);//指明句柄为标准输出HANDLE
	COORD co = {width,height};
	SetConsoleScreenBufferSize(m_hStdOut, co);//指明缓冲区大小

	if(fname)
		m_fStdOut = fopen(fname, "w");
#else
	//::AfxMessageBox("创建控制台版本release!");
#endif 
	
	return ;
#endif
}

BOOL CDosDebug::ReleaseConsole()
{
#ifdef _DEBUG
	return FreeConsole();
#else //_DEBUG
#ifdef RELEASE_OPEN
	return FreeConsole();
#else
	return TRUE;
#endif 
	
#endif //_DEBUG
}

int CDosDebug::Print(char *fmt, ... )
{
#ifdef _DEBUG
	char s[400];
	va_list argptr;
	int cnt;
	va_start(argptr, fmt);
	cnt = vsprintf(s, fmt, argptr);
	va_end(argptr);
	DWORD cCharsWritten;
	if(m_hStdOut)// 写Console
		WriteConsole(m_hStdOut, s, strlen(s), &cCharsWritten, NULL);

	if(m_fStdOut)
		fprintf(m_fStdOut, s);

	return(cnt);
#else
#ifdef RELEASE_OPEN
	char s[400];
	va_list argptr;
	int cnt;
	va_start(argptr, fmt);
	cnt = vsprintf(s, fmt, argptr);
	va_end(argptr);
	DWORD cCharsWritten;
	if(m_hStdOut)// 写Console
		WriteConsole(m_hStdOut, s, strlen(s), &cCharsWritten, NULL);

	if(m_fStdOut)
		fprintf(m_fStdOut, s);

	return(cnt);
#else
	return 0;
#endif 

#endif
}

int CDosDebug::Print(CString &msg)
{
#ifdef _DEBUG

	char s[400];	
	sprintf(s, "%s", msg);
	DWORD cCharsWritten;
	if(m_hStdOut)// 写Console
		WriteConsole(m_hStdOut, s, strlen(s), &cCharsWritten, NULL);

	if(m_fStdOut)
		fprintf(m_fStdOut, s);

	return(0);
#else

#ifdef RELEASE_OPEN
	char s[400];	
	sprintf(s, "%s", msg);
	DWORD cCharsWritten;
	if(m_hStdOut)// 写Console
		WriteConsole(m_hStdOut, s, strlen(s), &cCharsWritten, NULL);

	if(m_fStdOut)
		fprintf(m_fStdOut, s);

	return(0);
#else
	return 0;
#endif 

	
#endif
}

CString CDosDebug::GetErrorNumMsg(DWORD dwErrorNum)
{
	CString strErrorMessage;
	LPVOID lpMsgBuf;
	lpMsgBuf=LocalLock(LocalAlloc(LMEM_MOVEABLE|LMEM_ZEROINIT,1000));
	if(lpMsgBuf==NULL)
	{
		//exit(1);
		strErrorMessage = "内存分配失败！错误";
		return strErrorMessage;
	}
    
	SetLastError(dwErrorNum);
	FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER| 
		FORMAT_MESSAGE_FROM_SYSTEM| 
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dwErrorNum,
		MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT), // Default language
		(LPTSTR) &lpMsgBuf,
		0,
		NULL);
	strErrorMessage=(LPCTSTR)lpMsgBuf;
	if(strErrorMessage=="")
	{
		strErrorMessage="Undefined error code .";
	}
	
	LocalFree(lpMsgBuf);
	
	return strErrorMessage;

}
