#include "stdafx.h"
#include "myThreadManager.h"
#include <process.h>

const int KILL_THREAD_WAIT_TIME = 5 * 1000;

CThreadManager::CThreadManager(void)
{
	m_hStopEvent = NULL;
	m_hThreadHandle = NULL;
}


CThreadManager::~CThreadManager(void)
{
	KillThread();
}

 
unsigned CThreadManager::ThreadMainLoopEnter(LPVOID pParam)
{
	CThreadManager* p = (CThreadManager*)pParam;
	if(!p)
		return 0;
	return p->ThreadFunc();
}

void CThreadManager::StopThread()
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
bool CThreadManager::StartThread()
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

bool CThreadManager::IsExit(int nTime)
{
	bool bRet = false;
	if(::WaitForSingleObject(m_hStopEvent,nTime) == WAIT_OBJECT_0)
	{
		bRet = true;
	}
	return bRet;
}

void CThreadManager::KillThread()
{
	DWORD dwExitCode;
	::GetExitCodeThread(m_hThreadHandle,&dwExitCode);
	if(STILL_ACTIVE == dwExitCode)
	{
		::TerminateThread(m_hThreadHandle,dwExitCode);
	}
}