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
// File    : file.cpp                                                                                       //
// By      : scottyu                                                                                        //
// Version : V1.00                                                                                          //
// Time    : 2007-12-13 15:50:23                                                                            //
//                                                                                                          //
//----------------------------------------------------------------------------------------------------------//
//                                                                                                          //
// HISTORY                                                                                                  //
//                                                                                                          //
// 1 2007-12-13 15:50:23                                                                                    //
//                                                                                                          //
//                                                                                                          //
//                                                                                                          //
//----------------------------------------------------------------------------------------------------------//

#ifndef __COMMON_FILE__C_____
#define __COMMON_FILE__C_____	1


//------------------------------------------------------------------------------------------------------------
//����ͷ�ļ�
//------------------------------------------------------------------------------------------------------------
#include "file.h"
#include <stdio.h>

#ifdef _WINDOWS
#include <direct.h>
#include <windows.h>

static u32 _Bin2Cdata(s8 * filename, u32 startaddress, u32 length);

//------------------------------------------------------------------------------------------------------------
//
// ����˵��
//     ����Ŀ¼
//
// ����˵��
//     
//
// ����ֵ
//     
//
// ����
//    ��
//
//------------------------------------------------------------------------------------------------------------
u32 CreateDir(s8 * dir, void * para)
{
	int result = CreateDirectory((const char *)dir, (LPSECURITY_ATTRIBUTES)para);
	if (result)
		return OK;
	else
	{
		DWORD err = ::GetLastError();
		return err;
		return __LINE__;
	}
}

//------------------------------------------------------------------------------------------------------------
//����һ���ļ���
//------------------------------------------------------------------------------------------------------------
u32 makedir(const char * dir)
{
	int ret = 0;
	ret = _mkdir(dir);
	if (0 == ret)
		return OK;

	Err("makekdir", __FILE__, __LINE__, "return = %d", ret);
	return __LINE__;
}
//------------------------------------------------------------------------------------------------------------
//ֻ��ɾ����Ŀ¼�����Ŀ¼�����ļ�����Ҫ�Ƚ��ļ�����Ŀ¼ɾ����
//------------------------------------------------------------------------------------------------------------
u32 removedir( const char * dir)
{
	if (!dir)
		return __LINE__;

	if (0 == strlen(dir))
		return __LINE__;

	if (RemoveDirectory(dir))
		return OK;
	else
		return __LINE__;
}
//------------------------------------------------------------------------------------------------------------
//
// ����˵��
//     ɾ����Ŀ¼
//
// ����˵��
//     
//
// ����ֵ
//     
//
// ����
//    ��
//
//------------------------------------------------------------------------------------------------------------
u32 DeleteDir(s8 * dir)
{
	int result = RemoveDirectory((const char *)dir);

	if (result)
		return OK;
	else
		return __LINE__;
}

//------------------------------------------------------------------------------------------------------------
//
// ����˵��
//     
//
// ����˵��
//     
//
// ����ֵ
//     
//
// ����
//    ��
//
//------------------------------------------------------------------------------------------------------------
u32 DeleteFullDirectory(s8 * Path)
{
	u32 ret 	 = OK;	
	HANDLE hFind = NULL;
	WIN32_FIND_DATA finddata;
	s8 str[128];
	s8 finddir[MAX_PATH];		
	s8 findmode[MAX_PATH];		
	s8 fullpath[MAX_PATH];		
	
	//���Ŀ¼����
	memset(finddir, 0, MAX_PATH);
	strcpy((char *)finddir, (char *)Path);

	memset(findmode, 0, MAX_PATH);
	sprintf((char *)findmode, "%s\\*.*", Path);//
	
	memset(&finddata, 0, sizeof(WIN32_FIND_DATA));
	lstrcpy(finddata.cFileName, (char *)findmode);

	hFind = FindFirstFile((const char *)findmode, &finddata);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		DWORD dwTemp = GetLastError();
		FindClose(hFind);		
		return __LINE__;
	}
	//Start to find file					
	while(1)
	{
		if (!FindNextFile(hFind, &finddata))
		{
			if (GetLastError() != ERROR_NO_MORE_FILES)
			{
				sprintf(( char *)str, "_DeletePath FindNextFile failed %d", GetLastError());
				::OutputDebugString((const char *)str);
			}
			break;
		}				
		memset(fullpath, 0, MAX_PATH);
		sprintf(( char *)fullpath, "%s\\%s", finddir,  finddata.cFileName);
		
		if (OK == IsDotDot(&finddata))
		{
			//
		}		
		else if (OK == IsDot(&finddata))
		{					
			//
		}			
		else if (OK == IsDirectory(&finddata))
		{							
			DeleteFullDirectory(fullpath);			
		}
		else  //file
		{				
			DeleteSingleFile((const char *)fullpath);						
		}		
	}
	FindClose(hFind);
	hFind = NULL;
	
	DeleteDir(Path);

	return OK;	

}

//------------------------------------------------------------------------------------------------------------
//
// ����˵��
//     ɾ���ļ�
//
// ����˵��
//     filename ɾ�����ļ�����
//
// ����ֵ
//     OK��ʾ�ɹ�������ʧ��
//
// ����
//    ��
//
//------------------------------------------------------------------------------------------------------------
u32 RemoveFile(const char  * filename)
{
	if (0 == remove((const char *)filename))
	{	
		return OK;
	}
	
	return __LINE__;

}

//------------------------------------------------------------------------------------------------------------
//
// ����˵��
//     
//
// ����˵��
//     
//
// ����ֵ
//     
//
// ����
//    ��
//
//------------------------------------------------------------------------------------------------------------
u32 DeleteSingleFile( const char * FullPath)
{
	return RemoveFile(FullPath);
}

//------------------------------------------------------------------------------------------------------------
//
// ����˵��
//     ȡ���ļ��ĳ���
//
// ����˵��
//     filename�ļ�����
//
// ����ֵ
//     �ļ����ȣ�0��ʾʧ��
//
// ����
//    ��
//
//------------------------------------------------------------------------------------------------------------
u32 GetFileLength(s8 * filename)
{

	FILE * fp = NULL;
	fp = fopen((const char *)filename, "rb");
	if (fp == NULL)
	{
		return 0;
	}
	long position = ftell(fp);
	fseek(fp, 0, SEEK_END);
	u32 len = (u32)ftell(fp) - position;
	fclose(fp);
	return len;
}

//------------------------------------------------------------------------------------------------------------
//
// ����˵��
//     ȡ���ļ�ָ����������
//
// ����˵��
//     filename�ļ�����
//
// ����ֵ
//     OK��ʾ�ɹ�
//
// ����
//    ��
//
//------------------------------------------------------------------------------------------------------------
u32 GetFileData(s8 * file, u8 * buffer, u32 Size)
{
	u32 FileLength = 0;
	FileLength = GetFileLength(file);
	if (FileLength < Size)
	{
		return __LINE__;
	}
	
	FILE * fp = NULL;
	fp = fopen((const char *)file, "rb");
	if (fp == NULL)
	{
		return __LINE__;
	}
	
	if (1 != fread(buffer, Size, 1, fp))
	{
		fclose(fp);
		return __LINE__;
	}
				
	fclose(fp);
	return OK;
}

//------------------------------------------------------------------------------------------------------------
//
// ����˵��
//     ��ȡ��ǰӦ�ó����·��
//
// ����˵��
//     ��
//
// ����ֵ
//     ��ǰӦ�ó����·��
//
// ����
//    ��
//
//------------------------------------------------------------------------------------------------------------
/*
s8 *GetModuleDirectory()
{	
	static s8  szModuleDirectory[MAX_PATH];	
	
	memset(szModuleDirectory, 0, MAX_PATH);
    ::GetModuleFileName(NULL, ( char *)szModuleDirectory, MAX_PATH);
    	
	(strrchr((const char *)szModuleDirectory, '\\'))[1] = 0;
	
	return szModuleDirectory; 
}
*/

u32 GetModuleDirectory(char * dir)
{	
	if (!dir)
		return __LINE__;
		
	char szModuleDirectory[MAX_PATH];		
	memset(szModuleDirectory, 0, MAX_PATH);
    ::GetModuleFileName(NULL, ( char *)szModuleDirectory, MAX_PATH);    	
	(strrchr((const char *)szModuleDirectory, '\\'))[0] = 0;
	
	//(strrchr((const char *)szModuleDirectory, '\\'))[1] = 0;

	strcpy(dir, szModuleDirectory);
	
	return OK; 
}

//------------------------------------------------------------------------------------------------------------
//
// ����˵��
//     �����ļ��Ƿ����
//
// ����˵��
//     
//
// ����ֵ
//     
//
// ����
//    ��
//
//------------------------------------------------------------------------------------------------------------
u32 IsFileExist( const char * szFilePath)
{
	if (szFilePath == NULL)
		return __LINE__;

#ifdef _WINDOWS_
	//------------------------------------------------
	//�����ļ��Ƿ����
	//------------------------------------------------
	FILE * fp = NULL;
	fp = fopen((const char *)szFilePath, "r");
	if (fp == NULL)
		return __LINE__;
		
	//------------------------------------------------
	//
	//------------------------------------------------
	fclose(fp);
#else

	HANDLE hFile = open(szFilePath, O_RDWR);
	if(hFile == NULL)
	{
		return __LINE__;
	}
	close((int)hFile);
#endif
	return OK;
}



//------------------------------------------------------------------------------------------------------------
//
// ����˵��
//     ����Ŀ¼�Ƿ����
//
// ����˵��
//     szDirĿ¼����
//
// ����ֵ
//     OK��ʾ����
//
// ����
//    ��
//
//------------------------------------------------------------------------------------------------------------
u32 IsDirExist( s8 *szDir)
{
    if (szDir == NULL)
		return __LINE__;
    
    //------------------------------------------------
    //
    //------------------------------------------------
    u32 attr = (u32)GetFileAttributes((const char *)szDir);
	if (0xFFFFFFFF == attr)
		return __LINE__;

    if ((attr & FILE_ATTRIBUTE_DIRECTORY) != 0)
        return OK;
    else
        return __LINE__;    
}
//ע�⣺IsDirExist�ǲ��ɿ��ģ���������



//------------------------------------------------------------------------------------------------------------
//
// ����˵��
//     ���Ե�ǰ���ҵ����Ƿ�ΪĿ¼
//
// ����˵��
//     pfinddata��ǰ���ҵ���Ŀ¼�ļ���Ϣ
//
// ����ֵ
//     OK��ʾ��Ŀ¼
//
// ����
//    ��
//
//------------------------------------------------------------------------------------------------------------
u32 IsDirectory(WIN32_FIND_DATA * pfinddata)
{
	if (!pfinddata )
		return __LINE__;
		
	//------------------------------------------------
    //
    //------------------------------------------------	
	if ((pfinddata->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0)
		return OK;
	
	return __LINE__;
}


//------------------------------------------------------------------------------------------------------------
//
// ����˵��
//     ���Ե�ǰ���ҵ����Ƿ�Ϊ. ..
//
// ����˵��
//     pfinddata��ǰ���ҵ���Ŀ¼�ļ���Ϣ
//
// ����ֵ
//     OK��ʾ��. ..
//
// ����
//    ��
//
//------------------------------------------------------------------------------------------------------------
u32 IsDot(WIN32_FIND_DATA * pfinddata)
{
	if (!pfinddata )
		return __LINE__;
	
	//------------------------------------------------
    //
    //------------------------------------------------	
	if (OK != IsDirectory(pfinddata))
		return __LINE__;

	//------------------------------------------------
    //
    //------------------------------------------------	
	if (pfinddata->cFileName[0] == '.')
	{
		if (pfinddata->cFileName[1] == '\0' ||
		   (pfinddata->cFileName[1] == '.' && pfinddata->cFileName[2] == '\0'))
		{
			return OK;
		}
	}
	
	return __LINE__;
}

//------------------------------------------------------------------------------------------------------------
//
// ����˵��
//     ���Ե�ǰ���ҵ����Ƿ�Ϊ. ..
//
// ����˵��
//     pfinddata��ǰ���ҵ���Ŀ¼�ļ���Ϣ
//
// ����ֵ
//     OK��ʾ��. ..
//
// ����
//    ��
//
//------------------------------------------------------------------------------------------------------------
u32 IsDotDot(WIN32_FIND_DATA * pfinddata)
{
	if (!pfinddata )
		return __LINE__;
	
	//------------------------------------------------
    //
    //------------------------------------------------	
	if (OK != IsDirectory(pfinddata))
		return __LINE__;
		
	//------------------------------------------------
    //
    //------------------------------------------------	
	if (pfinddata->cFileName[0] == '.' &&
		pfinddata->cFileName[1] == '.' && 
		pfinddata->cFileName[2] == '\0')
	{
		
		return OK;
		
	}
	
	return __LINE__;
}


//------------------------------------------------------------------------------------------------------------
//
// ����˵��
//     ʹ��notepad���ļ�
//
// ����˵��
//     file�ļ�����
//
// ����ֵ
//     ��
//
// ����
//    ��
//
//------------------------------------------------------------------------------------------------------------
void ShowInNotepad( s8 * file)
{
	s8 command [256];
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	
	
	memset(command, 0, 256);
	sprintf(( char *)command, "notepad %s" , file);
	
	ZeroMemory(&si, sizeof(si));
	ZeroMemory(&pi, sizeof(pi));
	si.cb = sizeof(si);
	
	//------------------------------------------------
    //
    //------------------------------------------------
	CreateProcess(NULL, ( char *)command, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
}




//------------------------------------------------------------------------------------------------------------
//
// ����˵��
//     binȫ��ת��
//
// ����˵��
//     
//
// ����ֵ
//     
//
// ����
//    ��
//
//------------------------------------------------------------------------------------------------------------
u32 Bin2Cdata(s8 * filename, u32 startaddress)
{
	u32 filelength = GetFileLength(filename);
	return _Bin2Cdata( filename,  startaddress, filelength);
}


//------------------------------------------------------------------------------------------------------------
//
// ����˵��
//     ������ת��
//
// ����˵��
//     
//
// ����ֵ
//     
//
// ����
//    ��
//
//------------------------------------------------------------------------------------------------------------
u32 Bin2CdataWithLen(s8 * filename, u32 startaddress, u32 length)
{
	return _Bin2Cdata( filename,  startaddress, length);  
}


//------------------------------------------------------------------------------------------------------------
//
// ����˵��
//     ��ʼ��ַת��
//
// ����˵��
//     
//
// ����ֵ
//     
//
// ����
//    ��
//
//------------------------------------------------------------------------------------------------------------
u32 Bin2CdataWithAddr(s8 * filename, u32 startaddress, u32 endaddress)
{
	if (endaddress < startaddress)
	{
		Err("Bin2CdataWithAddr", __FILE__, __LINE__, "endaddress < startaddress");
		return __LINE__;
	}
	return _Bin2Cdata( filename,  startaddress, endaddress - startaddress);
}


//------------------------------------------------------------------------------------------------------------
//
// ����˵��
//     
//
// ����˵��
//     
//
// ����ֵ
//     
//
// ����
//    ��
//
//------------------------------------------------------------------------------------------------------------

static u32 _Bin2Cdata(s8 * filename, u32 startaddress, u32 length)
{
#define ___BUFFER_LEN__	16                  //ÿ��16�ֽ�����
#define STR_LEN		(___BUFFER_LEN__ * 6)    //


	FILE * fb = NULL;
	FILE * fc = NULL;
	s8 cdatafilename[MAX_PATH] = "";

	u8 buffer[___BUFFER_LEN__];
	s8 string[STR_LEN];
	s8 tmp[8];
	u32 i = 0;
	u32 binlength = 0;
	
	u32 filelength = GetFileLength(filename);
	if (0 == filelength)
	{
		Err("_Bin2Cdata", __FILE__, __LINE__, "file length is zero");
		return __LINE__;
	}
	sprintf(( char *)cdatafilename, "%s.c", filename);

	if (filelength <= startaddress)
	{
		Err("_Bin2Cdata",  __FILE__, __LINE__, "File filelength < startaddress");
		return __LINE__;
	}

	fb = fopen((const char *)filename, "rb");
	if (NULL == fb)
	{
		Msg("Open %s Failed !\n", filename);
		return __LINE__;
	}

	fc = fopen((const char *)cdatafilename, "wb");
	if (NULL == fc)
	{
		fclose(fb);
		Msg("Create %s Failed !\n", cdatafilename);
		return __LINE__;
	}

	if (0 != fseek(fb, startaddress, SEEK_SET))
	{
		fclose(fc);
		fclose(fb);
		Msg("Seek %s %d Failed !\n", filename, startaddress);
		return __LINE__;
	}
	if (filelength < startaddress + length)
		length = filelength - startaddress;
	if (length == 0)
	{
//		Warn("_Bin2Cdata", __FILE__, __LINE__, "length == 0");
		return OK;
	}

//��ʼת������
	i = 0;
	binlength = length;

	memset(string, 0, STR_LEN);
	sprintf(( char *)string, "//File=%s\n", filename);
	fwrite((const char *)string, strlen((const char *)string), 1, fc);

	memset(string, 0, STR_LEN);
	sprintf(( char *)string, "//addr=0x%X length=0x%X(%d)\n", startaddress, binlength, binlength);
	fwrite((const char *)string, strlen((const char *)string), 1, fc);
	
	memset(string, 0, STR_LEN);
	sprintf(( char *)string, "static unsigned char _LION_RES_DATA[%d] = {\n", binlength);
	fwrite((const char *)string, strlen((const char *)string), 1, fc);

	while (binlength >= ___BUFFER_LEN__)
	{		
		memset(tmp, 0, 8);
		memset(buffer, 0, ___BUFFER_LEN__);
		memset(string, 0, STR_LEN);
		fread(buffer, ___BUFFER_LEN__, 1, fb);
		binlength -= ___BUFFER_LEN__;
		for (i = 0; i < ___BUFFER_LEN__; i++)
		{
			sprintf(( char *)tmp, "0x%02X,", buffer[i]);
			strcat(( char *)string, (const char *)tmp);
		}
		strcat(( char *)string, "\n");
		fwrite((const char *)string, strlen((const char *)string), 1, fc);
	}
	if (binlength > 0)
	{
		memset(tmp, 0, 8);
		memset(buffer, 0, ___BUFFER_LEN__);
		memset(string, 0, STR_LEN);
		fread(buffer, binlength, 1, fb);
		for (i = 0; i < binlength; i++)
		{
			if (i < binlength - 1) 
				sprintf(( char *)tmp, "0x%02X,", buffer[i]);
			else
				sprintf(( char *)tmp, "0x%02X", buffer[i]);
			strcat(( char *)string, (const char *)tmp);
		}
		strcat((char *)string, "\n");
		fwrite(string, strlen((const char *)string), 1, fc);
		
	}
	memset(tmp, 0, 8);
	sprintf((char *)tmp,"};\n");
	memset(string, 0, STR_LEN);
	strcat((char *)string, (const char *)tmp);
	fwrite(string, strlen((const char *)string), 1, fc);

	fclose(fc);
	fclose(fb);

	return OK;
}

//------------------------------------------------------------------------------------------------------------
//
// ����˵��
//     ��ȫ·��������ļ���չ��
//
// ����˵��
//     
//
// ����ֵ
//     
//
// ����
//    ��
//
//------------------------------------------------------------------------------------------------------------
u32 GetFileExt(s8 * szFileFullPath, s8 * ext)
{
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];

	if (!szFileFullPath || !ext)
		return __LINE__;

	if( OK != IsFileExist((const char *)szFileFullPath))
	{
		return __LINE__;
	}
	
	_splitpath((const char *)szFileFullPath, drive, dir, fname, (char *)ext);	

	return OK;
}

//------------------------------------------------------------------------------------------------------------
//
// ����˵��
//     ��ȫ·��������ļ���(��������չ��)
//
// ����˵��
//     
//
// ����ֵ
//     
//
// ����
//    ��
//
//------------------------------------------------------------------------------------------------------------
u32 GetFileName(s8 * szFileFullPath, s8 * filename)
{
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
//	char fname[_MAX_FNAME];
	s8   ext[_MAX_EXT];

	if (!szFileFullPath || !filename)
		return __LINE__;

	if( OK != IsFileExist((const char *)szFileFullPath))
	{
		return __LINE__;
	}
	
	_splitpath((const char *)szFileFullPath, drive, dir, (char *)filename, (char *)ext);	

	return OK;
}


//------------------------------------------------------------------------------------------------------------
//
// ����˵��
//     
//
// ����˵��
//     
//
// ����ֵ
//     
//
// ����
//    ��
//
//------------------------------------------------------------------------------------------------------------
u32 SaveDataToFile(u8 * buffer, s8 * filename, u32 startaddr, u32 length, s8 * ext)
{
	FILE * fp = NULL;
	s8 file[MAX_PATH];
	sprintf((char *)file, "%s_%x.%s", filename, startaddr, ext);
	fp = fopen((const char *)file, "wb");
	if (NULL == fp)
	{	
		Err("save", __FILE__, __LINE__, "open file %s failed", file);
		return 1;
	}
	fwrite(buffer, length, 1, fp);
	fclose(fp);
	return OK;
}

#ifdef SCRIPT_LUA
/*
//------------------------------------------------------------------------------------------------------------
//
// ����˵��
//     
//
// ����˵��
//     
//
// ����ֵ
//     
//
// ����
//    ��
//
//------------------------------------------------------------------------------------------------------------

static int proxy_Bin2Cdata(lua_State *L)
{
	//����
    s8 * _filename	= (s8 *)lua_tostring(L, 1);
    u32 _startaddress	= (u32)lua_tonumber(L, 2);
	u32 ret = Bin2Cdata(_filename, _startaddress);
	lua_pushnumber(L, ret);		
	return 1;//1��ʾ��1������ֵ
}


//------------------------------------------------------------------------------------------------------------
//
// ����˵��
//     
//
// ����˵��
//     
//
// ����ֵ
//     
//
// ����
//    ��
//
//------------------------------------------------------------------------------------------------------------
static int proxy_Bin2CdataWithLen(lua_State *L)
{
    //����
    s8 * _filename	= (s8 *)lua_tostring(L, 1);
    u32 _startaddress	= (u32)lua_tonumber(L, 2);
	u32 _length		= (u32)lua_tonumber(L, 3);
	u32 ret = Bin2CdataWithLen(_filename, _startaddress, _length);
	lua_pushnumber(L, ret);		
	return 1;//1��ʾ��1������ֵ
}


//------------------------------------------------------------------------------------------------------------
//
// ����˵��
//     
//
// ����˵��
//     
//
// ����ֵ
//     
//
// ����
//    ��
//
//------------------------------------------------------------------------------------------------------------
static int proxy_Bin2CdataWithAddr(lua_State *L)
{
    //����
    s8 * _filename	= (s8 *)lua_tostring(L, 1);
    u32 _startaddress	= (u32)lua_tonumber(L, 2);
	u32 _endaddress		= (u32)lua_tonumber(L, 3);
	u32 ret = Bin2CdataWithAddr(_filename, _startaddress, _endaddress);
	lua_pushnumber(L, ret);		
	return 1;//1��ʾ��1������ֵ
}
*/
#endif //SCRIPT_LUA


//------------------------------------------------------------------------------------------------------------
// THE END !
//------------------------------------------------------------------------------------------------------------
#else   //_WINDOWS

u32 IsFileExist( const char * szFilePath)
{
	if (szFilePath == NULL)
		return __LINE__;

	HANDLE hFile = open(szFilePath, O_RDWR);
	if(hFile == NULL)
	{
		return __LINE__;
	}
	close((int)hFile);
	
	return OK;
}

u32 RemoveFile(const char  * filename)
{
	if (0 == remove((const char *)filename))
	{	
		return OK;
	}
	
	return __LINE__;

}

#endif //_WINDOWS
#endif //__COMMON_FILE__C_____
