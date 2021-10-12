#include "CSingletonMode.h"

std::mutex g_Mutex;
std::once_flag g_once;

// ���þ�̬����ʱ����Ҫ��ʼ����̬����������Ҫ���Ϸ���ֵ
// ���������Ҫ�ڱ������ʹ��������������˱�����ָ��ľ�̬�������ͱ���ѳ�ʼ��������cpp��
// ��Ȼ�ڱ���ʱ��������ظ�����Ĵ���
CSingletonMode* CSingletonMode::s_pSignle = nullptr;

CSingletonMode* CSingletonMode::getStatic()
{
	// ˫�ؼ����
#ifdef DOUBLELOCK
	if (s_pSignle == nullptr)
	{
		unique_lock<std::mutex>unLock(g_Mutex);
		if (s_pSignle == nullptr)
		{
			s_pSignle = new CSingletonMode;
			// ����һ���ڲ������
			// �ڳ������ʱ��ִ�л���
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

		// ����һ���ڲ������
		// �ڳ������ʱ��ִ�л���
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
	cout << "��ִ��call_once" << endl;

	if (s_pSignle == nullptr)
	{
		s_pSignle = new CSingletonMode;
		CRecycle cRecyle;
	}
}

CSingletonMode* CSingletonMode::getCallOnce()
{
	// ����call_once Ҳ��ʵ����˫�ؼ������ͬ��Ч��
	// call_once ����������ʵ���߳��е�����
	// ��̬����ֱ�ӵ��ã�����&
	std::call_once(g_once, RunCallOnce);

	cout << "call_onceִ�����" << endl;
	return s_pSignle;
}










