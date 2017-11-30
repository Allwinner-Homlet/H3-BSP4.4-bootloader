//----------------------------------------------------------------------------------------------------------//
//                                                                                                          //
//                                                                                                          //
//                                                                                                          //
//                                                                                                          //
//                                                                                                          //
//                                                                                                          //
//                                                                                                          //
//                                                                                                          //
//                                                                                                          //
//                                                                                                          //
//    I BBMMBB      BBMMBBI     BBI EEMMFFI BBMMI     BBI EEMMFFI BBMMI       BBMMBBI   EEMMI EEMMLL        //
//    EEI   EEFF  EEI   LLBB  EEMMBBI I BBEE  I EE  EEMMBBI I BBEE  I EE    EEI   LLBB    EEBBI I BB        //
//  I FF        I EE      BBI   EEI     EEI   I EE    EEI     EEI   I EE  I EE      BBI   EEI   I EE        //
//  LLFF        LLEE      EELL  EEI     EEI   I EE    EEI     EEI   I EE  LLEE      EELL  EEI   I EE        //
//  I EE      I I BB      EEI   EEI     EEI   I EE    EEI     EEI   I EE  I BB      EEI   EEI   I EE        //
//    BBLL  I I   BBFF  I EE    EEI     EEI   I BB    EEI     EEI   I BB    BBFF  I EE    BBI   I EE        //
//    I BBMMEE    I BBMMBB    EEMMMMLLBBMMBBLLMMMMEEEEMMMMLLBBMMBBLLMMMMEE  I BBMMBB    EEMMMMLLBBMMBB      //
//                                                                                                          //
//                                                                                                          //
//                                                                                                          //
//                                                                                                          //
//                                                                                                          //
//                                                                                                          //
//                                                                                                          //
//                                                                                                          //
//----------------------------------------------------------------------------------------------------------//
//                                                                                                          //
//                                                Scott System                                              //
//                                                                                                          //
//                               (c) Copyright 2006-2007, Scottyu China                                     //
//                                                                                                          //
//                                           All Rights Reserved                                            //
//                                                                                                          //
//                                                                                                          //
// File    : PlugInHelp.cpp                                                                                 //
// By      : scottyu                                                                                        //
// Version : V1.00                                                                                          //
// Time    : 2008-04-14 13:32:00                                                                            //
//                                                                                                          //
//----------------------------------------------------------------------------------------------------------//
//                                                                                                          //
// HISTORY                                                                                                  //
//                                                                                                          //
// 1 2008-04-14 13:32:00                                                                                    //
//                                                                                                          //
//                                                                                                          //
//                                                                                                          //
//----------------------------------------------------------------------------------------------------------//


#ifndef __PLUGIN_HELP__C____
#define __PLUGIN_HELP__C____	1


#include "PlugInHelp.h"
#include "error.h"
#include "xcomplier_def.h"

#define ERR_LEN						128


//------------------------------------------------------------------------------------------------------------
//
// 函数说明
//     加载一个插件
//
// 参数说明
//     
//
// 返回值
//     
//
// 其他
//    无
//
//------------------------------------------------------------------------------------------------------------
Interface_t *LoadPlugIn(const char * Type, char * szPlugFileName)
{
	Interface_t * Interface = NULL;
	char szErr[ERR_LEN] = "";
	Interface = new Interface_t;
	memset(Interface, 0, sizeof(Interface_t));
	memcpy(Interface->Type, Type, strlen(Type));

	Interface->hModule = NULL;
	Interface->hModule = ::LoadLibrary(szPlugFileName);
	if (Interface->hModule == NULL)
	{
		u32 err = ::GetLastError();
		delete Interface;
		Interface = NULL;
//		Err("LoadPlugIn", __FILE__, __LINE__, "LoadLibrary %s 失败 %d", szPlugFileName, err);
		return NULL;
	}
	
	//公共接口
	Interface->Common.GetPlugInType = 	(pGetPlugInType)::GetProcAddress(Interface->hModule, "GetPlugInType");
	Interface->Common.GetPlugInName = 	(pGetPlugInName)::GetProcAddress(Interface->hModule, "GetPlugInName");
	Interface->Common.GetPlugInVersion = (pGetPlugInVersion)::GetProcAddress(Interface->hModule, "GetPlugInVersion");
	Interface->Common.GetAuthor     = 	(pGetAuthor)::GetProcAddress(Interface->hModule, "GetAuthor");
	Interface->Common.GetCopyRight  =   (pGetCopyRight)::GetProcAddress(Interface->hModule, "GetCopyRight");
	/*
	if (Interface->Common.GetPlugInType == NULL)
	{
		Err("LoadPlugIn", __FILE__, __LINE__, "插件通用接口失败 GetPlugInType 失败");
		goto LoadPlugInError;
	}

	if (Interface->Common.GetPlugInName == NULL)
	{
		Err("LoadPlugIn", __FILE__, __LINE__, "插件通用接口失败 GetPlugInName 失败");
		goto LoadPlugInError;
	}

	if (Interface->Common.GetPlugInVersion == NULL)
	{
		Err("LoadPlugIn", __FILE__, __LINE__, "插件通用接口失败 GetPlugInVersion 失败");
		goto LoadPlugInError;
	}

	if (Interface->Common.GetAuthor == NULL)
	{
		Err("LoadPlugIn", __FILE__, __LINE__, "插件通用接口失败 GetAuthor 失败");
		goto LoadPlugInError;
	}

	if (Interface->Common.GetCopyRight == NULL)
	{
		Err("LoadPlugIn", __FILE__, __LINE__, "插件通用接口失败 GetCopyRight 失败");
		goto LoadPlugInError;
	}
	*/
	if (Interface->Common.GetPlugInType 	== NULL ||
		Interface->Common.GetPlugInName 	== NULL ||
		Interface->Common.GetPlugInVersion 	== NULL ||
		Interface->Common.GetAuthor 		== NULL ||
		Interface->Common.GetCopyRight 		== NULL)
	{
//		Err("LoadPlugIn", __FILE__, __LINE__, "插件通用接口失败 %s 失败", szPlugFileName);
		goto LoadPlugInError;
	}

	
	if (strcmp((const char *)Interface->Common.GetPlugInType(), Type) != 0)
	{			
//		Err("LoadPlugIn", __FILE__, __LINE__, "插件类型 %s 失败", Type);
		goto LoadPlugInError;
	}
/*
	if (strcmp(Type, FEX_INTERFACE) == 0)//专用接口
	{					
		Interface->Fex.Fex_Open 	= (pFex_Open)::GetProcAddress(Interface->hModule, "Fex_Open");
		Interface->Fex.Fex_Close 	= (pFex_Close)::GetProcAddress(Interface->hModule, "Fex_Close");
		Interface->Fex.Fex_Query 	= (pFex_Query)::GetProcAddress(Interface->hModule, "Fex_Query");
		Interface->Fex.Fex_Send 	= (pFex_Send)::GetProcAddress(Interface->hModule, "Fex_Send");
		Interface->Fex.Fex_Recv 	= (pFex_Recv)::GetProcAddress(Interface->hModule, "Fex_Recv");
		Interface->Fex.Fex_transmit_receive = (pFex_transmit_receive)::GetProcAddress(Interface->hModule, "Fex_transmit_receive");
		Interface->Fex.Fex_command 	= (pFex_command)::GetProcAddress(Interface->hModule, "Fex_command");
			
		if (Interface->Fex.Fex_Open 	== NULL || 
			Interface->Fex.Fex_Close 	== NULL ||
			Interface->Fex.Fex_Query 	== NULL ||
			Interface->Fex.Fex_Send 	== NULL || 
			Interface->Fex.Fex_Recv 	== NULL || 
			Interface->Fex.Fex_transmit_receive == NULL ||
			Interface->Fex.Fex_command 	== NULL)
		{
			Err("LoadPlugIn", __FILE__, __LINE__, "插件 %s 失败", FEX_INTERFACE);
			goto LoadPlugInError;
		}
	}

	else if (strcmp(Type, FES_INTERFACE) == 0)//专用接口
	{		
		Interface->Fes.Fes_Open 		= (pFes_Open)::GetProcAddress(Interface->hModule, "Fes_Open");
		Interface->Fes.Fes_Elf2Fes 		= (pFes_Elf2Fes)::GetProcAddress(Interface->hModule, "Fes_Elf2Fes");
		Interface->Fes.Fes_GetBlock 	= (pFes_GetBlock)::GetProcAddress(Interface->hModule, "Fes_GetBlock");
		Interface->Fes.Fes_EntryPoint 	= (pFes_EntryPoint)::GetProcAddress(Interface->hModule, "Fes_EntryPoint");
		Interface->Fes.Fes_Close 		= (pFes_Close)::GetProcAddress(Interface->hModule, "Fes_Close");	
				
		if (Interface->Fes.Fes_Open 		== NULL || 	
			Interface->Fes.Fes_Elf2Fes 		== NULL ||
			Interface->Fes.Fes_GetBlock 	== NULL || 
			Interface->Fes.Fes_EntryPoint 	== NULL ||
			Interface->Fes.Fes_Close 		== NULL )
		{
			Err("LoadPlugIn", __FILE__, __LINE__, "插件 %s 失败", FES_INTERFACE);
			goto LoadPlugInError;
		}
	}

	else if (strcmp(Type, ELF_INTERFACE) == 0)//专用接口
	{		
		Interface->Elf.Elf_Create 	= (pElf_Create)::GetProcAddress(Interface->hModule, "Elf_Create");
		Interface->Elf.Elf_Destory 	= (pElf_Destory)::GetProcAddress(Interface->hModule, "Elf_Destory");
		if (Interface->Elf.Elf_Create == NULL ||  Interface->Elf.Elf_Destory == NULL)
		{
			Err("LoadPlugIn", __FILE__, __LINE__, "插件 %s 失败", ELF_INTERFACE);
			goto LoadPlugInError;
		}
	}
	else if (strcmp(Type, TOOL_INTERFACE) == 0)//专用接口
	{		
		Interface->Tool.SetInterface= (pSetInterface)::GetProcAddress(Interface->hModule, "SetInterface");
		Interface->Tool.SetImage	= (pSetImage)::GetProcAddress(Interface->hModule, "SetImage");
		Interface->Tool.ToolEntry	= (pToolEntry)::GetProcAddress(Interface->hModule, "ToolEntry");
		if (!Interface->Tool.SetInterface  || 
			!Interface->Tool.SetImage ||
			!Interface->Tool.ToolEntry )
		{
			Err("LoadPlugIn", __FILE__, __LINE__, "插件 %s 失败", TOOL_INTERFACE);
			goto LoadPlugInError;
		}
	}
	else if (strcmp(Type, DATACHECK_INTERFACE) == 0)//专用接口
	{		
		Interface->DataCheck.Initial    	= (pInitial)::GetProcAddress(Interface->hModule, "Initial");
		    Interface->DataCheck.UnInitial  = (pInitial)::GetProcAddress(Interface->hModule, "UnInitial");
		Interface->DataCheck.VerifyData 	= (pVerifyData)::GetProcAddress(Interface->hModule, "VerifyData");
		Interface->DataCheck.VerifyFile 	= (pVerifyFile)::GetProcAddress(Interface->hModule, "VerifyFile");
			
		if (Interface->DataCheck.Initial    == NULL || 
		    Interface->DataCheck.UnInitial  == NULL || 
			Interface->DataCheck.VerifyData == NULL || 
			Interface->DataCheck.VerifyFile == NULL )
		{
			Err("LoadPlugIn", __FILE__, __LINE__, "插件 %s 失败", DATACHECK_INTERFACE);
			goto LoadPlugInError;
		}
	}	
	else */if (strcmp(Type, ENGINE_INTERFACE) == 0)//专用接口
	{
		Interface->Engine.Engine_Open 		= (pEngine_Open)::GetProcAddress(Interface->hModule, "Engine_Open");
		Interface->Engine.Engine_Close		= (pEngine_Close)::GetProcAddress(Interface->hModule, "Engine_Close");
		Interface->Engine.Engine_CloseAll	= (pEngine_CloseAll)::GetProcAddress(Interface->hModule, "Engine_CloseAll");
		Interface->Engine.Engine_Run		= (pEngine_Run)::GetProcAddress(Interface->hModule, "Engine_Run");
		Interface->Engine.GetGlobalString	= (pGetGlobalString)::GetProcAddress(Interface->hModule, "GetGlobalString");
		Interface->Engine.GetGlobalNumber	= (pGetGlobalNumber)::GetProcAddress(Interface->hModule, "GetGlobalNumber");
		Interface->Engine.GetTableString	= (pGetTableString)::GetProcAddress(Interface->hModule, "GetTableString");
		Interface->Engine.GetTableNumber	= (pGetTableNumber)::GetProcAddress(Interface->hModule, "GetTableNumber");
		Interface->Engine.GetTableSubtableString= 
			(pGetTableSubtableString)::GetProcAddress(Interface->hModule, "GetTableSubtableString");
		Interface->Engine.GetTableSubtableNumber= 
			(pGetTableSubtableNumber)::GetProcAddress(Interface->hModule, "GetTableSubtableNumber");
		Interface->Engine.Register			= (pRegister)::GetProcAddress(Interface->hModule, "Register");
		Interface->Engine.l_setpath			= (pl_setpath)::GetProcAddress(Interface->hModule, "l_setpath");
		Interface->Engine.l_run				= (pl_run)::GetProcAddress(Interface->hModule, "l_run");
		
		if (Interface->Engine.Engine_Open ==NULL ||
			Interface->Engine.Engine_Close==NULL ||
			Interface->Engine.Engine_CloseAll==NULL ||
			Interface->Engine.Engine_Run==NULL ||
			Interface->Engine.GetGlobalString==NULL ||
			Interface->Engine.GetGlobalNumber==NULL ||
			Interface->Engine.GetTableString==NULL ||
			Interface->Engine.GetTableNumber==NULL ||
			Interface->Engine.GetTableSubtableString==NULL ||
			Interface->Engine.GetTableSubtableNumber==NULL ||
			Interface->Engine.Register== NULL ||		
			Interface->Engine.l_setpath == NULL ||
			Interface->Engine.l_run == NULL)
		{
//			Err("LoadPlugIn", __FILE__, __LINE__, "插件 %s 失败", ENGINE_INTERFACE);
			goto LoadPlugInError;
		}
    
	}
	else if (strcmp(Type, IMAGE_BUILDER_INTERFACE) == 0) 
	{	    
	    Interface->ImageBuilder.Image_Open          = (pImage_Open)::GetProcAddress(Interface->hModule, "Image_Open");
	    Interface->ImageBuilder.Image_GetItem       = (pImage_GetItem)::GetProcAddress(Interface->hModule, "Image_GetItem");
	    Interface->ImageBuilder.Image_GetItemLength = (pImage_GetItemLength)::GetProcAddress(Interface->hModule, "Image_GetItemLength");
	    Interface->ImageBuilder.Image_ReadItem      = (pImage_ReadItem)::GetProcAddress(Interface->hModule, "Image_ReadItem");
	    Interface->ImageBuilder.Image_CloseItem     = (pImage_CloseItem)::GetProcAddress(Interface->hModule, "Image_CloseItem");
	    Interface->ImageBuilder.Image_Close         = (pImage_Close)::GetProcAddress(Interface->hModule, "Image_Close");
        if (!Interface->ImageBuilder.Image_Open || 
            !Interface->ImageBuilder.Image_GetItem || 
            !Interface->ImageBuilder.Image_GetItemLength || 
            !Interface->ImageBuilder.Image_ReadItem || 
            !Interface->ImageBuilder.Image_CloseItem || 
            !Interface->ImageBuilder.Image_Close)
        {
//			Err("LoadPlugIn", __FILE__, __LINE__, "插件 %s 失败", IMAGE_BUILDER_INTERFACE);
            goto LoadPlugInError;
        }
	}
/*	else if (strcmp(Type, ROOT_TOOLS_INTERFACE) == 0 )
	{
	    Interface->RootTools.ImageTools = (pImageTools)::GetProcAddress(Interface->hModule, "ImageTools");
	    if (Interface->RootTools.ImageTools == NULL)
		{
			Err("LoadPlugIn", __FILE__, __LINE__, "插件 %s 失败", ROOT_TOOLS_INTERFACE);
	        goto LoadPlugInError;
		}
	}*/
	else //未知接口
	{	
		FreeLibrary(Interface->hModule);
		delete Interface;
		Interface = NULL;
//		Err("LoadPlugIn", __FILE__, __LINE__, "%s类型插件当前不支持!", Type);
		return NULL;
	}

	return Interface;
	
LoadPlugInError:
    FreeLibrary(Interface->hModule);
	delete Interface;
	Interface = NULL;	
//	Err("LoadPlugIn", __FILE__, __LINE__, "%s插件接口定位失败", Type);
	return NULL;
}



//------------------------------------------------------------------------------------------------------------
//
// 函数说明
//     加载一个插件
//
// 参数说明
//     
//
// 返回值
//     
//
// 其他
//    无
//
//------------------------------------------------------------------------------------------------------------
Interface_t *LoadPlugIn(PlugIn_Info_t *pPlugInInfo)
{
	return LoadPlugIn(pPlugInInfo->type, pPlugInInfo->filename);
}



//------------------------------------------------------------------------------------------------------------
//
// 函数说明
//     卸载一个插件
//
// 参数说明
//     
//
// 返回值
//     
//
// 其他
//    无
//
//------------------------------------------------------------------------------------------------------------
BOOL	UnLoadPlugIn(Interface_t *Interface)
{
	if (Interface == NULL)
		return TRUE;

	if (Interface->hModule == NULL)
		return TRUE;

	FreeLibrary(Interface->hModule);

	delete Interface;
	Interface = NULL;
	return TRUE;
}



//------------------------------------------------------------------------------------------------------------
//
// 函数说明
//     加载一个插件集合
//
// 参数说明
//     
//
// 返回值
//     
//
// 其他
//    无
//
//------------------------------------------------------------------------------------------------------------
BOOL LoadPlugInSet(InterfaceSet_t * pInterfaceSet, PlugIn_Info_t *pPlugInInfo, int Number)
{
	char szErr[ERR_LEN] = "";

	if (pInterfaceSet == NULL || pPlugInInfo == NULL || Number == 0)
	{
//		Err("LoadPlugInSet", __FILE__, __LINE__, "参数非法\n");
		return FALSE;
	}

	memset(pInterfaceSet, 0, sizeof(InterfaceSet_t));	
	pInterfaceSet->Version = 0x0100;	///服务接口的版本
	pInterfaceSet->Count   = 0;			///有效服务接口的数目	
	for (int i = 0; i < Number; i++)
	{		
		Interface_t	*pInterface_t = NULL;
		pInterface_t = LoadPlugIn(pPlugInInfo+i);
		if (pInterface_t != NULL)
		{
			pInterfaceSet->Interface[i] = pInterface_t;
			pInterfaceSet->Count++;
		}
		else
		{		
//			Err("LoadPlugInSet", __FILE__, __LINE__, "加载插件%s %s失败\n", 
//					pPlugInInfo[i].type, pPlugInInfo[i].filename);
			UnLoadPlugInSet(pInterfaceSet);
			return FALSE;
		}
	}

	return TRUE;
}



//------------------------------------------------------------------------------------------------------------
//
// 函数说明
//     卸载一个插件集合
//
// 参数说明
//     
//
// 返回值
//     
//
// 其他
//    无
//
//------------------------------------------------------------------------------------------------------------
BOOL UnLoadPlugInSet(InterfaceSet_t * pInterfaceSet)
{	
	if (pInterfaceSet == NULL)
		return TRUE;

	int Number = pInterfaceSet->Count;
	for (int i = 0 ; i < Number; i++)
	{
		if (NULL !=	pInterfaceSet->Interface[i])
		{
			UnLoadPlugIn(pInterfaceSet->Interface[i]);
		}
	}
	return TRUE;
}



//------------------------------------------------------------------------------------------------------------
//
// 函数说明
//     加载tool需要的全部插件
//
// 参数说明
//     
//
// 返回值
//     
//
// 其他
//    无
//
//------------------------------------------------------------------------------------------------------------
BOOL LoadAllPlugIn(InterfaceSet_t * pInterfaceSet)
{
#define PLUGIN_COUNT	4
	if (pInterfaceSet == NULL)
		return FALSE;

	PlugIn_Info_t PlugIn_Info[PLUGIN_COUNT] =
	{
		{FEX_INTERFACE,				"Phoenix_Fex.dll"},	//
		{FES_INTERFACE,				"Phoenix_Fes.dll"},	//
		{ELF_INTERFACE,				"Phoenix_Elf.dll"},	//		
		{IMAGE_BUILDER_INTERFACE,	"Imagebuilder.dll"}//
		//{DATACHECK_INTERFACE, "Datacheck.dll"}
	};

	return LoadPlugInSet(pInterfaceSet, PlugIn_Info, PLUGIN_COUNT);
}



//------------------------------------------------------------------------------------------------------------
//
// 函数说明
//     卸载tool需要的全部插件
//
// 参数说明
//     
//
// 返回值
//     
//
// 其他
//    无
//
//------------------------------------------------------------------------------------------------------------
BOOL	UnloadAllPlugIn(InterfaceSet_t * pInterfaceSet)
{
	if (pInterfaceSet == NULL)
		return FALSE;

	for (int i = 0 ; i < MAX_INTERFACE; i++)
	{
		if (NULL !=	pInterfaceSet->Interface[i])
		{
			UnLoadPlugIn(pInterfaceSet->Interface[i]);
		}
	}
	return TRUE;
}


//------------------------------------------------------------------------------------------------------------
// THE END
//------------------------------------------------------------------------------------------------------------

#endif //__PLUGIN_HELP__C____

