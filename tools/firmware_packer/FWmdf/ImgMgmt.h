// ImgMgmt.h: interface for the CImgMgmt class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMGMGMT_H__B0AE058F_F019_47BF_A90A_117F239F0244__INCLUDED_)
#define AFX_IMGMGMT_H__B0AE058F_F019_47BF_A90A_117F239F0244__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef IMGMGMTEXPORT
#define IMG_API __declspec(dllexport)
#else
#define IMG_API __declspec(dllimport);
#endif
#define __RC_ENCODE__
#define __RC_DECODE__


#include "basetypes.h"
#include "fwmdf_if.h"
#include "ImageFile.h"
#include "encode_cfg.h"
#include <stdlib.h>
#include <malloc.h>
#include <stdio.h>

#include <vector>
#ifndef _WIN32
typedef FILE* HANDLE;
#endif
using namespace std;







#define HEAD_ID				0		//头加密接口索引
#define TABLE_ID			1		//表加密接口索引
#define DATA_ID				2		//数据加密接口索引
#define IF_CNT				3		//加密接口个数	现在只有头加密，表加密，数据加密3种
#define	MAX_KEY_SIZE 		32		//密码长度

#pragma pack(push, 1)

typedef struct __tag_ImgMemItem
{
	__int64	nLength;
	char    szName[256];
	LPBYTE	pBuffer;
}ImgMemItem; 

typedef struct tag_IMAGE_HANDLE
{		
	HANDLE 				_hf;					//文件句柄
	ImageHead_t			_ImageHead;				//img头信息
	RC_ENDECODE_IF_t	_rc_if_decode[IF_CNT];	//解密接口
	ImageItem_t*		_pItemTable;			//item信息表
	ImgMemItem*			_pItemMem;				//item内存内容
	BOOL				_bUseEncypt;
}IMAGE_HANDLE;

#define INVALID_INDEX		0xFFFFFFFF

typedef struct tag_ITEM_HANDLE{	
	u32	index;					//在ItemTable中的索引
	u32 pos;
}ITEM_HANDLE;

#pragma pack(pop)




typedef void* (*pLoadPlugin)();
//typedef HANDLE HIMAGE;


class CImgMgmt  
{
public:
	CImgMgmt();
	virtual ~CImgMgmt();

public:
	HIMAGE	LoadImg(LPCSTR lpFileName);
	int		EnumItem(HIMAGE hImage, LPCSTR lpFileName, DWORD& dwLen); //枚举文件，返回索引和长度
	int		GetItemIndex(HIMAGE hImage , LPCSTR lpFileName);
	int		GetItemIndex(HIMAGE hImage , LPCSTR szMainType, LPCTSTR szSubType);
	__int64 GetItemSize(HIMAGE hImage , int nIndex);    //
	LRESULT	GetImgItem(HIMAGE hImage , int nIndex, LPBYTE lpBuff, DWORD dwLen);    //获取文件内容
	LRESULT	SetImgItem(HIMAGE hImage, int nIndex, LPBYTE lpBuff, DWORD dwLen);	//设置文件
	LRESULT SetCallback(pOPCallback pFun);

	LRESULT GetItemMainSubType(HIMAGE hImage,int nIndex, LPSTR lpMainName, LPSTR lpSubName);
	int		GetItemCnt(HIMAGE hImage);

	//设置固件项	lpFileName本地文件名，将其内容设置至固件中
	LRESULT	SetImgItem(HIMAGE hImage, int nIndex, LPCTSTR lpFileName);	
	LRESULT	SaveImg(HIMAGE hImage, LPCSTR lpFileName);
	void	Release(HIMAGE hImage);

	LRESULT AddImgItem(HIMAGE hImage, LPCTSTR lpMainType, LPCTSTR lpSubType, LPCTSTR lpName, LPCTSTR lpFileName);
protected:
	void	OPCallback(WPARAM wParam, LPARAM lParam);
	pOPCallback			_pCallbackFun;
//	IMAGE_HANDLE*       _pImgHandle;
//	ImageHead_t			_ImgHead;
//	vector<ImageItem_t> _ImgItems;
//	vector<ImgMemItem>	_ImgItemMems; 
//	AW_PLGIN_FUN_REGTRAN* _pPluginFun;
};


#endif // !defined(AFX_IMGMGMT_H__B0AE058F_F019_47BF_A90A_117F239F0244__INCLUDED_)
