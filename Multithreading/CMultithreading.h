#pragma once

#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <direct.h>
#include <vector>
#include <map>
#include <windows.h>
#include <list>
#include <thread>
#include <mutex>

using namespace std;

class CMultithreading
{
public:

	CMultithreading();

	CMultithreading(string strTemp);

	CMultithreading(CMultithreading& cmultTemp);

	virtual ~CMultithreading();

	void MyThreadClassTest(CMultithreading& cmultTemp);

	void Execute_MyThreadClassTest(bool bIsThreadExecute = true);

	void MyThreadRefTest(string& strTemp);

	void Execute_MyThreadRefTest(bool bIsThreadExecute = true);

	void MutexClassMemberFunction(bool bIsThreadExecute = true);

	void InsetValue();

	void TakeOutValue();

	void DeadLockInsetValue();

	void DeadLockTakeOutValue();

	void DeadLockMutexThread(bool bIsThreadExecute = true);

	void SolveDeadLockTakeOutValue();

	void SolveDeadLock(bool bIsThreadExecute = true);

	void ShowUnqiue_lock(bool bIsThreadExecute = true);

	void Unique_lockInsert(string strParam);

	void Unique_lockTakeOut(string strParam);

	void SingletonFunction(string strParam);

	void SingletonModel(bool bIsThreadExecute = true);

	void ShowCallOnce(bool bIsThreadExecute = true);




public:
	CMultithreading* pMulthread;
	std::thread m_thTest;
	string m_strValue;

	list<int>m_listValue;
	std::mutex m_mutext;
	std::mutex m_OneMutex;
	std::mutex m_TwoMutex;
};
