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
//                               (c) Copyright 2006-2007, Scottyu China                                     //
//                                                                                                          //
//                                           All Rights Reserved                                            //
//                                                                                                          //
// File    : strdata.cpp                                                                                    //
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

#ifndef __COMMON__STR_DATA___C___
#define __COMMON__STR_DATA___C___	1



#include "strdata.h"

#include <stdlib.h>


//------------------------------------------------------------------------------------------------------------
//
// 函数说明
//     十六进制字符串转换为dword数据  0xhhhhhhhh
//
// 参数说明
//     
//
// 返回值
//     
//
// 其他
//    无
//
//------------------------------------------------------------------------------------------------------------
long hexstr2dword(char * hexstr)
{
    long ul = 0;
    char *stopstring;
    ul = strtoul( hexstr, &stopstring, 16 );
    return ul;
}

//------------------------------------------------------------------------------------------------------------
//
// 函数说明
//     
//
// 参数说明
//     
//
// 返回值
//     
//
// 其他
//    无
//
//------------------------------------------------------------------------------------------------------------
long decstr2dword(char * decstr)
{
	long ul = 0;
    char *stopstring;
    ul = strtoul( decstr, &stopstring, 10);
    return ul;
}


//------------------------------------------------------------------------------------------------------------
//
// 函数说明
//     
//
// 参数说明
//     
//
// 返回值
//     
//
// 其他
//    无
//
//------------------------------------------------------------------------------------------------------------
BOOL IsValidHex32Str(const char *szHexStr)
{
	int len = strlen(szHexStr);
	char ch;
	BOOL bHexStr = TRUE;
	if (len > 8 || len <= 0)
		return FALSE;
	for (int i = 0; i < len ; i++)
	{
		ch = szHexStr[i];
		if (ch >= '0' && ch <= '9')
		{			
		}
		else if (ch >= 'a' && ch <= 'f')
		{			
		}
		else if (ch >= 'A' && ch <= 'F')
		{			
		}		
		else
		{
			bHexStr = FALSE;
		}
	}
	return bHexStr;
}

//------------------------------------------------------------------------------------------------------------
//
// 函数说明
//     
//
// 参数说明
//     
//
// 返回值
//     
//
// 其他
//    无
//
//------------------------------------------------------------------------------------------------------------
BOOL IsValidDecStr(const char *szDecStr)
{
	int len = strlen(szDecStr);
	char ch;
	BOOL bValid = TRUE;
	if (len > 8 || len <= 0)
		return FALSE;
	for (int i = 0; i < len ; i++)
	{
		ch = szDecStr[i];
		if (ch >= '0' && ch <= '9')
		{			
		}			
		else
		{
			bValid = FALSE;
		}
	}
	return bValid;
}

//------------------------------------------------------------------------------------------------------------
//
// 函数说明
//     
//
// 参数说明
//     
//
// 返回值
//     
//
// 其他
//    无
//
//------------------------------------------------------------------------------------------------------------
DWORD HexStr2DWORD(const char * Hexstr)
{
	int len = strlen(Hexstr);
	DWORD dwValue, dwChar;
	char c;
	dwValue = 0;
	for (int i = 0; i < len; i++)
	{
		c = Hexstr[i];
		if (c >= '0' && c <= '9')
		{
			dwChar = c - '0';
			dwValue = dwValue * 16 + dwChar;
		}
		else if (c >= 'a' && c <= 'f')
		{
			dwChar = c - 'a' + 10;
			dwValue = dwValue * 16 + dwChar;
		}
		else if (c >= 'A' && c <= 'F')
		{
			dwChar = c - 'A' + 10;
			dwValue = dwValue * 16 + dwChar;
		}		
		else
		{	
			//::AfxMessageBox("非法的HexString!");
			return 0;
		}
	}
	return dwValue;	
}

//------------------------------------------------------------------------------------------------------------
//
// 函数说明
//     
//
// 参数说明
//     
//
// 返回值
//     
//
// 其他
//    无
//
//------------------------------------------------------------------------------------------------------------
DWORD DecStr2DWORD(const char * Decstr)
{
	int len = strlen(Decstr);
	DWORD dwValue, dwChar;
	char c;
	dwValue = 0;
	for (int i = 0; i < len; i++)
	{
		c = Decstr[i];
		if (c >= '0' && c <= '9')
		{
			dwChar = c - '0';
			dwValue = dwValue * 10 + dwChar;
		}		
		else
		{	
			//::AfxMessageBox("非法的DecString!");
			return 0;
		}
	}
	return dwValue;	
}


/*
//------------------------------------------------------------------------------------------------------------
//
// 函数说明
//     
//
// 参数说明
//     
//
// 返回值
//     
//
// 其他
//    无
//
//------------------------------------------------------------------------------------------------------------
BOOL HexStringToDWORD(CString str, DWORD  &dwData)
{
	if (!IsValidHex32Str((LPCTSTR)str))
		return FALSE;
	dwData = HexStr2DWORD((LPCTSTR)str);
	return TRUE;
}

//------------------------------------------------------------------------------------------------------------
//
// 函数说明
//     
//
// 参数说明
//     
//
// 返回值
//     
//
// 其他
//    无
//
//------------------------------------------------------------------------------------------------------------
BOOL DecStringToDWORD(CString str, DWORD  &dwData)
{
	if (!IsValidDecStr((LPCTSTR)str))
		return FALSE;

	dwData = DecStr2DWORD((LPCTSTR)str);
	return TRUE;
}
*/




//------------------------------------------------------------------------------------------------------------
//
// 函数说明
//     十六进制字符串转换为dword数据  0xhhhhhhhh
//
// 参数说明
//     
//
// 返回值
//     
//
// 其他
//    无
//
//------------------------------------------------------------------------------------------------------------
/*
DWORD hexstr2dword(char * hexstr)
{
	DWORD dwData = 0;
	dwData = HexStr2DWORD(hexstr + 2);
	return dwData;
}
*/

//------------------------------------------------------------------------------------------------------------
// THE END !
//------------------------------------------------------------------------------------------------------------
#endif //__COMMON__STR_DATA___C___

