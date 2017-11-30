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
// File    : list.h                                                                                         //
// By      : scottyu                                                                                        //
// Version : V1.00                                                                                          //
// Time    : 2008-05-16 9:08:24                                                                             //
//         : 通用的单向链表数据结构处理模块                                                                 //
//----------------------------------------------------------------------------------------------------------//
//                                                                                                          //
// HISTORY                                                                                                  //
//                                                                                                          //
// 1 2008-05-16 9:08:24                                                                                     //
//                                                                                                          //
//                                                                                                          //
//                                                                                                          //
//----------------------------------------------------------------------------------------------------------//


#ifndef __COMMON____LIST___H___
#define __COMMON____LIST___H___	1

//------------------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------------------
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <windows.h>

//------------------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------------------
typedef struct _Node                  // A linked list node
{
    void *         pData;             // Pointer to the node's data
    struct _Node * pNext;             // Pointer to the next node in the list
}Node;

//------------------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------------------
typedef struct tag_List List;

//------------------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------------------
typedef void (*PF_Init)      (List * pList);
typedef void (*PF_Free)      (List * pList);
typedef int  (*PF_AddTail)   (List * pList, void * pData);
typedef u32  (*PF_IsEmpty)   (List * pList);
typedef Node*(*PF_RemoveHead)(List * pList);
typedef int  (*PF_GetCount)  (List * pList);
typedef void (*PF_Print)     (List * pList);

//------------------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------------------
struct tag_List                 // A linked list
{
    Node * pHead,               // Pointer to head node
         * pTail;               // Pointer to tail nail node
    int iNodeCount;             // The number of nodes in the list
    
    BOOL			IsInitial;	//
    
    PF_Init         Init;		//
	PF_Free         Free;		//
	PF_AddTail      AddTail;	//
	PF_IsEmpty      IsEmpty;	//
	PF_RemoveHead   RemoveHead;	//
	PF_GetCount     GetCount;	//
	PF_Print        Print;		//
};

//------------------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------------------
static void _List_Init      (List * pList );
static void _List_Free      (List * pList );
static int  _List_AddTail   (List * pList, void * pData );
static u32  _List_IsEmpty   (List * pList);
static Node*_List_RemoveHead(List * pList);
static int  _List_GetCount  (List * pList);
static void _List_Print     (List * pList);




//------------------------------------------------------------------------------------------------------------
// public list api
//------------------------------------------------------------------------------------------------------------
List * CreateList();

void DestroyList(List * list);

//------------------------------------------------------------------------------------------------------------
// THE END !
//------------------------------------------------------------------------------------------------------------

#endif //__COMMON____LIST___H___


