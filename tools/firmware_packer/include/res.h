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
// File    : res.h                                                                                          //
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


#ifndef __LION_RES__H__
#define __LION_RES__H__     1

#include "pack.h"

//------------------------------------------------------------------------------------------------------------
//��Դ���ݽ���
//------------------------------------------------------------------------------------------------------------
#define RES_SEC_NAME_SIZE	8			//
#define RES_SEC_NAME		".resthem"	//8�ֽڳ���
#define RES_VERSION			0x0100		//��Դ���ݽڸ�ʽ�汾
#define RES_ALIGN			32


//------------------------------------------------------------------------------------------------------------
//��Դ�ļ�����
//------------------------------------------------------------------------------------------------------------
#define SYSRES 				0
#define LIBRES  			1
#define APPRES				2			



//------------------------------------------------------------------------------------------------------------
//��Դ����ͷ����
//------------------------------------------------------------------------------------------------------------
#ifdef WIN32_DATA_PACK
#pragma pack(push, 1)
#endif
typedef __PACKED struct tag_RESHEAD{
	s8		SecName[8];		    //����
	u16		version;		    //�汾0x0100
	u16		size;			    //ͷ���ݽṹ��С	
	u32		StyleOffset;	    //������ݽṹƫ����
	u32		StyleNum;		    //������
    u32		ResNum;		        //��Դ����
	u32		align;			    //���ݱ߽����ģʽ
	u32		flags;			    //��־��SYSRES LIBRES  APPRES
}RESHEAD;
#ifdef WIN32_DATA_PACK
#pragma pack(pop)
#endif

//------------------------------------------------------------------------------------------------------------
//���ṹ�Ĵ�С
#define RESHEAH_SIZE	sizeof(RESHEAD)	//32 byte




//------------------------------------------------------------------------------------------------------------
//������������
//------------------------------------------------------------------------------------------------------------
#define STYLE_NAME_SIZE 18

#ifdef WIN32_DATA_PACK
#pragma pack(push, 1)
#endif
typedef __PACKED struct tag_STYLE{
	u16		ID;					//���ID�� Ψһ
	s8		Name[STYLE_NAME_SIZE];//������ƣ�ASCII��	
	u32		ResTableOffset;	    //��Դ����ʼ��ַƫ����
	u32		ResOffset;		    //��Դ������ʼ��ַƫ����
	u32		ResSize;			//��Դ���ݳ���
}STYLE;
#ifdef WIN32_DATA_PACK
#pragma pack(pop)
#endif

//------------------------------------------------------------------------------------------------------------
//���ṹ�Ĵ�С
#define STYLE_SIZE	sizeof(STYLE)	//32 byte



//------------------------------------------------------------------------------------------------------------
//��Դ��������
//------------------------------------------------------------------------------------------------------------
#define RES_NAME_SIZE    20

#ifdef WIN32_DATA_PACK
#pragma pack(push, 1)
#endif
typedef __PACKED struct tag_RESITEM{	
    u16		type;			    //��Դ����
	u16		ID;				    //��ԴID,��ͬ������ͬ��ԴID��ͬ
	s8		Name[RES_NAME_SIZE];//��Դ���ƣ����ƿ����ظ�
	u32		offset;		        //��Դ��ʼ��ַ��res�ڵ�ƫ��
	u32		size;			    //��Դ��size
}RESITEM;
#ifdef WIN32_DATA_PACK
#pragma pack(pop)
#endif

//------------------------------------------------------------------------------------------------------------
//���ṹ�Ĵ�С
#define RESITEM_SIZE	sizeof(RESITEM) //32


//------------------------------------------------------------------------------------------------------------
// THE END !
//------------------------------------------------------------------------------------------------------------

#endif //__LION_RES__H__

