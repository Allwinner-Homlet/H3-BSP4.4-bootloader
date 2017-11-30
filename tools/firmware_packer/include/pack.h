//----------------------------------------------------------------------------------------------------------//
//                                                                                                          //
//                                                                                                          //
//                                                                                                          //
//                                                                                                          //
//                                                                                                          //
//              I MMMMI     LLMMBBI   I BBLL                              LLMMEE                            //
//                FFFF        EELL                                          LLEE                            //
//                LLEEI BBFF  BBI   FFMMMMLL      LLMMEEEEMMFF      I BBMMMMMMEE                            //
//                I BBLLMMBBI BBI       EELL        LLBBI   BBI   I BBLL  I BBEE                            //
//                I BBFFBBBBFFBBI       EELL        LLEE    EELL  LLEE      LLEE                            //
//                I BBMMLLFFMMEE        EELL        LLEE    EELL  LLEE      LLEE                            //
//                  BBBBI I MMEE        EELL        LLEE    EELL  I BBI     BBEE                            //
//                  EEEE    BBFF    I BBMMMMBBI   LLMMMMEEFFMMMMI   I MMMMBBFFMMBBI                         //
//                                                                                                          //
//                                                                                                          //
//                                                                                                          //
//----------------------------------------------------------------------------------------------------------//
//                                                                                                          //
//                                          WindShell System                                                //
//                                                                                                          //
//                               (c) Copyright 2006-2007, Scottyu China                                     //
//                                                                                                          //
//                                         All Rights Reserved                                              //
//                                                                                                          //
// File    : pack.h                                                                                        //
// By      : scottyu                                                                                        //
// Version : V1.00                                                                                          //
// Time    : 2008-04-28 9:34:19                                                                             //
//                                                                                                          //
//----------------------------------------------------------------------------------------------------------//
//                                                                                                          //
// HISTORY                                                                                                  //
//                                                                                                          //
// 1 2008-04-28 9:34:21                                                                                     //
//                                                                                                          //
//                                                                                                          //
//                                                                                                          //
//----------------------------------------------------------------------------------------------------------//

#ifndef ___STRUCT_DATA_PACK_H__
#define ___STRUCT_DATA_PACK_H__		1


#ifdef WIN32
	#ifndef WIN32_DATA_PACK
    #define WIN32_DATA_PACK		1
	#endif //WIN32_DATA_PACK
	
	#ifndef __PACKED
    #define __PACKED   
	#endif //__PACKED
#else
	
	#ifndef __PACKED
    #define __PACKED __packed
	#endif //__PACKED
#endif //_WIN32

//------------------------------------------------------------------------------------------------------------
// THE END !
//------------------------------------------------------------------------------------------------------------

#endif //___STRUCT_DATA_PACK_H__

