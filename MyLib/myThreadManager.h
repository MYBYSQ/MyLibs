#pragma once
#include <Windows.h>
class CThreadManager
{
public:
	CThreadManager(void);
	virtual ~CThreadManager(void);
	static unsigned _stdcall ThreadMainLoopEnter(LPVOID pParam);
	virtual unsigned ThreadFunc()= 0;
	void StopThread();
	bool StartThread();
	bool IsExit(int nTime);
	void KillThread();

private:
	HANDLE m_hStopEvent;
	HANDLE m_hThreadHandle;
};

