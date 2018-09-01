#include "stdafx.h"
#include "FileLog.h"
#include <Windows.h>
#include "myCritSec.h"

using namespace std;

const LONGLONG g_nMaxSize = 300 * 1024 *1024;


long CFileLog::m_nContentSize = 0;
CFileLog::LOG_LEVEL CFileLog::m_eLevel;

HANDLE g_hLog = INVALID_HANDLE_VALUE;
CBaseCritsec g_fileLogCrit;

std::string GetTime()
{
	const int nBufferSize = 128;
	char szBuf[nBufferSize] = {0};
	SYSTEMTIME st;
	GetLocalTime(&st);

	sprintf_s(szBuf,nBufferSize,"%04d-%02d-%02d %02d:%02d:%02d %03d\t",st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond,st.wMilliseconds);
	return std::string(szBuf);
}

std::string GetLogLeverString(CFileLog::LOG_LEVEL eLevel)
{
	std::string sLevel;
	if(eLevel == CFileLog::error)
	{
		sLevel = "error";
	}
	else if(eLevel == CFileLog::warning)
	{
		sLevel = "warning";
	}
	else if(eLevel == CFileLog::info)
	{
		sLevel = "info";
	}
	else if(eLevel == CFileLog::detail)
	{
		sLevel = "detail";
	}

	sLevel += "\t";
	return sLevel;
}

bool CFileLog::Init(char* szLogName,enum LOG_LEVEL eLevel)
{
	std::string sLogFile(szLogName);
	m_eLevel = eLevel;

	g_hLog = ::CreateFile(sLogFile.c_str(),
		GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
		NULL,
		OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if(g_hLog == INVALID_HANDLE_VALUE)
		return false;

	std::string sHeader("\r\n****************   log begin    *********************\r\n");
	DWORD dwWritten = 0;
	::WriteFile(g_hLog,sHeader.c_str(),(DWORD)sHeader.size(),&dwWritten,NULL);
	m_nContentSize += dwWritten;
	if(m_nContentSize >= g_nMaxSize)
	{
		::SetFilePointer(g_hLog,0,0,FILE_BEGIN);
		m_nContentSize = 0;
	}
	else
	{
		DWORD dwSize = ::SetFilePointer(g_hLog,0,0,FILE_END);
		m_nContentSize += dwSize;
	}

	return true;
}

void CFileLog::Clear()
{
	if(g_hLog != INVALID_HANDLE_VALUE)
		::CloseHandle(g_hLog);
	g_hLog = INVALID_HANDLE_VALUE;
}

void CFileLog::Print(enum LOG_LEVEL eLevel,const char* szFmt,...)
{
	CAutoLockCritSec lock(&g_fileLogCrit);
	try
	{
		if(eLevel > m_eLevel)
		{
			return;
		}

		std::string sContent;
		const int nMaxContentSize = 1024;
		char szBuf[nMaxContentSize] = {0};

		va_list args;
		va_start(args,szFmt);
		vsnprintf_s(szBuf,nMaxContentSize - 1,szFmt,args);
		va_end(args);

		sContent += GetTime();
		sContent += GetLogLeverString(eLevel);
		sContent += szBuf;
		sContent += "\r\n";
		if(g_hLog != INVALID_HANDLE_VALUE)
		{
			DWORD dwWrittem  = 0;
			::WriteFile(g_hLog,sContent.c_str(),sContent.size(),&dwWrittem,NULL);
			m_nContentSize += dwWrittem;
			if(m_nContentSize >= g_nMaxSize)
			{
				::SetFilePointer(g_hLog,0,0,FILE_BEGIN);
				m_nContentSize = 0;
			}
		}
	}
	catch(...)
	{

	}
}