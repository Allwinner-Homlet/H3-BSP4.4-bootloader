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
// File    : string.cpp                                                                                     //
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

#ifndef __COMMON__STRING___C_____
#define __COMMON__STRING___C_____	1

#include <string.h>


//------------------------------------------------------------------------------------------------------------
//
// 函数说明
//     lower To Upper
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
void ToUpperCase( s8 * str )
{
    
    u32 len = strlen((const char *)str);
    s8 *p = str;
    u32 i = 0;
    if (!p)
        return ;
        
    for (i = 0; i < len; i++)
    {
        if (islower(p[i]))
		{
            p[i] = toupper(p[i]);
		}
    }
    return ;   
}




//------------------------------------------------------------------------------------------------------------
//
// 函数说明
//     . To _
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
void DotToSlash(s8 * str)
{
    u32 len = strlen((const char *)str);
    s8 *p = str;
    u32 i = 0;
    if (!p)
        return ;
        
    for (i = 0; i < len; i++)
    {
        if ( '.' == p[i])
		{
            p[i] = '_';
		}
    }
    return ;  
}

//------------------------------------------------------------------------------------------------------------
//
// 函数说明
//     Returns OK if the given character is whitespace, or zero otherwise.
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
u32 IsCharWhitespace ( char cChar )
{
	// Return true if the character is a space or tab.
	if ( cChar == ' ' || cChar == '\t' )
		return OK;
	else
		return __LINE__;
}

//------------------------------------------------------------------------------------------------------------
//
// 函数说明
//     Returns OK if the given character is numeric, or zero otherwise.
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
u32 IsCharNumeric ( char cChar )
{
	// Return true if the character is between 0 and 9 inclusive.
	if ( cChar >= '0' && cChar <= '9' )
		return OK;
	else
		return __LINE__;
}


//------------------------------------------------------------------------------------------------------------
//
// 函数说明
//     Returns OK if the given character is part of a valid identifier, meaning it's an
//     alphanumeric or underscore. Zero is returned otherwise.
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
u32 IsCharIdent ( char cChar )
{
	// Return true if the character is between 0 or 9 inclusive or is an uppercase or
	// lowercase letter or underscore
	if ( ( cChar >= '0' && cChar <= '9' ) ||
		 ( cChar >= 'A' && cChar <= 'Z' ) ||
		 ( cChar >= 'a' && cChar <= 'z' ) ||
		   cChar == '_' )
		return OK;
	else
		return __LINE__;
}

//------------------------------------------------------------------------------------------------------------
//
// 函数说明
//     Return OK if the given character is a token delimeter, and return nonzero otherwise
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
u32 IsCharDelimiter ( char cChar )
{
	// Return true if the character is a delimiter
	if ( cChar == ':' || cChar == ',' || cChar == '"' ||
		 cChar == '[' || cChar == ']' ||
		 cChar == '{' || cChar == '}' ||
         OK    == IsCharWhitespace ( cChar ) || 
         cChar == '\n' )
		return OK;
	else
		return __LINE__;
}


//------------------------------------------------------------------------------------------------------------
//
// 函数说明
//     Trims whitespace off both sides of a string.
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
void TrimWhitespace ( char * pstrString )
{
	unsigned int iStringLength = strlen ( pstrString );
	unsigned int iPadLength;
	unsigned int iCurrCharIndex;
	
	if ( iStringLength > 1 )
	{
		// First determine whitespace quantity on the left
		for ( iCurrCharIndex = 0; iCurrCharIndex < iStringLength; ++ iCurrCharIndex )
			if (OK  !=  IsCharWhitespace ( pstrString [ iCurrCharIndex ] ) )
				break;
				
		// Slide string to the left to overwrite whitespace
		iPadLength = iCurrCharIndex;
		if ( iPadLength )
		{
			for ( iCurrCharIndex = iPadLength; iCurrCharIndex < iStringLength; ++ iCurrCharIndex )
				pstrString [ iCurrCharIndex - iPadLength ] = pstrString [ iCurrCharIndex ];

			for ( iCurrCharIndex = iStringLength - iPadLength; iCurrCharIndex < iStringLength; ++ iCurrCharIndex )
				pstrString [ iCurrCharIndex ] = ' ';
		}
		
		// Terminate string at the start of right hand whitespace
		for ( iCurrCharIndex = iStringLength - 1; iCurrCharIndex > 0; -- iCurrCharIndex )
		{
			if (OK != IsCharWhitespace ( pstrString [ iCurrCharIndex ] ) )
			{
				pstrString [ iCurrCharIndex + 1 ] = '\0';
				break;
			}
		}
	}
}


//------------------------------------------------------------------------------------------------------------
//
// 函数说明
//     Returns OK if the given string is whitespace, or nonzero otherwise.
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
u32 IsStringWhitespace ( char * pstrString )
{
	// If the string is NULL, return false
	if ( ! pstrString )
		return __LINE__;
		
	// If the length is zero, it's technically whitespace
	if ( strlen ( pstrString ) == 0 )
		return OK;
		
	// Loop through each character and return false if a non-whitespace is found
	for ( unsigned int iCurrCharIndex = 0; iCurrCharIndex < strlen ( pstrString ); ++ iCurrCharIndex )
		if (OK != IsCharWhitespace ( pstrString [ iCurrCharIndex ] ) && pstrString [ iCurrCharIndex ] != '\n' )
			return __LINE__;
			
	// Otherwise return true
	return OK;
}


//------------------------------------------------------------------------------------------------------------
//
// 函数说明
//     Returns OK if the given string is composed entirely of valid identifier
//     characters and begins with a letter or underscore. Zero is returned otherwise.
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
u32 IsStringIdent ( char * pstrString )
{
	// If the string is NULL return false
	if ( ! pstrString )
		return __LINE__;
		
	// If the length of the string is zero, it's not a valid identifier
	if ( strlen ( pstrString ) == 0 )
		return __LINE__;
		
	// If the first character is a number, it's not a valid identifier
	if ( pstrString [ 0 ] >= '0' && pstrString [ 0 ] <= '9' )
		return __LINE__;
		
	// Loop through each character and return zero upon encountering the first invalid identifier
	// character
	for ( unsigned int iCurrCharIndex = 0; iCurrCharIndex < strlen ( pstrString ); ++ iCurrCharIndex )
		if (OK != IsCharIdent ( pstrString [ iCurrCharIndex ] ) )
			return __LINE__;
			
	// Otherwise return true
	return OK;
}


//------------------------------------------------------------------------------------------------------------
//
// 函数说明
//     Returns OK if the given string is composed entire of integer characters, or zero otherwise.
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
u32 IsStringInteger ( char * pstrString )
{
	// If the string is NULL, it's not an integer
	if ( ! pstrString )
		return __LINE__;
		
	// If the string's length is zero, it's not an integer
	if ( strlen ( pstrString ) == 0 )
		return __LINE__;
		
	unsigned int iCurrCharIndex;
	// Loop through the string and make sure each character is a valid number or minus sign
	for ( iCurrCharIndex = 0; iCurrCharIndex < strlen ( pstrString ); ++ iCurrCharIndex )
		if (OK != IsCharNumeric ( pstrString [ iCurrCharIndex ] ) && ! ( pstrString [ iCurrCharIndex ] == '-' ) )
			return __LINE__;
			
	// Make sure the minus sign only occured at the first character
	for ( iCurrCharIndex = 1; iCurrCharIndex < strlen ( pstrString ); ++ iCurrCharIndex )
		if ( pstrString [ iCurrCharIndex ] == '-' )
			return __LINE__;

	return OK;
}

//------------------------------------------------------------------------------------------------------------
//
// 函数说明
//     Returns OK if the given string is composed entire of float characters, or zero otherwise
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
u32 IsStringFloat( char * pstrString )
{
	if ( ! pstrString )
		return __LINE__;
		
	if ( strlen ( pstrString ) == 0 )
		return __LINE__;
		
	// First make sure we've got only numbers and radix points
	unsigned int iCurrCharIndex;
	for ( iCurrCharIndex = 0; iCurrCharIndex < strlen ( pstrString ); ++ iCurrCharIndex )
		if (OK != IsCharNumeric ( pstrString [ iCurrCharIndex ] ) && 
			! ( pstrString [ iCurrCharIndex ] == '.' ) && 
			! ( pstrString [ iCurrCharIndex ] == '-' ) )
			return __LINE__;
			
	// Make sure only one radix point is present
	int iRadixPointFound = 0;
	for ( iCurrCharIndex = 0; iCurrCharIndex < strlen ( pstrString ); ++ iCurrCharIndex )
		if ( pstrString [ iCurrCharIndex ] == '.' )
			if ( iRadixPointFound )
				return __LINE__;
			else
				iRadixPointFound = 1;
	// Make sure the minus sign only appears in the first character
	for ( iCurrCharIndex = 1; iCurrCharIndex < strlen ( pstrString ); ++ iCurrCharIndex )
		if ( pstrString [ iCurrCharIndex ] == '-' )
			return __LINE__;
			
	// If a radix point was found, return true; otherwise, it must be an integer so return false
	if ( iRadixPointFound )
		return OK;
	else
		return __LINE__;
}

//------------------------------------------------------------------------------------------------------------
// THE END !
//------------------------------------------------------------------------------------------------------------


#endif //__COMMON__STRING___C_____

