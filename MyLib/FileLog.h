#pragma once
#include <iostream>

class CFileLog
{
public:
	typedef enum LOG_LEVEL
	{
		error,
		warning,
		info,
		detail
	}LOG_LEVEL;

	static bool Init(char* szLogName,enum LOG_LEVEL eLevel = detail);
	static void Clear();
	static void Print(enum LOG_LEVEL eLevel,const char* szFmt,...);

private:
	static long m_nContentSize;
	static LOG_LEVEL m_eLevel;



};

