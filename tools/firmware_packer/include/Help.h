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
// File    : Help.h                                                                                         //
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


#if !defined(AFX_HELP_H__65A40FC1_A2A5_40A4_8194_7B883B03C741__INCLUDED_)
#define AFX_HELP_H__65A40FC1_A2A5_40A4_8194_7B883B03C741__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "basetypes.h"
//------------------------------------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------------------------------------




//------------------------------------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------------------------------------
typedef enum tagHelpType
{
	chmType,
	pdfType,
	docType,
	unkownType
}emHelpType;

u32 ShowHelp(emHelpType type, const char * csHelpFilepath);

/*
//------------------------------------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------------------------------------
class CHelp  
{
public:
	BOOL ShowHelp(emHelpType type, CString csHelpFilepath);
	CHelp();
	virtual ~CHelp();
protected:
	emHelpType m_type;
	CString m_helpfilepath;
};
*/

//------------------------------------------------------------------------------------------------------------
// THE END !
//------------------------------------------------------------------------------------------------------------

#endif // !defined(AFX_HELP_H__65A40FC1_A2A5_40A4_8194_7B883B03C741__INCLUDED_)

