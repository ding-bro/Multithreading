#include "CMultithreading.h"
#include "CThreadBasicInfor.h"
#include "CSingletonMode.h"

CMultithreading::CMultithreading(string strTemp)
{
	cout << "构造形参执行" << endl;
}

CMultithreading::CMultithreading()
{
	cout << "构造执行" << endl;
}

CMultithreading::CMultithreading(CMultithreading& cmultTemp)
{
	cout << "拷贝构造执行" << endl;
}

CMultithreading::~CMultithreading()
{
	cout << "析构执行" << endl;
}

void CMultithreading::MyThreadClassTest(CMultithreading& cmultTemp)
{

}

CThreadBasicInfor m_CThreadBasicInfor;

void TransferCThreadBasicInfor()
{
	m_CThreadBasicInfor.GetThreadBasicInfor();
}

/*
	如果需要改变线程中传递的值，那么不能单单用传址符来解决
	需要用到 std::ref() 来去满足

	并且在创建线程时，如果有使用类的成员函数，第二个参数需要传入类的对象
*/
void CMultithreading::Execute_MyThreadClassTest(bool bIsThreadExecute /* = true */)
{
	if (bIsThreadExecute)
	{
		CMultithreading cmultTemp;
		std::thread thTest(&CMultithreading::MyThreadClassTest, std::ref(cmultTemp), std::ref(cmultTemp));
		thTest.join();
	}
}

void CMultithreading::MyThreadRefTest(string& strTemp)
{
	cout << strTemp << endl;
}

void CMultithreading::Execute_MyThreadRefTest(bool bIsThreadExecute /* = true */)
{
	if (bIsThreadExecute)
	{
		string strTemp;
		strTemp = "ding";

		CMultithreading cmultTemp;

		std::thread thTest(&CMultithreading::MyThreadRefTest, std::ref(cmultTemp), std::ref(strTemp));
		thTest.join();
	}
}

/*
	当多线程执行数据不稳定时，就是共享数据的问题，这时就应该引入互斥锁

	互斥锁的 lock() 和 unlock() 要同时使用，并且两个缺一不可
	为了防止忘记调用其中的一个，引入了 lock_guard<mutex>模板

	简而言之就是当定义了 lock_guard<mutex>模板时，执行构造函数进行 lock()，退出时执行析构函数 unlock()
	利用构造和析构函数
*/
void CMultithreading::InsetValue()
{
	for (int i = 0; i < 100000; i++)
	{
		cout << "插入数据：" << i << endl;

		//m_mutext.lock();

		lock_guard<mutex>lockMutex(m_mutext);

		m_listValue.push_back(i);

		//m_mutext.unlock();
	}
}

void CMultithreading::TakeOutValue()
{
	for (int i = 0; i < 100000; i++)
	{
		//m_mutext.lock();

		lock_guard<mutex>lockMutex(m_mutext);

		if (!m_listValue.empty())
		{
			int iTemp = 0;
			iTemp = m_listValue.front();
			m_listValue.pop_front();

			cout << "取出数据为：" << iTemp << endl;
		}
		else
		{
			cout << "容器为空" << endl;
		}

		//m_mutext.unlock();
	}
}

void CMultithreading::DeadLockInsetValue()
{
	for (int i = 0; i < 100000; i++)
	{
		m_OneMutex.lock();
		m_TwoMutex.lock();

		cout << "插入数据：" << i << endl;

		m_listValue.push_back(i);

		m_OneMutex.unlock();
		m_TwoMutex.unlock();
	}
}

void CMultithreading::DeadLockTakeOutValue()
{
	for (int i = 0; i < 100000; i++)
	{
		m_TwoMutex.lock();
		m_OneMutex.lock();

		if (!m_listValue.empty())
		{
			int iTemp = 0;
			iTemp = m_listValue.front();
			m_listValue.pop_front();

			cout << "取出数据为：" << iTemp << endl;
		}
		else
		{
			cout << "容器为空" << endl;
		}

		m_TwoMutex.unlock();
		m_OneMutex.unlock();
	}
}

void CMultithreading::SolveDeadLockTakeOutValue()
{
	for (int i = 0; i < 100000; i++)
	{
		// 两个互斥量上锁顺序一致即可以解决
		m_OneMutex.lock();
		m_TwoMutex.lock();

		if (!m_listValue.empty())
		{
			int iTemp = 0;
			iTemp = m_listValue.front();
			m_listValue.pop_front();

			cout << "取出数据为：" << iTemp << endl;
		}
		else
		{
			cout << "容器为空" << endl;
		}

		m_TwoMutex.unlock();
		m_OneMutex.unlock();
	}
}

/*
	传入类的成员函数时，第二个参数需要传入类的对象，不管这个成员函数有参还是无参
*/
void CMultithreading::MutexClassMemberFunction(bool bIsThreadExecute /* = true */)
{
	if (bIsThreadExecute)
	{
		CMultithreading cmultTemp;

		string strTemp;

		std::thread thInsert(&CMultithreading::InsetValue, std::ref(cmultTemp));
		std::thread thTakeout(&CMultithreading::TakeOutValue, std::ref(cmultTemp));

		thInsert.join();
		thTakeout.join();
	}
}

/*
	死锁问题：
	比如我有两把锁，死锁问题必须由两个锁头也就是两个互斥量才能产生；金锁和银锁 - 两个线程A、B

	1. 线程A执行时，这个线程先锁金锁，把金锁 lock()成功，然后去 lock()银锁
	这个时候出现了上下文切换

	2. 线程B开始执行，线程B先锁银锁，因为银锁还没有被锁，所以线程B lock()银锁成功，此时线程B又要去锁金锁
	此时此刻，死锁问题就产生了

	3. 线程A因为拿不到银锁头，流程卡死
	线程B因为拿不到金锁头，流程卡死
*/
void CMultithreading::DeadLockMutexThread(bool bIsThreadExecute /* = true */)
{
	if (bIsThreadExecute)
	{
		CMultithreading cmultTemp;

		std::thread thOne(&CMultithreading::DeadLockInsetValue, std::ref(cmultTemp));
		std::thread thTwo(&CMultithreading::DeadLockTakeOutValue, std::ref(cmultTemp));

		thOne.join();
		thTwo.join();
	}
}

void CMultithreading::SolveDeadLock(bool bIsThreadExecute /* = true */)
{
	if (bIsThreadExecute)
	{
		CMultithreading cmultTemp;

		std::thread thOne(&CMultithreading::DeadLockInsetValue, std::ref(cmultTemp));
		std::thread thTwo(&CMultithreading::SolveDeadLockTakeOutValue, std::ref(cmultTemp));

		thOne.join();
		thTwo.join();
	}
}

/*
1.
	unique_lock取代lock_guard
	unique_lock是一个类模板，工作中一般lock_guard(推荐使用)；lock_guard取代了mutex的 lock() 和 unlock()；
	unique_lock比lock_guard更灵活；效率上差一点，内存占用多一点

2.
	unique_lock的第二个参数：
	lock_guard也可以带第二个参数lock_guard<mutex>lockguard(m_mutext,std::adopt_lock); // adopt_lock起标记作用

	adopt_lock_t
		表示互斥量已经加锁,必须要先把互斥量提前加锁，否则会报异常
		作用是通知 lock_guard 或者 unique_lock 不需要在构造函数中lock这个互斥量了

	defer_lock_t
		互斥量没有加锁
		初始化一个没有加锁的互斥量，可以在之后进行lock和unlock操作

	try_to_lock_t
		互斥量尝试去加锁

	unique_lock的成员函数：
	1. lock()
		加锁
	2. unlock()
		解锁
	3. try_lock()
		尝试去加锁，如果拿不到锁返回false，拿到了返回true，这个函数不阻塞
	4. release()
		返回它所管理的 mutex 对象指针，并释放所有权，也就是说当前的 unique_lock 和 mutext 不再有关系



*/
void CMultithreading::Unique_lockInsert(string strParam)
{
	if ("adpot_lock" == strParam)
	{
		for (int i = 0; i < 100000; i++)
		{
			this->m_mutext.lock(); // 使用了adopt_lock如果没有提前加锁，会报出异常
			unique_lock<mutex>uniqlock(m_mutext, std::adopt_lock);

			// 线程睡眠 与QT的 Qthread::sleep异曲同工
			std::chrono::seconds chTime(1);
			std::this_thread::sleep_for(chTime);

			this->m_listValue.push_back(i);
			cout << "插入数据为" << " " << i << endl;
		}
	}
	else if ("defer_lock" == strParam)
	{
		for (int i = 0; i < 100000; i++)
		{
			//this->m_mutext.lock(); // 使用了adopt_lock如果没有提前加锁，会报出异常
			unique_lock<mutex>uniqlock(m_mutext, std::defer_lock);
			uniqlock.lock();

			// ....可以在此处处理共享代码

			uniqlock.unlock();

			this->m_listValue.push_back(i);
			cout << "插入数据为" << " " << i << endl;

			//uniqlock.unlock() 不用再解锁，unique_lock的析构函数会自己解锁
		}
	}
	else if ("try_to_lock" == strParam)
	{
		for (int i = 0; i < 100000; i++)
		{
			unique_lock<mutex>uniqlock(m_mutext, std::try_to_lock);

			// 判断是否拿到了锁
			if (uniqlock.owns_lock())
			{
				this->m_listValue.push_back(i);
				cout << "插入数据为" << " " << i << endl;
			}
			else
			{
				cout << "没有拿到锁" << " " << i << endl;
			}

			//unique_lock<mutex>uniqlockTemp(m_mutext, std::defer_lock);

			//// 判断互斥量尝试去锁有没有成功
			//if (uniqlock.try_lock())
			//{
			//	this->m_listValue.push_back(i);
			//	cout << "插入数据为" << " " << i << endl;
			//}
			//else
			//{
			//	cout << "没有拿到锁" << " " << i << endl;
			//}
		}
	}
	else if ("release" == strParam)
	{
		for (int i = 0; i < 100000; i++)
		{
			unique_lock<mutex>uniqueLock(this->m_mutext);
			std::chrono::seconds(1);

			this->m_listValue.push_back(i);

			mutex* pMutex = nullptr;
			pMutex = uniqueLock.release();

			// 释放完之后由自己管理加锁和解锁
			pMutex->unlock();
		}
	}
}

void CMultithreading::Unique_lockTakeOut(string strParam)
{
	if ("adpot_lock" == strParam)
	{
		for (int i = 0; i < 100000; i++)
		{
			this->m_mutext.lock(); // 使用了adopt_lock如果没有提前加锁，会报出异常
			unique_lock<mutex>uniqlock(m_mutext, std::adopt_lock);

			//unique_lock<mutex>uniqlock(m_mutext);

			if (!m_listValue.empty())
			{
				int iTemp = 0;
				iTemp = this->m_listValue.front();
				this->m_listValue.pop_front();

				cout << "取出数据为：" << iTemp << endl;
			}
			else
			{
				cout << "容器为空" << endl;
			}
		}
	}
	else if ("defer_lock" == strParam)
	{
		unique_lock<mutex>uniqlock(m_mutext);

		if (!m_listValue.empty())
		{
			int iTemp = 0;
			iTemp = this->m_listValue.front();
			this->m_listValue.pop_front();

			cout << "取出数据为：" << iTemp << endl;
		}
		else
		{
			cout << "容器为空" << endl;
		}
	}
	else if ("try_to_lock" == strParam)
	{
		unique_lock<mutex>uniqlock(m_mutext);

		// 线程睡眠 与QT的 Qthread::sleep异曲同工
		std::chrono::seconds chTime(1);
		std::this_thread::sleep_for(chTime);

		if (!m_listValue.empty())
		{
			int iTemp = 0;
			iTemp = this->m_listValue.front();
			this->m_listValue.pop_front();

			cout << "取出数据为：" << iTemp << endl;
		}
		else
		{
			cout << "容器为空" << endl;
		}
	}
	else if ("release" == strParam)
	{
		for (list<int>::iterator it = this->m_listValue.begin(); it != this->m_listValue.end(); it++)
		{
			unique_lock<mutex>uniqueLock(this->m_mutext);
			cout << "取出值为: " << *it << endl;
			uniqueLock.unlock();
		}

		//uniqueLock.release();
	}
}

void CMultithreading::ShowUnqiue_lock(bool bIsThreadExecute /*= true*/)
{
	if (bIsThreadExecute)
	{
		CMultithreading cMult;

		//std::thread thOne(&CMultithreading::Unique_lockInsert,std::ref(cMult),"adpot_lock");
		//std::thread thTwo(&CMultithreading::Unique_lockTakeOut, std::ref(cMult), "adpot_lock");

		//std::thread thOne(&CMultithreading::Unique_lockInsert, std::ref(cMult), "defer_lock");
		//std::thread thTwo(&CMultithreading::Unique_lockTakeOut, std::ref(cMult), "defer_lock");

		//std::thread thOne(&CMultithreading::Unique_lockInsert, std::ref(cMult), "try_to_lock");
		//std::thread thTwo(&CMultithreading::Unique_lockTakeOut, std::ref(cMult), "try_to_lock");

		std::thread thOne(&CMultithreading::Unique_lockInsert, std::ref(cMult), "release");
		std::thread thTwo(&CMultithreading::Unique_lockTakeOut, std::ref(cMult), "release");

		thOne.join();
		thTwo.join();
	}
}

void CMultithreading::SingletonFunction(string strParam)
{
	if ("Static" == strParam)
	{
		CSingletonMode* pSingle = CSingletonMode::getStatic();
		pSingle->FunctionTest();
	}
	else if ("CallOnce" == strParam)
	{
		CSingletonMode* pSingle = CSingletonMode::getCallOnce();
		pSingle->FunctionTest();
	}
}

void CMultithreading::SingletonModel(bool bIsThreadExecute /*= true*/)
{
	if (bIsThreadExecute)
	{
		// 同一个对象
		CSingletonMode* pSingleOne = CSingletonMode::getStatic();
		CSingletonMode* pSingleTwo = CSingletonMode::getStatic();

		std::thread thOne(&CMultithreading::SingletonFunction, this, "Static");
		std::thread thTwo(&CMultithreading::SingletonFunction, this, "Static");

		thOne.join();
		thTwo.join();

		// 错误的回收机制
#ifdef ERRORRECYCLE
		delete pSingleOne;
		delete pSingleTwo;
#endif
	}
}

// call_once 第一个参数是once_flag标志，第二个参数是函数模板-函数名
// call_once 的功能是保证传入的函数只被调用一次
// call_once 具备互斥量的能力，而且效率上比互斥量所使用的资源更少
// 原理是当一个线程执行完call_once的函数时，once_flag这个标记会被置为"已调用"，其他线程再执行时，会直接返回

void CMultithreading::ShowCallOnce(bool bIsThreadExecute /* = true */)
{
	if (bIsThreadExecute)
	{
		std::thread thOne(&CMultithreading::SingletonFunction, this, "CallOnce");
		std::thread thTwo(&CMultithreading::SingletonFunction, this, "CallOnce");

		thOne.join();
		thTwo.join();
	}

	std::condition_variable condvarTest;
	condvarTest.notify_one();

}


void FunctionTransfer(void(*Function)(int), int iOne)
{
	Function(iOne);
}

typedef void(*FunctionOne)(int);

void FunctionTest(FunctionOne function, int iOne)
{
	function(iOne);
}

int main()
{
	//TransferCThreadBasicInfor();

	CMultithreading* pThread = new CMultithreading;

	//线程传入类参数
	pThread->Execute_MyThreadClassTest(false);

	// 线程传入引用参数
	pThread->Execute_MyThreadRefTest(false);

	// 线程传入类的成员函数
	pThread->MutexClassMemberFunction(false);

	// 线程死锁问题
	pThread->DeadLockMutexThread(false);

	// 解决死锁问题
	pThread->SolveDeadLock(false);

	// unique_lock演示
	pThread->ShowUnqiue_lock(false);

	// 单例设计模式
	pThread->SingletonModel(false);

	// 单例模式的call_once
	pThread->ShowCallOnce(false);









	delete pThread;

	cout << "END" << endl;
	return 0;
}



