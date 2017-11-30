#ifndef __FW_MODIFY_HEAD_H___
#define __FW_MODIFY_HEAD_H___

#define  FWMDF_PLGIN_ID "EA4E0225-03F9-4289-A690-3BCD89633089"
#define  FWMDF_PLGIN_NAME "FWmdf.dll"

#define  FWMDF_PLGIN_ID_64 "B9F9BCF8-C7C1-4ec2-BA5E-2307F6E95A2F"
#define  FWMDF_PLGIN_NAME_64 "FWmdf64.dll"

//------------------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------------------
typedef void * HIMAGE;
#ifndef _WIN32
typedef unsigned int u32;
typedef unsigned char* LPBYTE;
typedef bool BOOL;
typedef const char* LPCSTR;
typedef unsigned long DWORD;
typedef LPCSTR LPCTSTR;
typedef long long __int64;
typedef char* LPSTR;
typedef u32 WPARAM;
typedef u32 LPARAM;
typedef unsigned char BYTE;
typedef char TCHAR;
typedef u32 LRESULT;
typedef u32 UINT;
typedef char* LPTSTR;

#define MAX_PATH 1024
#define ZeroMemory(buff, len)	memset(buff, 0, len)
#define OutputDebugString printf
#define TRUE	1
#define FALSE	0
#endif
typedef void * HIMAGEITEM;
typedef LRESULT (*pOPCallback)(WPARAM wParam, LPARAM lParam, LPCTSTR lpInfo);
typedef LRESULT (*pAddImgItem)(HIMAGE hImage, LPCTSTR lpMainType, LPCTSTR lpSubType, LPCTSTR lpName, LPCTSTR lpFileName);
typedef HIMAGE	(*pLoadImg)(LPCSTR lpFileName);
typedef int		(*pGetItemIndex)(HIMAGE hImage, LPCSTR szMainType, LPCTSTR szSubType); //��������
typedef __int64 (*pGetItemSize)(HIMAGE hImage, int nIndex); //���س���
typedef LRESULT	(*pGetImgItem)(HIMAGE hImage , int nIndex, LPBYTE lpBuff, DWORD dwLen);    //��ȡ�ļ�����
typedef LRESULT	(*pSetImgItem)(HIMAGE hImage, int nIndex, LPBYTE lpBuff, DWORD dwLen);	//�����ļ�
typedef LRESULT (*pSetImgItemWithFile)(HIMAGE hImage, int nIndex, LPCTSTR lpFileName);	//���ù̼���	lpFileName�����ļ��������������������̼���
typedef LRESULT (*pSaveImg)(HIMAGE hImage, LPCSTR lpFileName);
typedef LRESULT (*pSetCallback)(pOPCallback pFun);
typedef LRESULT (*pClose)(HIMAGE hImage);
typedef LRESULT (*pGetItemMainSubType)(HIMAGE hImage,int nIndex, LPSTR lpMainName, LPSTR lpSubName);
typedef int		(*pGetItemCnt)(HIMAGE hImage);
//////////////////////////////////////////////////////////////////////////
// For pOPCallback �ص���������
//////////////////////////////////////////////////////////////////////////
// For wParam
#define FWMDF_INFO_PROMPT	0x0001	// ��ʾ��Ϣ
#define FWMDF_INFO_WARNING	0x0002	// ������Ϣ
#define FWMDF_INFO_ERROR	0x0004	// ������Ϣ
#define FWMDF_INFO_LOADING	0x0008  // ������Ϣ
#define FWMDF_INFO_SAVING	0x0010	// ������Ϣ

// for lParam
// if wparam == FWMDF_INFO_PROMPT
// lparam: ��ʾ��Ϣ

// if wparam == FWMDF_INFO_WARNING
// lparam: ������Ϣ

// if wparam == FWMDF_INFO_ERROR
// lparam:
#define FWMDF_ERROR_VERSION 0x0001 // �̼��汾��֧��
#define FWMDF_ERROR_READ	0x0002 // ��ȡ�ļ�����
#define FWMDF_ERROR_WRITE	0x0004 // д�ļ�����


// if wparam == FWMDF_INFO_LOADING
// lparam: ���ؽ��ȵİٷֱȣ�0~100��


// if wparam == FWMDF_INFO_SAVING
// lparam: ������ȵİٷֱȣ�0~100��



//------------------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------------------
#pragma pack(push, 1)
typedef struct tag_IMGMDF_IF{
	pLoadImg      		LoadImage;
	pGetItemIndex     	GetItemIndex;
	pGetItemSize		GetItemSize;
	pGetImgItem			GetImgItem;
	pSetImgItem			SetImgItem;
	pSetImgItemWithFile	SetImgItemWithFile;
	pSaveImg			SaveImg;
	pSetCallback		SetCallback;
	pClose				Close;
	pAddImgItem			AddImgItem;
	pGetItemCnt			GetItemCnt;
	pGetItemMainSubType GetItemMainSubType;
}IMGMDF_IF;
#pragma pack(pop)


#endif //__FW_MODIFY_HEAD_H___
