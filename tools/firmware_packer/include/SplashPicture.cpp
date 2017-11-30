// SplashPicture.cpp: implementation of the CSplashPicture class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SplashPicture.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#include "FileDirHelp.h"


CSplashPicture::CSplashPicture(CString csPath, DWORD dwTime)
{
	fpSetPicture = NULL;
	fpShowPicture = NULL;
	fpClosePicture = NULL;
	hdll = NULL;

	CString csDllPath;
	csDllPath = CFileDirHelp::GetModuleDirectory();
	csDllPath = csDllPath + "SplashPictureLib.dll";
	hdll = ::LoadLibrary(csDllPath); 
	if (NULL == hdll) 
	{
		::AfxMessageBox("Load lib SplashPictureLib.dll failed !");
		return ;
	}
	
	fpSetPicture    = (pSetPicture) ::GetProcAddress(hdll, "SetPicture"); 	
	fpShowPicture	= (pShowPicture)::GetProcAddress(hdll, "ShowPicture");  	
	fpClosePicture	= (pClosePicture)::GetProcAddress(hdll, "ClosePicture"); 
	if (NULL == fpSetPicture || NULL == fpShowPicture || fpClosePicture == NULL) 
	{
		::FreeLibrary(hdll); 
		hdll	= NULL;
		fpSetPicture= NULL;
		fpShowPicture	= NULL;	
		fpClosePicture = NULL;
		return ;
	}	
	m_picturePath = csPath;
	m_dwTime = dwTime;
	fpSetPicture(m_picturePath);
}

CSplashPicture::~CSplashPicture()
{
	if (NULL != hdll) 
	{
		::FreeLibrary(hdll); 
		hdll	= NULL;
		fpSetPicture= NULL;
		fpShowPicture	= NULL;	
		fpClosePicture = NULL;
	}
}

BOOL CSplashPicture::ShowStartPicture()
{
	if (hdll == NULL || fpSetPicture == NULL || fpShowPicture == NULL || fpClosePicture == NULL)
		return FALSE;

	fpShowPicture();

	::Sleep(m_dwTime);

	fpClosePicture();

	return TRUE;
}
