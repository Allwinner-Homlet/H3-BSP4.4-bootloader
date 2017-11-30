#include "stdafx.h"
#include "log.h"

CLog::CLog()  //构造函数，设置日志文件的默认路径
{
	::InitializeCriticalSection(&m_crit);   //初始化临界区
}


CLog::~CLog()
{
	::DeleteCriticalSection(&m_crit);    //释放里临界区
}

 
/*================================================================ 
* 函数名：    InitLog
* 参数：      LPCTST lpszLogPath
* 功能描述:   初始化日志(设置日志文件的路径)
* 返回值：    void
================================================================*/ 
void CLog::InitLog(LPCTSTR lpszLogPath)   
{	
	m_strLogPath=lpszLogPath;
}

void CLog::Add(const char* fmt, ...)
{
	if (m_strLogPath.IsEmpty())
		return ;
	
	if (!AfxIsValidString(fmt, -1))
		return ;
/*-----------------------进入临界区(写文件)------------------------------------*/	
	::EnterCriticalSection(&m_crit);   
	try      
	{
		va_list argptr;          //分析字符串的格式
		va_start(argptr, fmt);
		_vsnprintf(m_tBuf, BUFSIZE, fmt, argptr);
		va_end(argptr);
	}
	catch (...)
	{
		m_tBuf[0] = 0;
	}
	
	FILE *fp = fopen(m_strLogPath, "a"); //以添加的方式输出到文件
	if (fp)
	{
		fprintf(fp,"%s:  ", AfxGetApp()->m_pszExeName);  //加入当前程序名
		
		CTime ct ;                          //加入当前时间
		ct = CTime::GetCurrentTime();
		fprintf(fp,"%s : ",ct.Format("%m/%d/%Y %H:%M:%S"));
		fprintf(fp, "%s\n", m_tBuf);		
		fclose(fp);		
	}	
	::LeaveCriticalSection(&m_crit);  
/*-------------------退出临界区----------------------------------------*/	
}


