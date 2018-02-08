#pragma once
//自己的
class CReadAndWriteLock
{
public:
	CReadAndWriteLock(void);
	~CReadAndWriteLock(void);
protected:
	HANDLE m_Rlock;
	HANDLE m_Wlock;
	HANDLE m_R;
public:
	void readlock();
	void readunlock();
	void writelock();
	void writeunlock();
private:
	int m_nReader;
	int m_nWriter;
};
//别人的
class RWLock
 {
 public:
     RWLock();
     ~RWLock();
 public:
    void AcquireReadLock();
    void ReleaseReadLock();
   void AcquireWriteLock();
   void ReleaseWriteLock();
private:
   volatile DWORD      m_cnt;
    CRITICAL_SECTION    m_cs;
    HANDLE              m_evt;
	HANDLE              m_sem;
};
