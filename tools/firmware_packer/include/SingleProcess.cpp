// SingleProcess.cpp: implementation of the CSingleProcess class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "SingleProcessLibTest.h"
#include "SingleProcess.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#include "FileDirHelp.h"

CSingleProcess::CSingleProcess( char * strAppName,  char * strID)
{
	hSingleProcessLib = NULL;
	SingleProcessOpen = NULL;
	SingleProcessClose= NULL;

	memset(m_strAppName, 0, sizeof(m_strAppName));
	strcpy(m_strAppName, strAppName);
	memset(m_strID, 0, sizeof(m_strID));
	strcpy(m_strID, strID);

	CString csSingleProcessLibPath;
	csSingleProcessLibPath = CFileDirHelp::GetModuleDirectory() + "CheckSingle.dll";
	hSingleProcessLib      = ::LoadLibrary(csSingleProcessLibPath); 	
	
	if (NULL == hSingleProcessLib) 
	{	
		::AfxMessageBox("Please put CheckSingle.dll in current dir !");
		SingleProcessOpen = NULL;
		return ;
	}

	SingleProcessOpen   = (pSingleProcessOpen) ::GetProcAddress(hSingleProcessLib, "SingleProcessOpen"); 	
	SingleProcessClose	= (pSingleProcessClose) ::GetProcAddress(hSingleProcessLib, "SingleProcessClose");  	
	if (NULL == SingleProcessOpen || NULL == SingleProcessClose ) 
	{
		::FreeLibrary(hSingleProcessLib); 
		hSingleProcessLib	= NULL;
		SingleProcessOpen   = NULL;
		SingleProcessClose	= NULL;	
	}		
}

CSingleProcess::~CSingleProcess()
{
	if (hSingleProcessLib != NULL && SingleProcessClose != NULL)
	{
		SingleProcessClose();

		::FreeLibrary(hSingleProcessLib);
		hSingleProcessLib = NULL;
		SingleProcessOpen = NULL;
		SingleProcessClose = NULL;
	}
}

BOOL CSingleProcess::IsSingle()
{
	if (SingleProcessOpen != NULL)
	{
		return SingleProcessOpen(m_strAppName, m_strID);
	}
	else
	{
		return FALSE;
	}
}
