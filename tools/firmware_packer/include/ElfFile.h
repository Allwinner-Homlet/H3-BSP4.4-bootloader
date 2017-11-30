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

	//!取得section在文件中的长度
	virtual u32		GetSectionFileLength(u16 wSectionIndex) = 0;

	//!取得section的长度
	virtual u32		GetSectionLength(u16 wSectionIndex) = 0;

	//!判断是否为section符号
	virtual BOOL	IsSectionSymbol(Elf32_Sym * pSymbol) = 0;

	//!判断是否为文件符号
	virtual BOOL	IsFileSymbol(Elf32_Sym * pSymbol) = 0;

	//!判断是否为函数符号
	virtual BOOL	IsFunctionSymbol(Elf32_Sym * pSymbol) = 0;

	//!判断是否为数据符号
	virtual BOOL	IsDataSymbol(Elf32_Sym * pSymbol) = 0;

	//!返回Program头信息表的文件偏移
	virtual u32		GetProgramTableOffset() = 0;

	//!返回section头信息表的文件偏移
	virtual u32		GetSectionTableOffset() = 0;

	//!返回全部program头信息表的长度
	virtual u32		GetProgramTableLength() = 0;

	//!返回全部section头信息表的长度
	virtual u32		GetSectionTableLength() = 0;
	
	//!判断指定索引的section是否为comment section
	virtual BOOL	IsCommentSection(u16 wSectionIndex) = 0;

	//!判断指定索引的section是否为note section
	virtual BOOL	IsNoteSection(u16 wSectionIndex) = 0;

	//!判断指定索引的section是否为string table section
	virtual BOOL	IsStringTableSection(u16 wSectionIndex) = 0;

	//!判断指定索引的section是否为symboltable section
	virtual BOOL	IsSymbolTableSection(u16 wSectionIndex) = 0;

	//!判断指定的program索引是否在合法的范围
	virtual BOOL	IsValidProgramIndex(u16 wProgramIndex) = 0;

	//!判断指定索引的section是否为section名称section
	virtual BOOL	IsSectionNameSection(u16 wSectionIndex) = 0;
	
	//!判断指定的section索引是否在合法的范围
	virtual BOOL	IsValidSectionIndex(u16 wSectionIndex) = 0;

	//!判断指定的section是否为bss区
	virtual BOOL	IsBssSection(u16 wSectionIndex) = 0;

	//!判断指定的section是否为data区
	virtual BOOL	IsDataSection(u16 wSectionIndex) = 0;

	//!判断指定的section是否为code区
	virtual BOOL	IsCodeSection(u16 wSectionIndex) = 0;

	//!取得elf文件中section的数目
	virtual u16	 	GetSectionCount() = 0;

	//!取得program段的数目
	virtual u16		GetProgramCount() = 0;	
	
	//!取得section名称section的索引
	virtual u16		GetSectionNameTableIndex() = 0;

	//!检查文件头，看是否合法。
	virtual BOOL	IsValid(BYTE * image) = 0;
	
	//!从内存卸载已经转载的elf文件image
	virtual BOOL	UnLoadElfFile() = 0;

	//!转载elf文件到内存
	virtual BOOL	LoadElfFile(LPCTSTR lpszPathName) = 0;
	
	//!取得elf的image的长度
	virtual DWORD	GetImageLength() = 0;

	//!取得指定索引section的名称字符串
	virtual BYTE *	GetSectionName(u16 index) = 0;

	//!取得section名称
	virtual BYTE *	GetSectionNameTable() = 0;	

	//!取得section区的首指针
	virtual BYTE *	GetSectionPtr(u16 index) = 0;

	//!取得image的buffer
	virtual BYTE *	GetImageBuf() = 0;

	//!取得符号的名称
	virtual char *	GetSymbolName(Elf32_Sym *pSymbol, u16 SectionIndex) = 0;

	//!取得elf文件头
	virtual Elf32_Ehdr * GetElfHeader() = 0;

	//!取得指定索引的program头信息
	virtual Elf32_Phdr * GetProgramTable(int index) = 0;

	//!取得section目录表中指定索引的section头
	virtual Elf32_Shdr * GetSectionTable(u16 index) = 0;

	//!返回指定索引section的类型
	virtual Elf32_Word	GetSectionType(u16 wSectionIndex) = 0;
	

protected:
	//elf类初始化
//	virtual void		Initialize() = 0;

	//设置elf文件名或路径
//	virtual void		 SetFile(LPCTSTR lpszPathName) = 0;

protected:
	Elf32_Ehdr *m_ElfHead;					//! ELF文件头 
   	char		m_szElfFilePath[MAX_PATH];	//! elf文件路径 
	BYTE		*m_ElfImage;				//! elf文件内存映像
	DWORD		m_dwImageLength;			//! elf 映像长度（字节）
};


//------------------------------------------------------------------------------------------------------------
// THE END
//------------------------------------------------------------------------------------------------------------
#endif //! !defined(AFX_ELFFILE_H__89772490_6E64_43BE_92FD_7192F00EFA94__INCLUDED_)


