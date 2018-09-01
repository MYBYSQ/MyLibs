#ifndef __MYCRITSEC_H__
#define __MYCRITSEC_H__

#pragma once

#include <WinBase.h>

class CBaseCritsec
{
public:
	CBaseCritsec()
	{
		::InitializeCriticalSection(&m_CritSec);
	}
	virtual ~CBaseCritsec()
	{
		::DeleteCriticalSection(&m_CritSec);
	}

	void Lock()
	{
		::EnterCriticalSection(&m_CritSec);
	}

	void Unlock()
	{
		::LeaveCriticalSection(&m_CritSec);
	}

private:
	CRITICAL_SECTION m_CritSec;

};

class CAutoLockCritSec
{
public:
	CAutoLockCritSec(CBaseCritsec* pBaseCritSec)
	{
		m_Lock = pBaseCritSec;
		if(m_Lock)
		{
			m_Lock->Lock();
		}
	}

	virtual ~CAutoLockCritSec()
	{
		if(m_Lock)
		{
			m_Lock->Unlock();
		}
	}

private:
	CBaseCritsec*  m_Lock;
	
};


#endif