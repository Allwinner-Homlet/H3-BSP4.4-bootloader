//----------------------------------------------------------------------------------------------------------//
//                                                                                                          //
//                                                                                                          //
//                                                                                                          //
//                                                                                                          //
//                                                                                                          //
//                                                                                                          //
//              LLMMLL                                                                                      //
//EEMMMMMMMMEE    LLLL                                                            LLFF                      //
//  FFI     LLFF  LLLL                                                                                      //
//  FFI     I EE  LLFFBBMMMMI      FFMMMMBBI      BBMMMMBBI   IMMBBBBMMMMI     IMMMMFF      IBBMMLL  BBMMFF //
//  FFI     I LL  LLMMI   I EE   FFFF    I BBI  EEI     I EEI  IBBI   I EE        IFF        IBBI   BBLL    //
//  EEMMMMMMBBI   LLLL      EEI IEE        I LL BBMMMMMMMMMMI  IFF      EEI       IFF            BBMMI      //
//  FFI           LLLL      EEI IEE        I LL FF             IFF      EEI       IFF          IMMMMFF      //
//  FFI           LLLL      EEI  FFI       FFI  FFLL           IFF      EEI       IFF        LLBBI   FFEE   //
//EEMMMMMMLL    LLMMMMLL  BBMMBBII BBMMMMMMLL     EEMMMMMMMMI IMMMMFF  BBMMBBIEEMMMMMMMMMMILLMMMMLL  BBMMBBI//
//                                                                                                          //
//                                                                                                          //
//----------------------------------------------------------------------------------------------------------//
//                                                                                                          //
//                                              Phoenix System                                              //
//                                                                                                          //
//                               (c) Copyright 2006-2007, Scottyu China                                     //
//                                                                                                          //
//                                           All Rights Reserved                                            //
//                                                                                                          //
// File    : Fexprotocol.h                                                                                  //
// By      : scottyu                                                                                        //
// Version : V1.00                                                                                          //
// Time    : 2008-11-15 14:55:18                                                                            //
//                                                                                                          //
//----------------------------------------------------------------------------------------------------------//
//                                                                                                          //
// HISTORY                                                                                                  //
//                                                                                                          //
// 1 2008-11-15 14:55:21                                                                                    //
//                                                                                                          //
//                                                                                                          //
//                                                                                                          //
//----------------------------------------------------------------------------------------------------------//


#ifndef __FEX_PROTOCOL_H__
#define __FEX_PROTOCOL_H__	1

//------------------------------------------------------------------------------------------------------------
//                                          �����Э��
//------------------------------------------------------------------------------------------------------------

#define MAX_TRANSFER_LENGTH				32768	//32K

#define TRANSFERDATA_DIRECTION_SEND		0x12	//�������ݵ��豸
#define TRANSFERDATA_DIRECTION_RECV		0x11	//���豸��������

#pragma pack(push, 1) 
typedef struct tag_TRANSFERDATA
{
	BYTE	Direction;					///
	BYTE	resv;						///
	DWORD	DataLen;					///
	BYTE    Resv2[6];					///
}tTransferData;
#pragma pack(pop) 

//------------------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------------------
#define TRANSFER_CBW_MAGIC				0x43555741	///"AWUC" lsb ��ת

#pragma pack(push, 1) 
typedef struct tag_TRANSFERCBW
{
	DWORD	Magic;						///TRANSFER_CBW_MAGIC
	DWORD	Tag;						///
	DWORD	DataTransferLen;			///
	BYTE	reserved[3];				///
	BYTE	Cmd_Len;					///sizeof(tTransferData)
	union{
		tTransferData Cmd_package;		///
		BYTE dummy[16];					///
	};
}tTransferCBW;
#pragma pack(pop) 


//------------------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------------------
#define TRANSFER_CSW_MAGIC				0x53555741	///"AWUS" lsb ��ת

#define CSWStatus_SUCCESS				0
#define CSWStatus_FAILED				1


//------------------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------------------
#pragma pack(push, 1) 
typedef struct tag_TRANSFERCSW
{
	DWORD	Magic;						///TRANSFER_CSW_MAGIC
	DWORD	Tag;						///
	DWORD	DataResidue;				///0
	BYTE	CSWStatus;					///CSWStatus_SUCCESS or CSWStatus_FAILED
}tTransferCSW;
#pragma pack(pop) 


//------------------------------------------------------------------------------------------------------------					 
//                                        Ӧ�ò�Э��
//------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------
// Ӧ�ò�����
//------------------------------------------------------------------------------------------------------------
#define FEX_CMD_verify_dev			0x0001		//	
#define FEX_CMD_switch_role			0x0002		//
#define FEX_CMD_is_ready			0x0003		//
#define FEX_CMD_get_cmd_set_ver		0x0004		//
#define FEX_CMD_disconnect			0x0010		//
#define FEX_CMD_fel_down			0x0101		//fel�׶��������ݵ�����
#define FEX_CMD_fel_run				0x0102		//fel�׶����г��������
#define	FEX_CMD_fel_up				0x0103		//fel�׶��ϴ����ݵ�����
#define FEX_CMD_fes_trans			0x0201		//fes�׶δ������ݵ�����
#define FEX_CMD_fes_run				0x0202		//
#define FEX_CMD_info				0x0203		//
#define FEX_CMD_get_msg				0x0204		//


//------------------------------------------------------------------------------------------------------------
///Ӧ�ò��������ݽṹ
//------------------------------------------------------------------------------------------------------------



//------------------------------------------------------------------------------------------------------------
///FEX_CMD_verify_dev����׶�ר������
//------------------------------------------------------------------------------------------------------------
#pragma pack(push, 1) 
typedef	struct tag_VERIFY_DEV
{
	BYTE	reserved[12];		///
}verify_dev_t;
#pragma pack(pop) 



//------------------------------------------------------------------------------------------------------------
///FEX_CMD_switch_role����׶�ר������
//------------------------------------------------------------------------------------------------------------
#pragma pack(push, 1) 
typedef struct tag_SWITCH_ROLE
{
	WORD	state;				///
	BYTE	reserved[10];		///
}switch_role_t;
#pragma pack(pop) 



//------------------------------------------------------------------------------------------------------------
///FEX_CMD_is_ready����׶�ר������
//------------------------------------------------------------------------------------------------------------
#pragma pack(push, 1) 
typedef struct tag_IS_READY
{
	WORD	state;				///
	BYTE	reserved[10];		///
}is_ready_t;
#pragma pack(pop) 

//------------------------------------------------------------------------------------------------------------
///FEX_CMD_get_cmd_set_ver����׶�ר������
//------------------------------------------------------------------------------------------------------------
#pragma pack(push, 1) 
typedef struct tag_GET_CMD_SET_VER
{
	WORD	state;				///
	BYTE	reserved[10];		///
}get_cmd_set_ver_t;
#pragma pack(pop) 


//------------------------------------------------------------------------------------------------------------
///FEX_CMD_disconnect����׶�ר������
//------------------------------------------------------------------------------------------------------------
#pragma pack(push, 1) 
typedef struct tag_DISCONNECT
{
	BYTE	reserved[12];		///
}disconnect_t;
#pragma pack(pop) 


//------------------------------------------------------------------------------------------------------------
///FEX_CMD_fel_down����׶�ר������
//------------------------------------------------------------------------------------------------------------
#pragma pack(push, 1) 
typedef struct tag_FEL_DOWN
{
	DWORD	addr;				///
	DWORD	len;				///
	BYTE	reserved[4];		///
}fel_down_t;
#pragma pack(pop) 


//------------------------------------------------------------------------------------------------------------
///FEX_CMD_fel_run����׶�ר������
//------------------------------------------------------------------------------------------------------------
#pragma pack(push, 1) 
typedef struct tag_FEL_RUN
{
	DWORD	addr;				///		
	BYTE	reserved[8];		///
}fel_run_t;
#pragma pack(pop) 

//------------------------------------------------------------------------------------------------------------
//FEX_CMD_fel_up����׶�ר������
//------------------------------------------------------------------------------------------------------------
#pragma pack(push, 1) 
typedef struct tag_FEL_UP
{
	DWORD	addr;				///
	DWORD	len;				///
	BYTE	reserved[4];		///
}fel_up_t;
#pragma pack(pop) 


//------------------------------------------------------------------------------------------------------------
///media_index ��������
//------------------------------------------------------------------------------------------------------------
#define MEDIA_TYPE_DRAM				0x0
#define MEDIA_TYPE_FLASH_PHY		0x1
#define MEDIA_TYPE_FLASH_LOG		0x2

//------------------------------------------------------------------------------------------------------------
///OOC ��������
//------------------------------------------------------------------------------------------------------------
#define MEDIA_OOC_NULL				0x0
#define MEDIA_OOC_OPEN				0x1
#define MEDIA_OOC_CLOSE				0x2
#define MEDIA_OOC_INVALID			0x3

//------------------------------------------------------------------------------------------------------------
///DOU ��������
//------------------------------------------------------------------------------------------------------------
#define DOU_DEFAULT					0x0
#define DOU_DOWNLOAD				0x1
#define DOU_UPLOAD					0x2
#define DOU_INVALID					0x3

//------------------------------------------------------------------------------------------------------------
///FEX_CMD_fes_trans����׶�ר������
//------------------------------------------------------------------------------------------------------------
#pragma pack(push, 1) 
typedef struct tag_fes_trans
{
	DWORD	addr;				///
	DWORD	len;				///

#pragma pack(push, 1) 
	struct {
		BYTE	logicunit_index : 4;	///��4����
		BYTE	media_index     : 4;	///��4���� 
	}u1;
#pragma pack(pop) 

#pragma pack(push, 1) 
	struct {
		BYTE	res		: 4;	///��4����
		BYTE	DOU		: 2;	///�м�2���� ��ʶ Download Or Upload
		BYTE	OOC     : 2;	///��2����
	}u2;
#pragma pack(pop) 

	BYTE	reserved[2];		///
}fes_trans_t;
#pragma pack(pop) 

//------------------------------------------------------------------------------------------------------------
///para ��������
//------------------------------------------------------------------------------------------------------------
#define FEX_RUN_NO_PATA			0
#define FEX_RUN_HAS_PATA		1


//------------------------------------------------------------------------------------------------------------
///type ��������
//------------------------------------------------------------------------------------------------------------
#define FEX_RUN_TYPE_FET		1
#define FEX_RUN_TYPE_GEN_CODE	2
#define FEX_RUN_TYPE_FED		3
#define FEX_RUN_TYPE_RES		0


//------------------------------------------------------------------------------------------------------------
///FEX_CMD_fes_run����׶�ר������
//------------------------------------------------------------------------------------------------------------
#pragma pack(push, 1) 
typedef struct tag_FES_RUN
{
	DWORD	addr;				///
#pragma pack(push, 1) 
	struct {
		BYTE	para : 1;		///
		BYTE	res  : 3;		///
		BYTE	type : 4;		///
	}u;
#pragma pack(pop) 
				
	BYTE	reserved[7];		///
}fes_run_t;
#pragma pack(pop) 

//------------------------------------------------------------------------------------------------------------
///FEX_CMD_info����׶�ר������
//------------------------------------------------------------------------------------------------------------
#define	INFO_CMD_TYPE_GET_INFO	0x00    ///hgl
#define	INFO_CMD_TYPE_PHOENIX	0x01    ///hgl

#pragma pack(push, 1) 
typedef struct tag_INFO
{
#pragma pack(push, 1) 
	struct {
		BYTE	res		: 3;	///
		BYTE	get_set	: 1;	///
		BYTE	type	: 4;	///
	}u;
#pragma pack(pop) 
	BYTE	reserved1[3];		///
	DWORD	ops_addr;			///
	BYTE	reserved2[4];		///
}info_t;
#pragma pack(pop) 

//------------------------------------------------------------------------------------------------------------
///FEX_CMD_get_msg����׶�ר������
//------------------------------------------------------------------------------------------------------------
#pragma pack(push, 1) 
typedef struct tag_GET_MSG
{
	DWORD	msg_len;			///
	BYTE	reserved[8];		///
}get_msg_t;
#pragma pack(pop) 

//------------------------------------------------------------------------------------------------------------
//FEX�����
//------------------------------------------------------------------------------------------------------------
#pragma pack(push, 1) 
typedef struct tag_FEXCOMMAND
{
	WORD	command;					///
	WORD	tag;						///
	union	{
		verify_dev_t		verify_dev;			///
		switch_role_t		switch_role;		///
		is_ready_t			is_ready;			///
		get_cmd_set_ver_t	get_cmd_set_ver;	///
		disconnect_t		disconnect;			///
		fel_down_t			fel_down;			///
		fel_run_t			fel_run;			///
		fel_up_t			fel_up;				///
		fes_trans_t			fes_trans;			///
		fes_run_t			fes_run;			///
		info_t				info;				///
		get_msg_t			get_msg;			///
	};
}FexCommand_t;
#pragma pack(pop) 

//------------------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------------------
#define VERIFY_DEV_DATA_TAG				"AWUSBFEX"

#define AL_VERIFY_DEV_MODE_NULL			0x00	///
#define AL_VERIFY_DEV_MODE_FEL			0x01	///
#define AL_VERIFY_DEV_MODE_SRV			0x02	///
#define AL_VERIFY_DEV_MODE_UPDATE_COOL	0x03	///
#define AL_VERIFY_DEV_MODE_UPDATE_HOT	0x04	///


//------------------------------------------------------------------------------------------------------------
//							Ӧ�ò����ݽ׶� ���ݽṹ
//------------------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------------------
//FEX_CMD_verify_dev�������ݽ׶����ݸ�ʽ
//------------------------------------------------------------------------------------------------------------
#define PHOENIX_DATA_SUPPORT			'D'

#pragma pack(push, 1)
typedef struct tag_VERIFY_DEV_DATA
{
	BYTE	tag[8];						///"AWUSBFEX"
	DWORD	Platform_id_hw;				///Ӳ��ƽ̨ID
	DWORD	Platform_id_fw;				///���ƽ̨ID
	WORD	mode;						///ģʽ
	BYTE	Phoenix_data_flag;			///phoenix˽�����ݱ�־
	BYTE	Phoenix_data_len;			///phoenix˽�����ݳ���
	DWORD	Phoenix_data_addr;			///phoenix˽�����ݵ�ַ
	BYTE	rev[8];						///����
}verify_dev_Data_t;
#pragma pack(pop) 

//------------------------------------------------------------------------------------------------------------
//FEX_CMD_is_ready�������ݽ׶����ݸ�ʽ
//------------------------------------------------------------------------------------------------------------
#pragma pack(push, 1)
typedef struct tag_IS_READY_DATA
{	
	WORD	State;						///
	WORD	Interval;					///
	BYTE	rev[16];					///
}is_ready_Data_t;
#pragma pack(pop) 


//------------------------------------------------------------------------------------------------------------
//FEX_CMD_get_cmd_set_ver�������ݽ׶����ݸ�ʽ
//------------------------------------------------------------------------------------------------------------
#pragma pack(push, 1)
typedef struct tag_GET_CMD_SET_VER_DATA
{	
	WORD	Version_high;				///
	WORD	Version_low;				///
	BYTE	rev[12];					///
}get_cmd_set_ver_Data_t;
#pragma pack(pop) 

//------------------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------------------
#define	INFO_DATA_STATE_IDLE	0x00    ///
#define	INFO_DATA_STATE_BUSY	0x01    ///
#define	INFO_DATA_STATE_ERROR	0x02    ///

#define	INFO_DATA_HAVE_MSG_TRUE	0x01    ///
#define	INFO_DATA_HAVE_MSG_FAIL	0x00    ///

//------------------------------------------------------------------------------------------------------------
//FEX_CMD_info�������ݽ׶����ݸ�ʽ
//------------------------------------------------------------------------------------------------------------
#pragma pack(push, 1)
typedef struct tag_INFO_DATA
{	
	BYTE	state;						///
	BYTE	have_msg;					///
	BYTE	rev[30];					///
}info_Data_t;
#pragma pack(pop) 


//------------------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------------------
#define	MSG_DATA_HAVE_OTHER_FAIL	0x00    ///
#define	MSG_DATA_HAVE_OTHER_TRUE	0x01    ///
    
#define	MSG_DATA_MSG_TYPE_DMSG		0x00    ///
#define	MSG_DATA_MSG_TYPE_DATA		0x01    ///

#define	MSG_DATA_MSG_TYPE_OFFSET	0x04    ///


//------------------------------------------------------------------------------------------------------------
//FEX_CMD_get_msg�������ݽ׶����ݸ�ʽ
//------------------------------------------------------------------------------------------------------------
#pragma pack(push, 1)
typedef struct tag_GET_MSG_DATA
{	
	WORD	msg_len;					///hgl
	BYTE	have_other;					///
	struct
	{
		BYTE	rev		: 4;			///
		BYTE	msg_type: 4;			///
	};
	BYTE	reserved[20];				///
	BYTE	msg_body[1000];				///
}get_msg_Data_t;
#pragma pack(pop) 



//------------------------------------------------------------------------------------------------------------
//FEX_CMD_fex_run�������ݽ׶����ݸ�ʽ
//------------------------------------------------------------------------------------------------------------
#pragma pack(push, 1)
typedef struct tag_FEX_RUN_DATA
{	
	DWORD	Para0;						///
	DWORD	Para1;						///
	DWORD	Para2;						///
	DWORD	Para3;						///
}fex_run_Data_t;
#pragma pack(pop) 


//------------------------------------------------------------------------------------------------------------
///Ӧ�ò�״̬ ���ݽṹ
//------------------------------------------------------------------------------------------------------------

#define STATUS_SUCCESS					0
#define STATUS_FAILED					1


//------------------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------------------
#pragma pack(push, 1)
typedef struct tag_STATUS
{
	WORD	mark;						///0xffff
	WORD	tag;						///
	BYTE	state;						///STATUS_SUCCESS
	BYTE	rev[3];						///
}Status_t;
#pragma pack(pop) 


//------------------------------------------------------------------------------------------------------------
//	THE END !
//------------------------------------------------------------------------------------------------------------

#endif //__FEX_PROTOCOL_H__

