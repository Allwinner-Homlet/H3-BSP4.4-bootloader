//////////////////////////////////////////////////////////////////////////
// Plugin vector interface header
//////////////////////////////////////////////////////////////////////////
// Sam Liu
// 2010-6-10
//////////////////////////////////////////////////////////////////////////

#ifndef __PLUGIN_VECTOR_HEADER__H__
#define __PLUGIN_VECTOR_HEADER__H__

#include "xcomplier_def.h"
#define AW_PLUGIN_VECTOR_NAME "AwPluginVector.dll"


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
	char szModualID[64];          // id
	char szModualName[MAX_PATH];  // 插件文件路径	  
}Plugin_info;

//////////////////////////////////////////////////////////////////////////
// 插件容器管理接口
typedef BOOL (*pPLoadPlugin)(Plugin_info& plg);
typedef void* (*pPQueryInterface)(const char* plgID);
typedef int (*pPGetVersion)();
typedef char* (*pPGetAuthor)();
typedef char* (*pPGetCopyRight)();
typedef char* (*pPGetPluginName)();
typedef void (*pFreeAllPlugin)();

typedef struct _tag_Plugin_Man_Fun
{
	pPLoadPlugin		LoadPlugin;
	pPQueryInterface	QueryInterface;
	pPGetVersion		GetVersion;
	pPGetAuthor			GetAuthor;
	pPGetCopyRight		GetCopyRight;
	pPGetPluginName		GetPluginName;
	pFreeAllPlugin		FreeAllPlugin;
}Plugin_Man_Fun; 


typedef Plugin_Man_Fun* (*pGetPluginMan)();

static Plugin_Man_Fun* LoadPluginCenter(LPCSTR lpPlgVector)
{
	HMODULE hMod = LoadLibrary(lpPlgVector);
//	HMODULE hMod = LoadLibrary(AW_PLUGIN_VECTOR_NAME); // Modified by Sam 2010-10-22 
//		HMODULE hMod = LoadLibrary("./AwPluginVector.dll");	

	if(hMod == NULL)
		return NULL;

	pGetPluginMan pFun;
	void *p= GetProcAddress(hMod, "GetPluginMan");
	pFun = (pGetPluginMan)p;

	return pFun();
}

static void InitilizePluginInfo(Plugin_info* plgInfo, const char* plgID, const char* szPathName)
{
	ZeroMemory(plgInfo, sizeof(Plugin_info));
	strcat(plgInfo->szModualID, plgID);
	strcat(plgInfo->szModualName, szPathName);
}

#endif   //__PLUGIN_VECTOR_HEADER__H__