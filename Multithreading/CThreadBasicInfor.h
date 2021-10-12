#pragma once

#include "CMultithreading.h"

class CThreadBasicInfor : public CMultithreading
{
public:
	CThreadBasicInfor();
	~CThreadBasicInfor();

	void GetThreadBasicInfor();


public:
	thread m_thread;

	
};

