#pragma once

#include "CMultithreading.h"

// 单例模式错误的析构方式
//#define ERRORRECYCLE

// 单例模式构造双重检查锁
#define DOUBLELOCK

extern std::mutex g_Mutex;
extern std::once_flag g_once;

// 单例设计模式
class CSingletonMode : public CMultithreading
{
public:
	CSingletonMode() {}

	// 错误的回收机制，delete自身类声明的指针会重复循环调用析构函数
#ifdef ERRORRECYCLE
	~CSingletonMode()
	{
		if (CSingletonMode::s_pSignle)
		{
			delete CSingletonMode::s_pSignle;
			CSingletonMode::s_pSignle = nullptr;
		}
	}
#else

#endif

	// 声明一个内部类
	class CRecycle
	{
	public:
		~CRecycle()
		{
			if (CSingletonMode::s_pSignle)
			{
				// 此时删除就不是删除自身类的指针
				delete CSingletonMode::s_pSignle;
				CSingletonMode::s_pSignle = nullptr;
			}
		}
	};

	// 使用静态成员函数的方式调用
	static CSingletonMode* getStatic();

	static CSingletonMode* getCallOnce();

	static void RunCallOnce();

	void FunctionTest() {}

public:
	// 初始化在cpp里
	static CSingletonMode* s_pSignle;
};

