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

	void ShowConditionVariable_Pushback();
	void ShowConditionVariable_Second();
	void ShowConditionVariable_Takeout();
	void ShowConditionVariable(bool bIsThreadExecute = true);




public:
	CMultithreading* pMulthread;

	std::mutex m_mutext;
	std::mutex m_OneMutex;
	std::mutex m_TwoMutex;

	std::condition_variable m_ConVariable;

	list<int>m_listValue;
	vector<int>m_vctValue;

	int m_iCount;
};
