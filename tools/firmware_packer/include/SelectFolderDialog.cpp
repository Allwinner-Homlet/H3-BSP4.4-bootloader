


#include "SelectFolderDialog.h"
#include <DLGS.H>
#include <WINUSER.H>
#include <IO.H>
#include <imagehlp.h>
#pragma comment(lib, "imagehlp")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define IDLB_FOLDER_DIALOG_FILES		101
#define IDEB_FOLDER_DIALOG_SEL_FOLDER	102

/////////////////////////////////////////////////////////////////////////////
// CSelectFolderDialog

IMPLEMENT_DYNAMIC(CSelectFolderDialog, CFileDialog)

WNDPROC CSelectFolderDialog::m_wndProc = NULL;

//------------------------------------------------------------------
//函数功能:
//函数流程:
//------------------------------------------------------------------
CSelectFolderDialog::CSelectFolderDialog(
						BOOL bShowFilesInDir /*= FALSE*/, 
						LPCSTR lpcstrInitialDir /*= NULL*/, 
						DWORD dwFlags /*= OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT*/,
						LPCTSTR lpszFilter /*= NULL*/, 
						CWnd* pParentWnd /*= NULL*/)
						: CFileDialog(TRUE, NULL, 
					(	bShowFilesInDir)? ((lpszFilter)? NULL : "*.*") : _T("*..*"), 
									dwFlags, lpszFilter, pParentWnd)
{
	if(lpcstrInitialDir)
		m_ofn.lpstrInitialDir = lpcstrInitialDir;
	
	m_bShowFilesInDir = bShowFilesInDir;

	m_pEdit = NULL;

	m_cstrTitle = "Select folder";
}


//------------------------------------------------------------------
//函数功能:
//函数流程:
//------------------------------------------------------------------
CSelectFolderDialog::~CSelectFolderDialog()
{
	if(m_pEdit)
		delete m_pEdit;
}

BEGIN_MESSAGE_MAP(CSelectFolderDialog, CFileDialog)
	//{{AFX_MSG_MAP(CSelectFolderDialog)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



//------------------------------------------------------------------
//函数功能:
//函数流程:
// Function name	: WindowProcNew
// Description	    : Call this function when user navigate into CFileDialog.
// Return type		: LRESULT
// Argument         : HWND hwnd
// Argument         : UINT message
// Argument         : WPARAM wParam
// Argument         : LPARAM lParam
//------------------------------------------------------------------
LRESULT CALLBACK WindowProcNew(HWND hwnd,UINT message, WPARAM wParam, LPARAM lParam)
{
	if(message ==  WM_COMMAND)
	{
		if (HIWORD(wParam) == BN_CLICKED)
		{
			if (LOWORD(wParam) == IDOK)
			{
				if (CFileDialog* pDlg = (CFileDialog*)CWnd::FromHandle(hwnd))
				{
					CString cstrPath;
					((CSelectFolderDialog*)pDlg->GetDlgItem(
						IDEB_FOLDER_DIALOG_SEL_FOLDER))->GetWindowText(cstrPath);
					
					//If there is nothing entered in the IDEB_FOLDER_DIALOG_SEL_FOLDER
					//Then use the current sel folder
					if(cstrPath.GetLength() == 0)
						cstrPath = ((CSelectFolderDialog*)pDlg->GetDlgItem(0))->GetFolderPath();

					//If the user has entered something like "c:\\windows\\\\system"
					//this is invalid but _access returns valid if this string is passed to
					//it. So before giving it to _access validate it.
					for (int i = 0; i < cstrPath.GetLength() - 1; i++)
						if (cstrPath[i] == '\\' && cstrPath[i+1] == '\\')
						{
							AfxMessageBox("Invalid folder", MB_OK | MB_ICONHAND);
							return NULL;
						}
					
					//If user has entered a trailing backslash remove it
					//e.g. "c:\\windows\\system\\" --> "c:\\windows\\system"
					if (cstrPath[i] == '\\')
						cstrPath = cstrPath.Left (cstrPath.GetLength() - 1);

					//If path does not exist
					if(_access(cstrPath, 00) != 0)
					{
						if(AfxMessageBox(cstrPath + "\n" + "\n" +
							"This path does not exist. Do you want to create it?",
							MB_YESNO | MB_ICONQUESTION) == IDYES)
						{
							MakeSureDirectoryPathExists(cstrPath + "\\");
							if(_access(cstrPath, 00) != 0)
							{
								AfxMessageBox((CString)"Could not create folder:" + "\n" + cstrPath);
								return NULL;
							}
						}
					}

					((CSelectFolderDialog*)pDlg->GetDlgItem(0))->SetSelectedPath(cstrPath);//soctt 2008-10-15 13:51:34
					//((CSelectFolderDialog*)pDlg->GetDlgItem(0))->SetSelectedPath(cstrPath + "\\");

					pDlg->EndDialog(IDOK);

					return NULL;
				}
			}
		}
	}

	return CallWindowProc(CSelectFolderDialog::m_wndProc, hwnd, message, wParam, lParam);
}


//------------------------------------------------------------------
//函数功能:
//函数流程:
// Function name	: CSelectFolderDialog::OnInitDone
// Description	    : For update the wiew of CFileDialog
// Return type		: void 
//------------------------------------------------------------------
void CSelectFolderDialog::OnInitDone()
{
	HideControl(edt1);

	CWnd* pFD = GetParent();
	CRect rectCancelPB; pFD->GetDlgItem(IDCANCEL)->GetWindowRect(rectCancelPB);
	pFD->ScreenToClient(rectCancelPB);
	CRect rectOKPB; pFD->GetDlgItem(IDOK)->GetWindowRect(rectOKPB);
	pFD->ScreenToClient(rectOKPB);
	pFD->GetDlgItem(IDOK)->SetWindowPos(0,rectCancelPB.left - rectOKPB.Width() - 4, rectCancelPB.top, 0,0, SWP_NOZORDER | SWP_NOSIZE);
	CRect rectList2LC; pFD->GetDlgItem(lst1)->GetWindowRect(rectList2LC);
	pFD->ScreenToClient(rectList2LC);
	
	pFD->GetDlgItem(lst1)->SetWindowPos(0,0,0,rectList2LC.Width(), abs(rectList2LC.top - (rectCancelPB.top - 45)) + 25, SWP_NOMOVE | SWP_NOZORDER);
	
	SetControlText(IDOK, _T("Select"));
	pFD->SetWindowText(_T(m_cstrTitle));
	m_wndProc = (WNDPROC)SetWindowLong(pFD->m_hWnd, GWL_WNDPROC, (long)WindowProcNew);

	CRect rectstc3ST; 
	pFD->GetDlgItem(stc3)->GetWindowRect(rectstc3ST);
	pFD->ScreenToClient(rectstc3ST);
	pFD->GetDlgItem(stc3)->SetWindowText("Current Folder");
	pFD->GetDlgItem(stc3)->SetWindowPos(0, rectstc3ST.left, rectstc3ST.top + 14, rectstc3ST.Width() + 20, rectstc3ST.Height(), SWP_NOZORDER);

	CRect rectCurrFolderEB;
	pFD->GetDlgItem(edt1)->GetWindowRect(rectCurrFolderEB);
	pFD->ScreenToClient(rectCurrFolderEB);
	rectCurrFolderEB.left = rectstc3ST.left;
	rectCurrFolderEB.right -= 70;
	rectCurrFolderEB.top += 30;
	rectCurrFolderEB.bottom += 30;

	m_pEdit = new CEdit();
	m_pEdit->Create(WS_TABSTOP | WS_VISIBLE | WS_CHILD, 
					rectCurrFolderEB, pFD, IDEB_FOLDER_DIALOG_SEL_FOLDER);
	m_pEdit->SetWindowText(m_ofn.lpstrInitialDir);
	m_pEdit->SetFont(pFD->GetDlgItem(edt1)->GetFont());
	
	//Draw the thick sunkun edge
	m_pEdit->ModifyStyleEx(0, WS_EX_CLIENTEDGE, SWP_DRAWFRAME);
	
	//Set the Tab order after the Folders Listbox
	m_pEdit->SetWindowPos(pFD->GetDlgItem(lst1), 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

	if(!m_bShowFilesInDir || (m_ofn.lpstrFilter == NULL) || (strlen(m_ofn.lpstrFilter) == 0))
	{
		//File Types Static text and Combo box
		HideControl(stc2);
		HideControl(cmb1);
	}
	
	if(m_bShowFilesInDir && (m_ofn.lpstrFilter != NULL) && (strlen(m_ofn.lpstrFilter) != 0))
	{
		//Increase the height of the dialog
		CRect rectDialog;
		pFD->GetWindowRect(rectDialog);
		rectDialog.bottom += 35;
		pFD->SetWindowPos(0, 0, 0, rectDialog.Width(), rectDialog.Height(), SWP_NOZORDER | SWP_NOMOVE);

		CRect rectST;
		pFD->GetDlgItem(stc2)->GetWindowRect(rectST);
		pFD->ScreenToClient(rectST);
		rectST.top += 25;
		rectST.bottom += 25;
		pFD->GetDlgItem(stc2)->SetWindowPos(0, rectST.left, rectST.top, 0, 0, SWP_NOZORDER | SWP_NOSIZE);

		CRect rectComboDL;
		pFD->GetDlgItem(cmb1)->GetWindowRect(rectComboDL);
		pFD->ScreenToClient(rectComboDL);
		rectComboDL.left = rectST.left;
		rectComboDL.top = rectST.top + rectST.Height();
		rectComboDL.bottom = rectST.bottom + rectST.Height();
		pFD->GetDlgItem(cmb1)->SetWindowPos(0, rectComboDL.left, rectComboDL.top, 0, 0, SWP_NOZORDER | SWP_NOSIZE);

		pFD->GetDlgItem(IDCANCEL)->GetWindowRect(rectCancelPB);
		pFD->ScreenToClient(rectCancelPB);
		rectCancelPB.top = rectST.bottom - 2;
		rectCancelPB.bottom = rectCancelPB.top + rectCancelPB.Height();
		pFD->GetDlgItem(IDCANCEL)->SetWindowPos(0, rectCancelPB.left, rectCancelPB.top, 0, 0, SWP_NOZORDER | SWP_NOSIZE);

		pFD->GetDlgItem(IDOK)->GetWindowRect(rectOKPB);
		pFD->ScreenToClient(rectOKPB);
		pFD->GetDlgItem(IDOK)->SetWindowPos(0,rectCancelPB.left - rectOKPB.Width() - 4, rectCancelPB.top, 0,0, SWP_NOZORDER | SWP_NOSIZE);

		rectCurrFolderEB.right = rectCancelPB.right;
		m_pEdit->SetWindowPos(pFD->GetDlgItem(lst1), 0, 0, rectCurrFolderEB.Width(), rectCurrFolderEB.Height(), SWP_NOMOVE | SWP_NOZORDER);
	}

	pFD->CenterWindow();
}


//------------------------------------------------------------------
//函数功能:
//函数流程:
//------------------------------------------------------------------
void CSelectFolderDialog::OnFolderChange()
{
	if(::IsWindow(GetParent()->m_hWnd))
	{
		m_pEdit->SetWindowText(GetFolderPath());
	}
}


//------------------------------------------------------------------
//函数功能:
//函数流程:
//------------------------------------------------------------------
void CSelectFolderDialog::SetSelectedPath(LPCSTR lpcstrPath)
{
	m_cstrPath = lpcstrPath;
}



//------------------------------------------------------------------
//函数功能:
//函数流程:
//------------------------------------------------------------------
CString CSelectFolderDialog::GetSelectedPath() const
{
	return m_cstrPath;
}



//------------------------------------------------------------------
//函数功能:
//函数流程:
//------------------------------------------------------------------
void CSelectFolderDialog::SetTitle(CString cstrTitle)
{
	m_cstrTitle = cstrTitle;
}

