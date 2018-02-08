#include "StdAfx.h"
#include "ReadAndWriteLock.h"


CReadAndWriteLock::CReadAndWriteLock(void):m_nReader(0),m_nWriter(0)
{
	m_Rlock = ::CreateMutexW(NULL,FALSE,_T("Rlock"));
	m_Wlock = ::CreateMutexW(NULL,FALSE,_T("Wlock"));
	m_R = ::CreateEvent(NULL, TRUE, TRUE, NULL);
}


CReadAndWriteLock::~CReadAndWriteLock(void)
{
	::CloseHandle(m_Rlock);
	::CloseHandle(m_Wlock);
	::CloseHandle(m_R);
}


void CReadAndWriteLock::readlock()
{
	::WaitForSingleObject(m_R,INFINITE);
	::WaitForSingleObject(m_Rlock,INFINITE);
	m_nReader++;
	if ( m_nReader == 1 )
		::WaitForSingleObject(m_Wlock,INFINITE);
	::ReleaseMutex(m_Rlock);
}

void CReadAndWriteLock::readunlock()
{
	::WaitForSingleObject(m_Rlock,INFINITE);
	m_nReader--;
	if ( m_nReader == 0 )
		::ReleaseMutex(m_Wlock);
	::ReleaseMutex(m_Rlock);
}

void CReadAndWriteLock::writelock()
{
	::ResetEvent(m_R);
	//::WaitForSingleObject(m_R,INFINITE);
	::WaitForSingleObject(m_Wlock,INFINITE);
	//m_nWriter++;
	//if ( m_nWriter == 1 )
		//::WaitForSingleObject(m_Rlock,INFINITE);
}

void CReadAndWriteLock::writeunlock()
{
	
	//m_nWriter--;
	//if ( m_nWriter == 0 )
	::ReleaseMutex(m_Wlock);
	//::ReleaseMutex(m_R);
	::SetEvent(m_R);
}


RWLock::RWLock()
     : m_cnt(0)
     , m_evt(NULL)
     , m_cs()
     , m_sem(NULL)
 {
    // 提倡的做法在专门的初始化函数里创建和初始化这些变量
 
     ::InitializeCriticalSection(&m_cs);
 
     // Event必须是手动重置，否则存在死锁隐患，即等待Event前，先被激活了
     m_evt = ::CreateEvent(NULL, TRUE, TRUE, NULL);
     m_sem = ::CreateSemaphore(NULL, 1, 1, NULL);
 }
 
 RWLock::~RWLock()
 {
     ::CloseHandle(m_sem);
     ::CloseHandle(m_evt);
     ::DeleteCriticalSection(&m_cs);
 }
 
 void RWLock::AcquireReadLock()
 {
     ::WaitForSingleObject(m_evt, INFINITE);
 
     ::EnterCriticalSection(&m_cs);
     if(0 == m_cnt++)
         ::WaitForSingleObject(m_sem, INFINITE);
     ::LeaveCriticalSection(&m_cs);
 }

 void RWLock::ReleaseReadLock()
 {
     ::EnterCriticalSection(&m_cs);
     if(0 == --m_cnt)
         ::ReleaseSemaphore(m_sem, 1, NULL);
     ::LeaveCriticalSection(&m_cs);
 }
 
 void RWLock::AcquireWriteLock()
 {
     ::ResetEvent(m_evt);
     ::WaitForSingleObject(m_sem, INFINITE);
 }
 
 void RWLock::ReleaseWriteLock()
 {
     ::ReleaseSemaphore(m_sem, 1, NULL);
     ::SetEvent(m_evt);
 }