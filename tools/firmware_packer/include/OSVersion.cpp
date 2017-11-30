// OSVersion.cpp: implementation of the COSVersion class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "OSVersion.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COSVersion::COSVersion()
{

}

COSVersion::~COSVersion()
{

}

eOSVersion COSVersion::GetOSVersion()
{
	m_version = unknown;
	OSVERSIONINFO OsVersionInfo;
	OsVersionInfo.dwOSVersionInfoSize=sizeof(OSVERSIONINFO);
	GetVersionEx(&OsVersionInfo);
	
	switch(OsVersionInfo.dwPlatformId)
	{
	case VER_PLATFORM_WIN32s:		m_version = win31;	break;

	case VER_PLATFORM_WIN32_WINDOWS:
		switch(OsVersionInfo.dwMajorVersion)
		{
		case 4:
			switch(OsVersionInfo.dwMinorVersion)
			{
			case 0:		m_version = win95;			break;				
			default:	m_version = win98;			break;
			}
			break;	
		default:		break;
		}
		break;

	case VER_PLATFORM_WIN32_NT:
		switch(OsVersionInfo.dwMajorVersion)
		{
		case 4:			m_version = winnt;			break;
		case 5:
			switch(OsVersionInfo.dwMinorVersion)
			{
			case 0:		m_version = win2k;			break;
			case 1:		m_version = win2k;			break;				
			default:	break;
			}
			break;
		default:		break;
		}
		break;

	default:	break;
	}
	return m_version;
}


eAnsiCode COSVersion::GetANSICode()
{
	m_ansi = ansi_unknown;
	switch (GetACP())
	{
	case 874  :	m_ansi = ansi_Thai;		break;
	case 932  :	m_ansi = ansi_Japan;	break;
	case 936  : m_ansi = ansi_ChinesePRC; break;
	case 949  :	m_ansi = ansi_Korean;	break;
	case 950  :	m_ansi = ansi_ChineseTW; break;	
	case 1200 :	m_ansi = ansi_Unicode; break;     
	case 1250 :	m_ansi = ansi_Eastern_European; break;    
	case 1251 :	m_ansi = ansi_Cyrillic; break;     
	case 1252 :	m_ansi = ansi_Latin; break;     
	case 1253 :	m_ansi = ansi_Greek; break;     
	case 1254 :	m_ansi = ansi_Turkish; break;     
	case 1255 :	m_ansi = ansi_Hebrew; break;     
	case 1256 :	m_ansi = ansi_Arabic; break;     
	case 1257 :	m_ansi = ansi_Baltic; break;     
	}
	return m_ansi;
}
