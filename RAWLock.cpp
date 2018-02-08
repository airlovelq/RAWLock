// RAWLock.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "ReadAndWriteLock.h"

RWLock mu;
CReadAndWriteLock mutex;
DWORD WINAPI read(LPVOID lpParam)
{
	//mu.AcquireReadLock();
	mutex.readlock();
	printf("read!\n");
	//Sleep(5000);
	mutex.readunlock();
	//mu.ReleaseReadLock();
	return 0;
}
DWORD WINAPI write(LPVOID lpParam)
{
	//mu.AcquireWriteLock();
	mutex.writelock();
	printf("write!\n");
	Sleep(1000);
	mutex.writeunlock();
	//mu.ReleaseWriteLock();
	return 0;
}
HANDLE mutexhandle;
DWORD WINAPI test1(LPVOID lpParam)
{
	::WaitForSingleObject(mutexhandle,INFINITE);
	printf("read!\n");
	Sleep(500);
	::ReleaseMutex(mutexhandle);
	return 0;
}

DWORD WINAPI test2(LPVOID lpParam)
{
	::WaitForSingleObject(mutexhandle,INFINITE);
	printf("write!\n");
	Sleep(200);
	::ReleaseMutex(mutexhandle);
	return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
	/*
	for ( int j=0 ; j<10 ; j++ )
	{
	for ( int i=0 ; i<6 ; i++ )
	{
	    CreateThread(NULL,0,read,NULL,0,NULL);
	}
	for ( int i=0 ; i<3 ; i++ )
	{
	    CreateThread(NULL,0,write,NULL,0,NULL);
	}
	}
	while(1)
	{
	}*/
	mutexhandle = CreateMutexW(NULL,FALSE,_T("W"));
	for ( int i=0 ; i<100 ; i++ )
	{
	    CreateThread(NULL,0,test1,NULL,0,NULL);
	}
	Sleep(10000);
	for ( int i=0 ; i<100 ; i++ )
	{
	    CreateThread(NULL,0,test2,NULL,0,NULL);
	}
	while(1)
	{
	}
	return 0;
}

