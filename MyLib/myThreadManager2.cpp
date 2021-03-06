#include "stdafx.h"
#include "myThreadManager2.h"
#include <process.h>

const int KILL_THREAD_WAIT_TIME = 5 * 1000;

CThreadManager2::CThreadManager2(void)
{
	m_hStopEvent = NULL;
	m_hThreadHandle = NULL;
}


CThreadManager2::~CThreadManager2(void)
{
	KillThread();
}

 
unsigned CThreadManager2::ThreadMainLoopEnter(LPVOID pParam)
{
	CThreadManager2* p = (CThreadManager2*)pParam;
	if(!p)
		return 0;
	return p->m_threadCB();
}

void CThreadManager2::StopThread()
{
	if(m_hStopEvent)
	{
		::SetEvent(m_hStopEvent);
		if(WaitForSingleObject(m_hStopEvent,KILL_THREAD_WAIT_TIME) != WAIT_OBJECT_0)
		{
			KillThread();
		}
	}
}
bool CThreadManager2::StartThread()
{
	m_hStopEvent = ::CreateEvent(NULL,FALSE,FALSE,NULL);
	if(!m_hStopEvent)
	{
		return false;
	}
	m_hThreadHandle = reinterpret_cast<HANDLE>(_beginthreadex(NULL,0,ThreadMainLoopEnter,this,0,NULL));
	if(!m_hThreadHandle)
	{
		return false;
	}
	return true;	
}

bool CThreadManager2::IsExit(int nTime)
{
	bool bRet = false;
	if(::WaitForSingleObject(m_hStopEvent,nTime) == WAIT_OBJECT_0)
	{
		bRet = true;
	}
	return bRet;
}

void CThreadManager2::KillThread()
{
	DWORD dwExitCode;
	::GetExitCodeThread(m_hThreadHandle,&dwExitCode);
	if(STILL_ACTIVE == dwExitCode)
	{
		::TerminateThread(m_hThreadHandle,dwExitCode);
	}
}

void CThreadManager2::SetThreadFuncCallback(THREAD_FUNC_CB cb)
{
	m_threadCB = cb;
}