#pragma once
#include <Windows.h>
class CThreadManager2
{
public:

	typedef unsigned (__cdecl* THREAD_FUNC_CB)(void);
	CThreadManager2(void);
	~CThreadManager2(void);

	static unsigned _stdcall ThreadMainLoopEnter(LPVOID pParam);
	typedef unsigned (__cdecl* THREAD_FUNC_CB)(void);
	void SetThreadFuncCallback(THREAD_FUNC_CB cb);
	void StopThread();
	bool StartThread();
	bool IsExit(int nTime);
	void KillThread();

private:
	HANDLE m_hStopEvent;
	HANDLE m_hThreadHandle;
	THREAD_FUNC_CB m_threadCB;	 
};

