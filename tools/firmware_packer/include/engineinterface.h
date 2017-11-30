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
// File    : engineinterface.h                                                                              //
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


#ifndef ___ENGINE__INTERFACE__H__
#define ___ENGINE__INTERFACE__H__   1

#include "basetypes.h"
//#include "languageversion.h"


//------------------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------------------
typedef int (*PCFUNCTION)(void/*lua_State*/ *L);

typedef  u32 (*pEngine_Open)(u32 index);

typedef  u32 (*pEngine_Close)(u32 index);

typedef  u32 (*pEngine_CloseAll)();

typedef  u32 (*pEngine_Run)(u32 index, s8 * script);

typedef  s8 * (*pGetGlobalString)(u32 index, s8 * string);

typedef u32 (*pGetGlobalNumber)(u32 index, s8 * number, df * pNumber);

typedef  s8 * (*pGetTableString)(u32 index, s8 * table, s8 * string);

typedef u32 (*pGetTableNumber)(u32 index, s8 * table, s8 * number, df * pNumber);

typedef s8 * (*pGetTableSubtableString)(u32 index, s8 * table, s8 *subtable, s8 *string) ;

typedef u32 (*pGetTableSubtableNumber)(u32 index, s8 * table, s8 *subtable, s8 *number, df * pNumber);

typedef u32 (*pRegister)(u32 index, s8 * FunctionName, PCFUNCTION Function);

typedef u32 (*pl_setpath)(u32 index, s8 * dragonpath);

typedef u32 (*pl_run)(u32 index);
						
						
//------------------------------------------------------------------------------------------------------------
///Engine服务的接口
//------------------------------------------------------------------------------------------------------------
#pragma pack(push, 1)
typedef	struct tag_Engine
{
	pEngine_Open            Engine_Open;                                                               
    pEngine_Close           Engine_Close;                                                      
    pEngine_CloseAll        Engine_CloseAll;
    pEngine_Run             Engine_Run;                                                       
    pGetGlobalString        GetGlobalString;                                                       
    pGetGlobalNumber        GetGlobalNumber;                                                        
    pGetTableString         GetTableString;                                                        
    pGetTableNumber         GetTableNumber;                                                  
    pGetTableSubtableString GetTableSubtableString;                                                   
    pGetTableSubtableNumber GetTableSubtableNumber;
    pRegister               Register;
	pl_setpath				l_setpath;
	pl_run					l_run;
}Engine_t;
#pragma pack(pop)


//------------------------------------------------------------------------------------------------------------
// THE END
//------------------------------------------------------------------------------------------------------------
#endif //___ENGINE__INTERFACE__H__
