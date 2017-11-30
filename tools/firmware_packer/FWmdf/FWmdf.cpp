// FWmdf.cpp : Defines the entry point for the DLL application.
//

//#include "stdafx.h"

#include "ImgMgmt.h"

#ifdef _WIN32
#define DLL_EXP __declspec(dllexport)

BOOL APIENTRY DllMain( HANDLE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
    return TRUE;
}
#else
#define DLL_EXP
#endif

CImgMgmt g_Imgmgmt;
HIMAGE	LoadImgFile(LPCSTR lpFileName)
{
	return g_Imgmgmt.LoadImg(lpFileName);
}

int		GetItemIndex(HIMAGE hImage, LPCSTR szMainType, LPCTSTR szSubType) //返回索引
{
	return g_Imgmgmt.GetItemIndex(hImage, szMainType, szSubType);
}

__int64 GetItemSize(HIMAGE hImage, int nIndex) //返回长度
{
	return g_Imgmgmt.GetItemSize(hImage, nIndex);
}

LRESULT	GetImgItem(HIMAGE hImage , int nIndex, LPBYTE lpBuff, DWORD dwLen)    //获取文件内容
{
	BOOL bnRet = g_Imgmgmt.GetImgItem(hImage, nIndex, lpBuff, dwLen);

	if(bnRet == FALSE)
		return __LINE__;
	return 0;
}

LRESULT	SetImgItem(HIMAGE hImage, int nIndex, LPBYTE lpBuff, DWORD dwLen)	//设置文件
{
	g_Imgmgmt.SetImgItem(hImage, nIndex, lpBuff, dwLen);
	return 0;
}

LRESULT SetImgItemWithFile(HIMAGE hImage, int nIndex, LPCTSTR lpFileName)	//设置固件项	lpFileName本地文件名，将其内容设置至固件中
{
	FILE* fp = fopen(lpFileName, "r");
	if(fp == NULL)
	{
		char szInfo[MAX_PATH];
		sprintf(szInfo, "***FWMDF***:The file %s is not exit\n", lpFileName);
		OutputDebugString(szInfo);
		return __LINE__;
	}
	fclose(fp);
	g_Imgmgmt.SetImgItem(hImage, nIndex, lpFileName);
	return 0;
}

LRESULT SaveImg(HIMAGE hImage, LPCSTR lpFileName)
{
	return g_Imgmgmt.SaveImg(hImage, lpFileName);
//	return 0;
}

LRESULT Close(HIMAGE hImage)
{
	g_Imgmgmt.Release(hImage);
	return 0;
}

LRESULT SetCallback(pOPCallback pFun)
{
	g_Imgmgmt.SetCallback(pFun);
	return 0;
}

LRESULT AddImgItem(HIMAGE hImage, LPCTSTR lpMainType, LPCTSTR lpSubType, LPCTSTR lpName, LPCTSTR lpFileName)
{
	g_Imgmgmt.AddImgItem(hImage, lpMainType, lpSubType, lpName, lpFileName);
	return 0;
}

LRESULT GetItemMainSubType(HIMAGE hImage,int nIndex, LPSTR lpMainName, LPSTR lpSubName)
{
	return g_Imgmgmt.GetItemMainSubType(hImage, nIndex, lpMainName, lpSubName);
}
int		GetItemCnt(HIMAGE hImage)
{
	return g_Imgmgmt.GetItemCnt(hImage);
}
int main(void)
{
	return 0;
}

extern "C"
{
	DLL_EXP void* GetInterface(LPCTSTR lpID)
	{
		static IMGMDF_IF ImgMdfIf;
		ImgMdfIf.LoadImage = LoadImgFile;
		ImgMdfIf.GetItemIndex	= GetItemIndex;
		ImgMdfIf.GetItemSize	= GetItemSize;
		ImgMdfIf.GetImgItem		= GetImgItem;
		ImgMdfIf.SetImgItem		= SetImgItem;
		ImgMdfIf.SetImgItemWithFile	= SetImgItemWithFile;
		ImgMdfIf.SaveImg		= SaveImg;
		ImgMdfIf.Close			= Close;
		ImgMdfIf.SetCallback	= SetCallback;
		ImgMdfIf.AddImgItem		= AddImgItem;
		ImgMdfIf.GetItemMainSubType = GetItemMainSubType;
		ImgMdfIf.GetItemCnt = GetItemCnt;
		return (void*)&ImgMdfIf;
	}
};
