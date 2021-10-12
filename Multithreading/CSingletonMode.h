#pragma once

#include "CMultithreading.h"

// ����ģʽ�����������ʽ
//#define ERRORRECYCLE

// ����ģʽ����˫�ؼ����
#define DOUBLELOCK

extern std::mutex g_Mutex;
extern std::once_flag g_once;

// �������ģʽ
class CSingletonMode : public CMultithreading
{
public:
	CSingletonMode() {}

	// ����Ļ��ջ��ƣ�delete������������ָ����ظ�ѭ��������������
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

	// ����һ���ڲ���
	class CRecycle
	{
	public:
		~CRecycle()
		{
			if (CSingletonMode::s_pSignle)
			{
				// ��ʱɾ���Ͳ���ɾ���������ָ��
				delete CSingletonMode::s_pSignle;
				CSingletonMode::s_pSignle = nullptr;
			}
		}
	};

	// ʹ�þ�̬��Ա�����ķ�ʽ����
	static CSingletonMode* getStatic();

	static CSingletonMode* getCallOnce();

	static void RunCallOnce();

	void FunctionTest() {}

public:
	// ��ʼ����cpp��
	static CSingletonMode* s_pSignle;
};

