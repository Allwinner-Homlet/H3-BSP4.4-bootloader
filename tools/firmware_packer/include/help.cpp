// Help.cpp: implementation of the CHelp class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"

#ifndef __cplusplus
#define __cplusplus
#endif //__cplusplus

#include "Help.h"

#include <windows.h>
#include "htmlhelp.h"
#pragma comment(lib, "htmlhelp.lib") 
#pragma comment(lib, "setupapi.lib") 
#pragma comment(lib, "winmm.lib") 

/*
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
*/

#include "File.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


u32 ShowHelp(emHelpType type, const char * csHelpFilepath)
{
	if (!csHelpFilepath)
		return __LINE__;

	if (strlen(csHelpFilepath) ==0)
		return __LINE__;

	//文件是否存在
	if (OK != IsFileExist(csHelpFilepath))
		return __LINE__;

	switch(type)
	{
	case chmType:	//直接打开帮助文件的默认主题
		HtmlHelp(NULL, csHelpFilepath, HH_DISPLAY_TOPIC, 0);
		break;

	case pdfType:
		::ShellExecute(NULL, "open", csHelpFilepath, "", NULL, SW_SHOWDEFAULT );
		break;

	case docType:
		::ShellExecute(NULL, "open", csHelpFilepath, "", NULL, SW_SHOWDEFAULT );
		break;

	case unkownType:
		return __LINE__;

	default:
		return __LINE__;

	}

	return OK;
}

/*
CHelp::CHelp()
{
	m_type = unkownType;
	m_helpfilepath = "";
}

CHelp::~CHelp()
{

}

BOOL CHelp::ShowHelp(emHelpType type, CString csHelpFilepath)
{
	m_type = type;
	m_helpfilepath = csHelpFilepath;
	if (m_helpfilepath.IsEmpty())
		return FALSE;

	//文件是否存在
	if (OK != IsFileExist((LPCTSTR)m_helpfilepath))
		return FALSE;

	switch(m_type)
	{
	case chmType:	
		//直接打开帮助文件的默认主题
		HtmlHelp(NULL,(LPCSTR)m_helpfilepath, HH_DISPLAY_TOPIC, 0);
		break;

	case pdfType:
		::ShellExecute(NULL, "open", m_helpfilepath, "", NULL, SW_SHOWDEFAULT );
		break;

	case docType:
		::ShellExecute(NULL, "open", m_helpfilepath, "", NULL, SW_SHOWDEFAULT );
		return FALSE;
		break;

	case unkownType:
		return FALSE;
		break;
	default:
		return FALSE;
		break;
	}

	return TRUE;
}

  */