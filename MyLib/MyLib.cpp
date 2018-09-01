// MyLib.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "FileLog.h"
#include "myThreadManager2.h"

CThreadManager2 g_threadMgr;
CThreadManager2 g_threadMgr1;

unsigned ThreadFunc()
{
	int nCount = 0;
	while(true)
	{
		if(g_threadMgr.IsExit(200))
		{
			break;
		}
		printf("ThreadFunc()  %d\n",nCount++);
	}
	return 0;
}

unsigned ThreadFunc1()
{
	int nCount = 0; 
	while(true)
	{
		if(g_threadMgr.IsExit(100))
		{
			break;
		}
		printf("ThreadFunc1() --- %d\n",100*(nCount++));
	}
	return 0;
}


int _tmain(int argc, _TCHAR* argv[])
{
	CFileLog::Init("c:\\log\\test.log",CFileLog::detail);
	CFileLog::Print(CFileLog::info,"lalalalala");
	std::string sLog = "hello";
	int i = 1;
	float f = 1.22222;

	CFileLog::Print(CFileLog::info,"%s %d %.2f",sLog.c_str(),i,f);
	g_threadMgr.SetThreadFuncCallback(ThreadFunc);
	g_threadMgr.StartThread();

	g_threadMgr1.SetThreadFuncCallback(ThreadFunc1);
	g_threadMgr1.StartThread();


	getchar();
	return 0;
}

