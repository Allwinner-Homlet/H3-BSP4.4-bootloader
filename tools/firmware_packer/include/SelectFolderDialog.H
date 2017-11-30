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
//                               (c) Copyright 2007-2008, Scottyu China                                     //
//                                                                                                          //
//                                           All Rights Reserved                                            //
//                                                                                                          //
// File    : SelectFolderDialog.h                                                                           //
// By      : scottyu                                                                                        //
// Version : V1.00                                                                                          //
// Time    : 2008-07-14 14:51:58                                                                            //
//                                                                                                          //
//----------------------------------------------------------------------------------------------------------//
//                                                                                                          //
// HISTORY                                                                                                  //
//                                                                                                          //
// 1 2008-07-14 14:52:02                                                                                    //
//                                                                                                          //
//                                                                                                          //
//                                                                                                          //
//----------------------------------------------------------------------------------------------------------//

#if !defined(AFX_MYFD_H__F9CB9441_F91B_11D1_8610_0040055C08D9__INCLUDED_)
#define AFX_MYFD_H__F9CB9441_F91B_11D1_8610_0040055C08D9__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
//

#include <afxdlgs.h>

/////////////////////////////////////////////////////////////////////////////
// CSelectFolderDialog dialog

class CSelectFolderDialog : public CFileDialog
{
	DECLARE_DYNAMIC(CSelectFolderDialog)

public:
	CSelectFolderDialog(BOOL bShowFilesInDir = FALSE, 
				LPCSTR lpcstrInitialDir = NULL, 
				DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
				LPCTSTR lpszFilter = NULL, CWnd* pParentWnd = NULL);
	~CSelectFolderDialog();

	static WNDPROC m_wndProc;
	virtual void OnInitDone();

	void OnFolderChange();

	void SetSelectedPath(LPCSTR lpcstrPath);
	CString GetSelectedPath() const;

	void SetTitle(CString cstrTitle);

protected:
	//{{AFX_MSG(CSelectFolderDialog)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	BOOL m_bShowFilesInDir;
	CString m_cstrPath;
	CEdit *m_pEdit;
	CString m_cstrTitle;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.


//------------------------------------------------------------------------------------------------------------
// THE END !
//------------------------------------------------------------------------------------------------------------

#endif // !defined(AFX_MYFD_H__F9CB9441_F91B_11D1_8610_0040055C08D9__INCLUDED_)

