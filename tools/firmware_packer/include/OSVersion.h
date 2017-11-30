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
// File    : OSVersion.h                                                                                    //
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


#if !defined(AFX_OSVERSION_H__2C39BF7E_6AE7_49C5_A1A2_C41F0587C941__INCLUDED_)
#define AFX_OSVERSION_H__2C39BF7E_6AE7_49C5_A1A2_C41F0587C941__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//------------------------------------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------------------------------------
typedef enum {
	win31, win95, win98,win2k,winnt,winxp,unknown
}eOSVersion;


//------------------------------------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------------------------------------
typedef enum {
	ansi_unknown	= 0,
	ansi_Thai		= 874,
	ansi_Japan		= 932,
	ansi_ChinesePRC	= 936,
	ansi_Korean		= 949,
	ansi_ChineseTW	= 950,  
	ansi_Unicode	= 1200, 
	ansi_Eastern_European  = 1250,
	ansi_Cyrillic	= 1251,
	ansi_Latin		= 1252, 
	ansi_Greek		= 1253,
	ansi_Turkish	= 1254,
	ansi_Hebrew		= 1255,
	ansi_Arabic		= 1256,
	ansi_Baltic		= 1257	
}eAnsiCode;


//------------------------------------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------------------------------------
class COSVersion  
{
public:

	virtual eAnsiCode GetANSICode();
	virtual eOSVersion GetOSVersion();

	
	COSVersion();
	virtual ~COSVersion();

protected:
	eOSVersion m_version;
	eAnsiCode  m_ansi;
};

//------------------------------------------------------------------------------------------------------------
// THE END !
//------------------------------------------------------------------------------------------------------------

#endif // !defined(AFX_OSVERSION_H__2C39BF7E_6AE7_49C5_A1A2_C41F0587C941__INCLUDED_)
