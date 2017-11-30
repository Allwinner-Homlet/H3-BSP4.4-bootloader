// FileDirHelp.cpp: implementation of the CFileDirHelp class.




#include "stdafx.h"

#include "FileDirHelp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


#ifndef __FILE__DIR__HELP__CPP__
#define __FILE__DIR__HELP__CPP__
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFileDirHelp::CFileDirHelp()
{

}

CFileDirHelp::~CFileDirHelp()
{

}

//函数功能:测试文件是否存在
BOOL CFileDirHelp::IsFileExist(CString &csFilePath)
{
	if (csFilePath.IsEmpty())
		return FALSE;

	//测试文件是否存在
	CFile file;
	if (!file.Open(csFilePath, CFile::modeRead ))
		return FALSE;	
	else
		file.Close();

	return TRUE;
}
//函数功能:测试文件是否存在
BOOL CFileDirHelp::IsFileExist(char *szFilePath)
{
	if (szFilePath == NULL)
		return FALSE;

	//测试文件是否存在
	CFile file;
	if (!file.Open(szFilePath, CFile::modeRead ))
		return FALSE;
	else
		file.Close();

	return TRUE;
}

//函数功能：获取当前应用程序的路径
CString CFileDirHelp::GetModuleDirectory()
{
	//获取当前应用程序的路径
	TCHAR  szModuleDirectory[MAX_PATH];	
	memset(szModuleDirectory, 0, MAX_PATH);
    ::GetModuleFileName(NULL, szModuleDirectory, MAX_PATH);
	(strrchr(szModuleDirectory, '\\'))[1] = 0;

	CString homeDir;
	homeDir.Format("%s", szModuleDirectory);

	return homeDir; 
}

//函数功能：获取当前路径
CString CFileDirHelp::GetCurrentDirectory()
{
	TCHAR  szCurrentDirectory[MAX_PATH];	
	memset(szCurrentDirectory, 0, MAX_PATH);
    ::GetCurrentDirectory(MAX_PATH, szCurrentDirectory);
	(strrchr(szCurrentDirectory, '\\'))[1] = 0;

	CString currentdir;
	currentdir.Format("%s", szCurrentDirectory);

	return currentdir; 
}
//函数功能：获取系统路径
CString CFileDirHelp::GetSystemDirectory()
{
	TCHAR  szSystemDirectory[MAX_PATH];	
	memset(szSystemDirectory, 0, MAX_PATH);
    ::GetSystemDirectory(szSystemDirectory, MAX_PATH);
	(strrchr(szSystemDirectory, '\\'))[1] = 0;

	CString systemdir;
	systemdir.Format("%s", szSystemDirectory);

	return systemdir; 
}

#endif //__FILE__DIR__HELP__CPP__