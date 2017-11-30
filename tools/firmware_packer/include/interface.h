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
// File    : interface.h                                                                                    //
// By      : scottyu                                                                                        //
// Version : V1.00                                                                                          //
// Time    : 2007-12-13 16:38:36                                                                            //
//                                                                                                          //
//----------------------------------------------------------------------------------------------------------//
//                                                                                                          //
// HISTORY                                                                                                  //
//                                                                                                          //
// 1 2007-12-13 16:38:36                                                                                    //
//                                                                                                          //
//                                                                                                          //
//                                                                                                          //
//----------------------------------------------------------------------------------------------------------//


#ifndef __INTERFACE__H__
#define __INTERFACE__H__    1

//------------------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------------------
#include "basetypes.h"
#include "interfaceconst.h"
#include "interfacetype.h"
#include "commoninterface.h"
#include "dummyinterface.h"
#include "datacheckinterface.h"
#include "engineinterface.h"
#include "imagebuilderinterface.h"

#ifdef _WINDOWS
#include <windows.h>
#endif
#include "xcomplier_def.h"
//------------------------------------------------------------------------------------------------------------
///接口的统一形式
//------------------------------------------------------------------------------------------------------------
#pragma pack(push, 1)
typedef struct tag_Interface
{
    s8                  Type[TYPE_LEN]; ///
    HINSTANCE           hModule;        ///插件句柄
    u32                 Res;            ///
    Common_t            Common;         ///
    union {
        DataCheck_t     DataCheck;      ///
        Engine_t        Engine;         ///
        ImageBuilder_t  ImageBuilder;   /// 
        Dummy_t         Dummy;          ///
    };
}Interface_t;
#pragma pack(pop)


//------------------------------------------------------------------------------------------------------------
///服务接口
//------------------------------------------------------------------------------------------------------------
#pragma pack(push, 1)
typedef struct tag_InterfaceSet
{
	u16			    Version;	///服务接口的版本
	u16			    Count;		///有效服务接口的数目	
	Interface_t *   Interface[MAX_INTERFACE];///
}InterfaceSet_t;
#pragma pack(pop)


//------------------------------------------------------------------------------------------------------------
// THE END !
//------------------------------------------------------------------------------------------------------------
#endif //__INTERFACE__H__
