// DirectoryDialog.cpp: implementation of the CDirectoryDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DirectoryDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#include "SelectFolderDialog.h"
#include "DirDialog.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDirectoryDialog::CDirectoryDialog()
{
	m_Type = ExtMode;

	m_dir = "";
}


CDirectoryDialog::CDirectoryDialog(eDirType type)
{
	if (ExtMode == type || SysMode == type)
		m_Type = type;
	else
		m_Type = ExtMode;

	m_dir = "";
}

CDirectoryDialog::~CDirectoryDialog()
{

}

BOOL CDirectoryDialog::DoModal()
{
	m_dir = "";
	switch(m_Type)
	{
	case ExtMode:
		{
			CString cstrFilters = "";
			CSelectFolderDialog oSelectFolderDialog(FALSE, 
				NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
				cstrFilters, NULL);
			if(oSelectFolderDialog.DoModal() == IDOK)
			{
				m_dir = oSelectFolderDialog.GetSelectedPath();		
				return TRUE;
			}
			else
			{
				return FALSE;
			}
		}
		break;
	case SysMode:
		{
			CDirDialog	dlg;
			if (IDOK  == dlg.DoBrowse(NULL)) 
			{				
				m_dir = dlg.m_strPath;
				return TRUE;
			}	
			else
			{
				return FALSE;
			}
		}
		break;
	default:
		return FALSE;
	}
}

CString CDirectoryDialog::GetSelectedDirectory()
{
	return m_dir;
}
