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
//                                                                                                          //
// File    : consolescreen.h                                                                                //
// By      : scottyu                                                                                        //
// Version : V1.00                                                                                          //
// Time    : 2008-04-14 13:32:00                                                                            //
//                                                                                                          //
//----------------------------------------------------------------------------------------------------------//
//                                                                                                          //
// HISTORY                                                                                                  //
//                                                                                                          //
// 1 2008-04-14 13:32:00                                                                                    //
//                                                                                                          //
//                                                                                                          //
//                                                                                                          //
//----------------------------------------------------------------------------------------------------------//

#if !defined(AFX_CONSOLESCREEN_H__20010922_A5C5_0E5D_0722_0080AD509054__INCLUDED_)
#define AFX_CONSOLESCREEN_H__20010922_A5C5_0E5D_0722_0080AD509054__INCLUDED_

#pragma once

#ifndef _ASSERTE
#define _ASSERTE(x)
#endif


#ifndef ASSERT
#define ASSERT(x)	//
#endif //ASSERT


/////////////////////////////////////////////////////////////////////////////
// CConsole - wrapper around Win32 Console API
//
// Written by Bjarke Viksoe (bjarke@viksoe.dk)
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed by any means PROVIDING it is 
// not sold for profit without the authors written consent, and 
// providing that this notice and the authors name is included. 
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability if it causes any damage to you or your
// computer whatsoever. It's free, so don't hassle me about it.
//
// Beware of bugs.
//
#include <windows.h> //scott 2007-11-2

//------------------------------------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------------------------------------
#define BLACK        0
#define DEEP_BLUE    1
#define DARK_GREEN   2
#define LOW_LAN      3
#define LOW_RED      4
#define ZI_RED       5
#define LOW_YELLOW   6
#define LOW_GRAY     7
#define GRAY         8
#define BLUE         9
#define GREEN        10
#define LAN          11
#define RED          12
#define YELLOW       14
#define WHITE		 15
//------------------------------------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------------------------------------

inline void lprintf(LPCTSTR szFormat, ...)
{
	TCHAR szMsg[1024];
	va_list pArg;
	va_start(pArg, szFormat);
	::wvsprintf(szMsg, szFormat, pArg);
	va_end(pArg);
	DWORD dwWritten = 0; 
	::WriteFile(::GetStdHandle(STD_OUTPUT_HANDLE), szMsg, 
	::lstrlen(szMsg), &dwWritten, NULL); 
};

//------------------------------------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------------------------------------
class CConsole
{
public:
	HANDLE m_hConsole;
	
	CConsole(HANDLE hConsole = INVALID_HANDLE_VALUE) : m_hConsole(hConsole)
	{
		::AllocConsole();
	}
	~CConsole()
	{
		Release();
		::FreeConsole();
	}
	HANDLE Detach()
	{
		HANDLE hConsole = m_hConsole;
		m_hConsole = INVALID_HANDLE_VALUE;
		return hConsole;
	}
	void Release()
	{
		if( m_hConsole == INVALID_HANDLE_VALUE ) return;
		::CloseHandle(m_hConsole);
		m_hConsole = INVALID_HANDLE_VALUE;
	}
	BOOL Create(BOOL bWriteOnly = TRUE)
	{
		_ASSERTE(m_hConsole==INVALID_HANDLE_VALUE);
		//    HANDLE hStdOut = ::GetStdHandle(STD_OUTPUT_HANDLE);
		//    if( hStdOut == INVALID_HANDLE_VALUE ) 
		//		  return FALSE;
		
		// DWORD dwAccess = bWriteOnly ? GENERIC_WRITE : GENERIC_READ | GENERIC_WRITE;
		DWORD dwAccess = GENERIC_READ | GENERIC_WRITE;
		m_hConsole = ::CreateConsoleScreenBuffer( 
			dwAccess,                // access
			FILE_SHARE_READ | FILE_SHARE_WRITE,                       // not shared 
			NULL,                    // no security attributes 
			CONSOLE_TEXTMODE_BUFFER, // must be TEXTMODE 
			NULL);                   // reserved; must be NULL 
		if( m_hConsole == INVALID_HANDLE_VALUE) 
			return FALSE;
		// Make the new screen buffer the active screen buffer. 
		if( !::SetConsoleActiveScreenBuffer(m_hConsole) ) 
			return FALSE;
		
		BOOL bret;
		// Assign io handles
		bret = ::SetStdHandle(STD_INPUT_HANDLE, m_hConsole);
		bret = ::SetStdHandle(STD_OUTPUT_HANDLE, m_hConsole);
		// if( !bWriteOnly ) 
		//	  bret = ::SetStdHandle(STD_INPUT_HANDLE, m_hConsole);
		
		return TRUE;
	}
	
	
	BOOL SetTitle(LPCTSTR pstrTitle) const
	{
		_ASSERTE(m_hConsole!=INVALID_HANDLE_VALUE);
		_ASSERTE(!::IsBadStringPtr(pstrTitle,(UINT)-1));
		return ::SetConsoleTitle(pstrTitle);
	}
	
	
	DWORD GetConsoleMode() const
	{
		_ASSERTE(m_hConsole!=INVALID_HANDLE_VALUE);
		DWORD dwMode = 0;
		if( !::GetConsoleMode(m_hConsole, &dwMode) ) return 0;
		return dwMode;
	}
	
	BOOL SetConsoleMode(DWORD dwMode) const
	{
		_ASSERTE(m_hConsole!=INVALID_HANDLE_VALUE);
		return ::SetConsoleMode(m_hConsole, dwMode);
	}
	
	BOOL ModifyConsoleMode(DWORD dwRemove, DWORD dwAdd) const
	{
		_ASSERTE(m_hConsole!=INVALID_HANDLE_VALUE);
		DWORD dwMode = 0;
		if( !::GetConsoleMode(m_hConsole, &dwMode) ) return FALSE;
		return ::SetConsoleMode(m_hConsole, (dwMode & ~dwRemove) | dwAdd);
	}
	WORD GetTextAttribute() const
	{
		CONSOLE_SCREEN_BUFFER_INFO Info = { 0 };
		GetScreenBufferInfo(&Info);
		return Info.wAttributes;
	}
	BOOL SetTextAttribute(WORD wAttrib) const
	{
		_ASSERTE(m_hConsole!=INVALID_HANDLE_VALUE);
		return ::SetConsoleTextAttribute(m_hConsole, wAttrib);
	}
	BOOL SetTextColor(WORD wColor)
	{
		//ATLASSERT(wColor>=0x0 && wColor<=0xF);                   // FOREGROUND_BLUE etc
		ASSERT(wColor>=0x0 && wColor<=0xF); 
		return SetTextAttribute(wColor);
	}
	BOOL SetBkColor(WORD wColor)
	{
		//ATLASSERT(wColor==0 || (wColor>=0x10 && wColor<=0xF0));  // BACKGROUND_BLUE etc
		ASSERT(wColor==0 || (wColor>=0x10 && wColor<=0xF0)); 
		return SetTextAttribute(wColor);
	}
	COORD GetLargestWindowSize() const
	{
		_ASSERTE(m_hConsole!=INVALID_HANDLE_VALUE);
		return ::GetLargestConsoleWindowSize(m_hConsole);
	}
	COORD GetCursorPos() const
	{
		CONSOLE_SCREEN_BUFFER_INFO Info = { 0 };
		GetScreenBufferInfo(&Info);
		return Info.dwCursorPosition;
	}
	BOOL GetScreenBufferInfo(PCONSOLE_SCREEN_BUFFER_INFO lpConsoleScreenBufferInfo) const
	{
		_ASSERTE(m_hConsole!=INVALID_HANDLE_VALUE);
		_ASSERTE(lpConsoleScreenBufferInfo);
		return GetConsoleScreenBufferInfo(m_hConsole, lpConsoleScreenBufferInfo);
	}
	BOOL GetCursorPos(SHORT& x, SHORT& y) const
	{
		CONSOLE_SCREEN_BUFFER_INFO Info = { 0 };
		if( !GetScreenBufferInfo(&Info) ) return FALSE;
		x = Info.dwCursorPosition.X;
		y = Info.dwCursorPosition.Y;
		return TRUE;
	}
	BOOL SetCursorPos(COORD Coord)
	{
		_ASSERTE(m_hConsole!=INVALID_HANDLE_VALUE);
		return ::SetConsoleCursorPosition(m_hConsole, Coord);
	}
	BOOL SetCursorPos(SHORT x, SHORT y)
	{
		_ASSERTE(m_hConsole!=INVALID_HANDLE_VALUE);
		COORD Coord = { x, y };
		return ::SetConsoleCursorPosition(m_hConsole, Coord);
	}
	BOOL GetCursorInfo(PCONSOLE_CURSOR_INFO pInfo) const
	{
		_ASSERTE(m_hConsole!=INVALID_HANDLE_VALUE);
		_ASSERTE(pInfo);
		return ::GetConsoleCursorInfo(m_hConsole, pInfo);
	}
	BOOL SetCursorInfo(const PCONSOLE_CURSOR_INFO pInfo) const
	{
		_ASSERTE(m_hConsole!=INVALID_HANDLE_VALUE);
		_ASSERTE(pInfo);
		return ::SetConsoleCursorInfo(m_hConsole, pInfo);
	}
	BOOL ShowCursor(BOOL bShow)
	{
		_ASSERTE(m_hConsole!=INVALID_HANDLE_VALUE);
		CONSOLE_CURSOR_INFO Info = { 0 };
		Info.dwSize = sizeof(CONSOLE_CURSOR_INFO);
		Info.bVisible = bShow;
		return ::SetConsoleCursorInfo(m_hConsole, &Info);
	}
	BOOL Read(LPTSTR lpBuffer, DWORD nNumberOfCharsToRead, LPDWORD lpNumberOfCharsRead) const
	{
		//scott
		//	   HANDLE hStdIn = ::GetStdHandle(STD_INPUT_HANDLE);
		//      ASSERT( hStdIn != INVALID_HANDLE_VALUE ) ;	
		//		return ::ReadConsole(hStdIn, lpBuffer, nNumberOfCharsToRead, lpNumberOfCharsRead, NULL);
		
		//orign
		_ASSERTE(m_hConsole!=INVALID_HANDLE_VALUE);
		_ASSERTE(lpBuffer);
		return ::ReadConsole(m_hConsole, lpBuffer, nNumberOfCharsToRead, lpNumberOfCharsRead, NULL);
	}
	BOOL Write(LPCTSTR pstrText, int nNumberOfCharsToWrite = -1, LPDWORD lpNumberOfCharsWritten = NULL) const
	{
		_ASSERTE(m_hConsole!=INVALID_HANDLE_VALUE);
		_ASSERTE(pstrText);
		DWORD dwDummy = 0;
		if( lpNumberOfCharsWritten == NULL ) lpNumberOfCharsWritten = &dwDummy;
		if( nNumberOfCharsToWrite == -1 ) nNumberOfCharsToWrite = (int) ::lstrlen(pstrText);
		return ::WriteConsole(m_hConsole, pstrText, (DWORD) nNumberOfCharsToWrite, lpNumberOfCharsWritten, NULL); 
	}
	BOOL Write(CONST CHAR_INFO *lpBuffer, COORD dwBufferSize, COORD Coord, PSMALL_RECT lpWriteRegion) const
	{
		_ASSERTE(m_hConsole!=INVALID_HANDLE_VALUE);
		_ASSERTE(lpWriteRegion);
		return ::WriteConsoleOutput(m_hConsole, lpBuffer,  dwBufferSize, Coord, lpWriteRegion);
	}
	BOOL Write(LPCTSTR lpCharacter, int nLength, COORD Coord, LPDWORD lpNumberOfCharsWritten = NULL) const
	{
		_ASSERTE(m_hConsole!=INVALID_HANDLE_VALUE);
		_ASSERTE(!::IsBadStringPtr(lpCharacter,(UINT)nLength));
		DWORD dwDummy = 0;
		if( lpNumberOfCharsWritten == NULL ) lpNumberOfCharsWritten = &dwDummy;
		if( nLength == -1 ) nLength = (int) ::lstrlen(lpCharacter);
		return ::WriteConsoleOutputCharacter(m_hConsole, lpCharacter, (DWORD) nLength, Coord, lpNumberOfCharsWritten);
	}
	BOOL Write(LPCTSTR lpCharacter, int nLength, SHORT x, SHORT y, LPDWORD lpNumberOfCharsWritten = NULL) const
	{
		_ASSERTE(m_hConsole!=INVALID_HANDLE_VALUE);
		_ASSERTE(!::IsBadStringPtr(lpCharacter,(UINT)nLength));
		COORD Coord = { x, y };
		return Write(lpCharacter, nLength, Coord, lpNumberOfCharsWritten);
	}
	BOOL Write(TCHAR cCharacter, int nLength, COORD Coord, LPDWORD lpNumberOfCharsWritten = NULL) const
	{
		_ASSERTE(m_hConsole!=INVALID_HANDLE_VALUE);
		DWORD dwDummy = 0;
		if( lpNumberOfCharsWritten == NULL ) lpNumberOfCharsWritten = &dwDummy;
		return ::FillConsoleOutputCharacter(m_hConsole, cCharacter, (DWORD) nLength, Coord, lpNumberOfCharsWritten);
	}
	BOOL Write(TCHAR cCharacter, int nLength, SHORT x, SHORT y, LPDWORD lpNumberOfCharsWritten = NULL) const
	{
		_ASSERTE(m_hConsole!=INVALID_HANDLE_VALUE);
		COORD Coord = { x, y };
		return Write(cCharacter, nLength, Coord, lpNumberOfCharsWritten);
	}
	BOOL ReadInput(PINPUT_RECORD lpBuffer, DWORD nLength, LPDWORD lpNumberOfEventsRead = NULL) const
	{
		_ASSERTE(m_hConsole!=INVALID_HANDLE_VALUE);
		DWORD dwDummy = 0;
		if( lpNumberOfEventsRead == NULL ) lpNumberOfEventsRead = &dwDummy;
		return ::ReadConsoleInput(m_hConsole, lpBuffer, nLength, lpNumberOfEventsRead);
	}
	BOOL PeekInput(PINPUT_RECORD lpBuffer, DWORD nLength, LPDWORD lpNumberOfEventsRead = NULL) const
	{
		_ASSERTE(m_hConsole!=INVALID_HANDLE_VALUE);
		DWORD dwDummy = 0;
		if( lpNumberOfEventsRead == NULL ) lpNumberOfEventsRead = &dwDummy;
		return ::PeekConsoleInput(m_hConsole, lpBuffer, nLength, lpNumberOfEventsRead);
	}
	BOOL WriteInput(const PINPUT_RECORD lpBuffer, DWORD nLength, LPDWORD lpNumberOfEventsWritten = NULL) const
	{
		_ASSERTE(m_hConsole!=INVALID_HANDLE_VALUE);
		DWORD dwDummy = 0;
		if( lpNumberOfEventsWritten == NULL ) lpNumberOfEventsWritten = &dwDummy;
		return ::WriteConsoleInput(m_hConsole, lpBuffer, nLength, lpNumberOfEventsWritten);
	}
	BOOL FlushInputBuffer() const
	{
		_ASSERTE(m_hConsole!=INVALID_HANDLE_VALUE);
		return ::FlushConsoleInputBuffer(m_hConsole);
	}
	BOOL FillAttribute(WORD attribute, int nLength, COORD Coord, LPDWORD lpNumberOfAttrsWritten = NULL) const
	{
		_ASSERTE(m_hConsole!=INVALID_HANDLE_VALUE);
		DWORD dwDummy = 0;
		if( lpNumberOfAttrsWritten == NULL ) lpNumberOfAttrsWritten = &dwDummy;
		return ::FillConsoleOutputAttribute(m_hConsole, attribute, (DWORD) nLength, Coord, lpNumberOfAttrsWritten);
	}
	BOOL FillAttributes(CONST WORD* lpAttribute, int nLength, COORD Coord, LPDWORD lpNumberOfAttrsWritten = NULL) const
	{
		_ASSERTE(m_hConsole!=INVALID_HANDLE_VALUE);
		_ASSERTE(lpAttribute);
		DWORD dwDummy = 0;
		if( lpNumberOfAttrsWritten == NULL ) lpNumberOfAttrsWritten = &dwDummy;
		return ::WriteConsoleOutputAttribute(m_hConsole, (WORD*) lpAttribute, (DWORD) nLength, Coord, lpNumberOfAttrsWritten);
	}
	BOOL SetCtrlHandler(PHANDLER_ROUTINE pHandlerProc, BOOL bAdd = TRUE) const
	{
		_ASSERTE(!::IsBadCodePtr((FARPROC)pHandlerProc));
		return ::SetConsoleCtrlHandler(pHandlerProc, bAdd);
	}
	static BOOL GenerateCtrlEvent(DWORD dwCtrlEvent, DWORD dwProcessGroupId = 0)
	{
		_ASSERTE(dwCtrlEvent==CTRL_C_EVENT || dwCtrlEvent==CTRL_BREAK_EVENT);
		return ::GenerateConsoleCtrlEvent(dwCtrlEvent, dwProcessGroupId);
	}
	operator HANDLE() const { return m_hConsole; }
};

//------------------------------------------------------------------------------------------------------------
// THE END
//------------------------------------------------------------------------------------------------------------
#endif // !defined(AFX_CONSOLESCREEN_H__20010922_A5C5_0E5D_0722_0080AD509054__INCLUDED_)

