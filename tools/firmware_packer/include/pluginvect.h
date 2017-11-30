//////////////////////////////////////////////////////////////////////////
// Plugin vector interface header
//////////////////////////////////////////////////////////////////////////
// Sam Liu
// 2010-6-10
//////////////////////////////////////////////////////////////////////////

#ifndef __PLUGIN_VECTOR_HEADER__H__
#define __PLUGIN_VECTOR_HEADER__H__
//#include "TCHAR.H"
#include "xcomplier_def.h"
#define PLUGIN_VECTOR_NAME _T("plgvector.dll")

#ifndef MAX_PATH
#define MAX_PATH	260								//
#endif //MAX_PATH
//////////////////////////////////////////////////////////////////////////
// struct for loading plugin
// 调用步骤
// 1. 初始化Plugin_info
// 2. 调用 LoadPluginCenter返回插件管理器接口，类型为Plugin_Man_Fun
// 2. 调用 Plugin_Man_Fun 的 LoadPlugin 加载插件
// 3. 通过 Plugin_Man_Fun 的 QueryInterface获取相应的接口
//////////////////////////////////////////////////////////////////////////
typedef struct _tag_Plugin_info
{	  
	TCHAR szModualID[64];          // id
	TCHAR szModualName[MAX_PATH];  // 插件文件路径	  
}Plugin_info;

typedef struct _tag_Plugin_Version
{
	TCHAR szAuthor[32];
	TCHAR szPlgName[32];
	TCHAR szCopyRight[256];
	UINT  nVersion;
}Plugin_Version;

//////////////////////////////////////////////////////////////////////////
// 插件容器管理接口
typedef BOOL (*pPLoadPlugin)(Plugin_info& plg);
typedef void* (*pPQueryInterface)(LPCTSTR plgID);
typedef UINT (*pPGetVersion)();
typedef TCHAR* (*pPGetAuthor)();
typedef TCHAR* (*pPGetCopyRight)();
typedef TCHAR* (*pPGetPluginName)();
typedef void  (*pFreePlg)(LPCTSTR lpPlgID);

typedef struct _tag_Plugin_Man_Fun
{
	pPLoadPlugin		LoadPlugin;
	pPQueryInterface	QueryInterface;
	pPGetVersion		GetVersion;
	pPGetAuthor			GetAuthor;
	pPGetCopyRight		GetCopyRight;
	pPGetPluginName		GetPluginName;
	pFreePlg			FreePlg;
}Plugin_Man_Fun; 

typedef Plugin_Man_Fun* (*pGetPluginMan)();


/***********************************************************************/
// int GetParentPath(LPTSTR pszpath) 
// 返回父目录路径  	                                                                   
/************************************************************************/
static int GetParentPath(LPTSTR pszpath) 
{
	int i;

	int pathLenght = _tcslen(pszpath);


	for(i = pathLenght; i>0; i--)
	{
		if(pszpath[i] =='/')
		{

			pszpath[i]	= 0;
			break;
		}
		else
			pszpath[i]	= 0;
	}
	return i;//the length of new string
}
//////////////////////////////////////////////////////////////////////////

/***********************************************************************
// int GetAbsPathName(LPCTSTR pszfilename) 
// 返回相对于当前运行的绝对路径  	                                                                   
/************************************************************************/
static int GetAbsPathName(LPCTSTR pszfilename, LPTSTR lpRet)
{
	TCHAR pName[MAX_PATH] = _T("");
	int		iCount;
	iCount	= GetModuleFileName(NULL,pName,MAX_PATH);
	iCount = GetParentPath(pName);
	_tcscat(pName, _T("/"));
	_tcscat(pName, pszfilename);
	_tcscpy(lpRet, pName);	

	iCount = _tcslen(pName);
	return iCount;
}

static Plugin_Man_Fun* LoadPluginCenter()
{
	TCHAR szPath[MAX_PATH] = _T("");
	GetAbsPathName(PLUGIN_VECTOR_NAME, szPath);
	//OutputDebugString(szPath);
	HMODULE hMod = LoadLibrary(szPath);
	if(hMod == NULL)
	{
		OutputDebugString(szPath);
		OutputDebugString(_T("hMod == NULL\r\n"));
	//	MessageBox(szPath);
		return NULL;
	}
	pGetPluginMan pFun = (pGetPluginMan)GetProcAddress(hMod, _T("GetPluginMan"));
	return pFun();
}

static void InitPluginInfo(Plugin_info* plgInfo, LPCTSTR plgID, LPCTSTR szPathName)
{
	TCHAR szPath[MAX_PATH] = _T("");
	GetAbsPathName(szPathName, szPath);
	ZeroMemory(plgInfo, sizeof(Plugin_info));
	_tcscat(plgInfo->szModualID, plgID);
	_tcscat(plgInfo->szModualName, szPath);
}
#endif