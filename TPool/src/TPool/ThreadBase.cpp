/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 
/// @par ģ����
/// �����̶߳���
/// @par ����ļ�
/// ThreadBase.cpp
/// @par ������ϸ����
/// @par ���̰߳�ȫ��
/// [��˵��]
/// @par �쳣ʱ��ȫ��
/// [��˵��]
/// @note         -
/// @file         ThreadBase.h
/// @brief        -
/// @author       Li.xl
/// @version      1.0
/// @date         2013/11/18
/// @todo         -
/// 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "ThreadBase.h"
#include "ThreadPool.h"

CThreadBase::CThreadBase(CThreadPool* pThreadPool)
{
	m_bIsRuning = false;
	m_hTheadHandle = NULL;
	m_hThreadEven = NULL;
	m_pTTask = NULL;
	m_pThreadPool = pThreadPool;
	// ��ʼ���߳������¼��ź�
	m_hThreadEven = CreateEvent(NULL, FALSE, FALSE, NULL);
	// �����߳�
	e_Creat();
}

CThreadBase::~CThreadBase()
{
	// �������ݱ���
	m_pTTask = NULL;
	m_pThreadPool = NULL;
	// �ͷ��߳�
	e_Destroy();
}

bool CThreadBase::e_Creat()
{
	// ��֤�߳��Ƿ��Ѿ�����
	if(NULL == m_hTheadHandle)
	{
		// �����߳�
		m_hTheadHandle = (HANDLE)_beginthreadex(NULL, 0, i_StartThread, this, 0, NULL);
		m_bIsRuning = (NULL != m_hTheadHandle);
	}
	// �����߳̾���Ƿ�ɹ�
	return m_bIsRuning;
}

bool CThreadBase::e_Run(CTTask* pTTask)
{
	// ��֤�߳��������ָ��
	if(NULL != pTTask)
	{
		m_pTTask = pTTask;
	}
	// �����̴߳����¼��ź�
	SetEvent(m_hThreadEven);
	return true;
}

bool CThreadBase::e_Destroy()
{
	// ��֤�߳��Ƿ����
	if(NULL != m_hTheadHandle)
	{
		// ����ֹͣ����
		m_bIsRuning = false;
		// �����߳��¼��ź���
		SetEvent(m_hThreadEven);
		// ��ȡ�̶߳������ź�
		WaitForSingleObject(m_hTheadHandle, INFINITE);
		// �ͷž��
		CloseHandle(m_hTheadHandle);
		m_hTheadHandle = NULL;
		CloseHandle(m_hThreadEven);
		m_hThreadEven = NULL;
	}
	// ɾ���������
	i_DelTask();
	// �����ͷ��̳߳ɹ�
	return true;
}

bool CThreadBase::e_GetRunStatus()
{
	// ��������״̬
	return m_bIsRuning;
}

void CThreadBase::e_ThreadProcess()
{
	// �����߳�
	while(true == m_bIsRuning)
	{
		/// �ȴ�ִ���¼�
		WaitForSingleObject(m_hThreadEven, INFINITE);
		/// ָ���Ĵ����¼���ΪNULL���ص�����
		if(NULL != m_pTTask)
		{
			// ִ�������߳�
			m_pTTask->e_TTaskProcess();
			// ɾ���������
			i_DelTask();
			// ֹͣ�̳߳أ������Ѵ������
			if(NULL != m_pThreadPool)
			{
				m_pThreadPool->e_Processed(this);
			}
		}
	}
}

bool CThreadBase::i_DelTask()
{
	// ���巵��ֵ
	bool bRet = false;
	if(NULL != m_pTTask)
	{
		// ɾ���������
		delete m_pTTask;
		m_pTTask = NULL;
		bRet = true;
	}
	return bRet;
}

UINT WINAPI CThreadBase::i_StartThread(LPVOID lpParama)
{
	CThreadBase* pThis = (CThreadBase* )lpParama;
	if(NULL != pThis)
	{
		// ��ʼ�����߳�ѭ��
		pThis->e_ThreadProcess();
	}
	return 0;
}