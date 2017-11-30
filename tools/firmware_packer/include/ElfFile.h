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
//                                                                                                          //
// File    : ElfFile.h                                                                                      //
// By      : scottyu                                                                                        //
// Version : V1.00                                                                                          //
// Time    : 2008-04-25 8:53:38                                                                             //
//                                                                                                          //
//----------------------------------------------------------------------------------------------------------//
//                                                                                                          //
// HISTORY                                                                                                  //
//                                                                                                          //
// 1 2008-04-25 8:55:44                                                                                     //
//                                                                                                          //
//                                                                                                          //
//                                                                                                          //
//----------------------------------------------------------------------------------------------------------//


#if !defined(AFX_ELFFILE_H__89772490_6E64_43BE_92FD_7192F00EFA94__INCLUDED_)
#define AFX_ELFFILE_H__89772490_6E64_43BE_92FD_7192F00EFA94__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "elf.h"

//------------------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------------------
class CElfFile  
{
public:	

//	 virtual CElfFile();	
	
//	 virtual ~CElfFile() = 0;
		
//	virtual CElfFile(LPCTSTR lpszPathName) = 0;
public:	

	//!ȡ��section���ļ��еĳ���
	virtual u32		GetSectionFileLength(u16 wSectionIndex) = 0;

	//!ȡ��section�ĳ���
	virtual u32		GetSectionLength(u16 wSectionIndex) = 0;

	//!�ж��Ƿ�Ϊsection����
	virtual BOOL	IsSectionSymbol(Elf32_Sym * pSymbol) = 0;

	//!�ж��Ƿ�Ϊ�ļ�����
	virtual BOOL	IsFileSymbol(Elf32_Sym * pSymbol) = 0;

	//!�ж��Ƿ�Ϊ��������
	virtual BOOL	IsFunctionSymbol(Elf32_Sym * pSymbol) = 0;

	//!�ж��Ƿ�Ϊ���ݷ���
	virtual BOOL	IsDataSymbol(Elf32_Sym * pSymbol) = 0;

	//!����Programͷ��Ϣ����ļ�ƫ��
	virtual u32		GetProgramTableOffset() = 0;

	//!����sectionͷ��Ϣ����ļ�ƫ��
	virtual u32		GetSectionTableOffset() = 0;

	//!����ȫ��programͷ��Ϣ��ĳ���
	virtual u32		GetProgramTableLength() = 0;

	//!����ȫ��sectionͷ��Ϣ��ĳ���
	virtual u32		GetSectionTableLength() = 0;
	
	//!�ж�ָ��������section�Ƿ�Ϊcomment section
	virtual BOOL	IsCommentSection(u16 wSectionIndex) = 0;

	//!�ж�ָ��������section�Ƿ�Ϊnote section
	virtual BOOL	IsNoteSection(u16 wSectionIndex) = 0;

	//!�ж�ָ��������section�Ƿ�Ϊstring table section
	virtual BOOL	IsStringTableSection(u16 wSectionIndex) = 0;

	//!�ж�ָ��������section�Ƿ�Ϊsymboltable section
	virtual BOOL	IsSymbolTableSection(u16 wSectionIndex) = 0;

	//!�ж�ָ����program�����Ƿ��ںϷ��ķ�Χ
	virtual BOOL	IsValidProgramIndex(u16 wProgramIndex) = 0;

	//!�ж�ָ��������section�Ƿ�Ϊsection����section
	virtual BOOL	IsSectionNameSection(u16 wSectionIndex) = 0;
	
	//!�ж�ָ����section�����Ƿ��ںϷ��ķ�Χ
	virtual BOOL	IsValidSectionIndex(u16 wSectionIndex) = 0;

	//!�ж�ָ����section�Ƿ�Ϊbss��
	virtual BOOL	IsBssSection(u16 wSectionIndex) = 0;

	//!�ж�ָ����section�Ƿ�Ϊdata��
	virtual BOOL	IsDataSection(u16 wSectionIndex) = 0;

	//!�ж�ָ����section�Ƿ�Ϊcode��
	virtual BOOL	IsCodeSection(u16 wSectionIndex) = 0;

	//!ȡ��elf�ļ���section����Ŀ
	virtual u16	 	GetSectionCount() = 0;

	//!ȡ��program�ε���Ŀ
	virtual u16		GetProgramCount() = 0;	
	
	//!ȡ��section����section������
	virtual u16		GetSectionNameTableIndex() = 0;

	//!����ļ�ͷ�����Ƿ�Ϸ���
	virtual BOOL	IsValid(BYTE * image) = 0;
	
	//!���ڴ�ж���Ѿ�ת�ص�elf�ļ�image
	virtual BOOL	UnLoadElfFile() = 0;

	//!ת��elf�ļ����ڴ�
	virtual BOOL	LoadElfFile(LPCTSTR lpszPathName) = 0;
	
	//!ȡ��elf��image�ĳ���
	virtual DWORD	GetImageLength() = 0;

	//!ȡ��ָ������section�������ַ���
	virtual BYTE *	GetSectionName(u16 index) = 0;

	//!ȡ��section����
	virtual BYTE *	GetSectionNameTable() = 0;	

	//!ȡ��section������ָ��
	virtual BYTE *	GetSectionPtr(u16 index) = 0;

	//!ȡ��image��buffer
	virtual BYTE *	GetImageBuf() = 0;

	//!ȡ�÷��ŵ�����
	virtual char *	GetSymbolName(Elf32_Sym *pSymbol, u16 SectionIndex) = 0;

	//!ȡ��elf�ļ�ͷ
	virtual Elf32_Ehdr * GetElfHeader() = 0;

	//!ȡ��ָ��������programͷ��Ϣ
	virtual Elf32_Phdr * GetProgramTable(int index) = 0;

	//!ȡ��sectionĿ¼����ָ��������sectionͷ
	virtual Elf32_Shdr * GetSectionTable(u16 index) = 0;

	//!����ָ������section������
	virtual Elf32_Word	GetSectionType(u16 wSectionIndex) = 0;
	

protected:
	//elf���ʼ��
//	virtual void		Initialize() = 0;

	//����elf�ļ�����·��
//	virtual void		 SetFile(LPCTSTR lpszPathName) = 0;

protected:
	Elf32_Ehdr *m_ElfHead;					//! ELF�ļ�ͷ 
   	char		m_szElfFilePath[MAX_PATH];	//! elf�ļ�·�� 
	BYTE		*m_ElfImage;				//! elf�ļ��ڴ�ӳ��
	DWORD		m_dwImageLength;			//! elf ӳ�񳤶ȣ��ֽڣ�
};


//------------------------------------------------------------------------------------------------------------
// THE END
//------------------------------------------------------------------------------------------------------------
#endif //! !defined(AFX_ELFFILE_H__89772490_6E64_43BE_92FD_7192F00EFA94__INCLUDED_)


