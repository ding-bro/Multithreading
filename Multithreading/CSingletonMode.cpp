#include "CSingletonMode.h"

std::mutex g_Mutex;
std::once_flag g_once;

// 调用静态变量时必须要初始化静态变量，而且要带上返回值
// 并且如果你要在别的类中使用这个带有声明了本身类指针的静态变量，就必须把初始化定义在cpp里
// 不然在编译时会出现类重复定义的错误
CSingletonMode* CSingletonMode::s_pSignle = nullptr;

CSingletonMode* CSingletonMode::getStatic()
{
	// 双重检查锁
#ifdef DOUBLELOCK
	if (s_pSignle == nullptr)
	{
		unique_lock<std::mutex>unLock(g_Mutex);
		if (s_pSignle == nullptr)
		{
			s_pSignle = new CSingletonMode;
			// 声明一个内部类对象
			// 在程序结束时会执行机构
#ifdef ERRORRECYCLE
#else
			static CRecycle pCRecycle;
#endif // ERRORRECYCLE
		}
	}
#else
	if (s_pSignle == nullptr)
	{
		unique_lock<std::mutex>unLock(g_Mutex);

		s_pSignle = new CSingletonMode();

		// 声明一个内部类对象
		// 在程序结束时会执行机构
#ifdef ERRORRECYCLE
#else
		static CRecycle pCRecycle;
#endif // ERRORRECYCLE
	}
#endif // DOUBLELOCK

	return s_pSignle;
}

void CSingletonMode::RunCallOnce()
{
	cout << "已执行call_once" << endl;

	if (s_pSignle == nullptr)
	{
		s_pSignle = new CSingletonMode;
		CRecycle cRecyle;
	}
}

CSingletonMode* CSingletonMode::getCallOnce()
{
	// 利用call_once 也能实现与双重检查锁相同的效果
	// call_once 令整个函数实现线程中的阻塞
	// 静态函数直接调用，不用&
	std::call_once(g_once, RunCallOnce);

	cout << "call_once执行完毕" << endl;
	return s_pSignle;
}










