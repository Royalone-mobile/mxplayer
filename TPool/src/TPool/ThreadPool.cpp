////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 
/// @par ģ����
/// �̳߳ع������
/// @par ����ļ�
/// CThreadPool.h
/// @par ������ϸ����
/// @par ���̰߳�ȫ��
/// [��˵��]
/// @par �쳣ʱ��ȫ��
/// [��˵��]
/// @note         -
/// @file         ThreadPool.cpp
/// @brief        -
/// @author       Li.xl
/// @version      1.0
/// @date         2013/11/19
/// @todo         -
/// 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "ThreadPool.h"

CThreadPool::CThreadPool()
{
	// ��ʼ�����ݳ�Ա
	m_usThreadCount = 0;
	m_TBusyList.clear();
	m_TFreeList.clear();
}

CThreadPool::~CThreadPool()
{
	// �����̳߳�
	i_DestroyPool();
	// �������ݳ�Ա
	m_usThreadCount = 0;
}

bool CThreadPool::e_InitTPool(USHORT usThreadCount)
{
	// ���巵��ֵ
	bool bRet = false;
	// �ж��̸߳�ʽ�Ƿ���
	if(MAX_THREAD_POOL < usThreadCount)
	{
		usThreadCount = MAX_THREAD_POOL;
	}

	// �����̶߳���ָ��
	CThreadBase* pThread = NULL;
	// �����߳�
	for(; m_usThreadCount < usThreadCount; m_usThreadCount++)
	{
		pThread = new CThreadBase(this);
		if(NULL != pThread)
		{
			i_PushToFreeList(pThread);
		}
	}
	// ��֤�߳��Ƿ���ȫ����
	bRet = (m_usThreadCount == usThreadCount);
	return bRet;
}

ULONG CThreadPool::e_GetTaskOrderID()
{
	// ��ȡ����ID
	return m_CTaskList.e_CreatTaskID();
}

bool CThreadPool::e_AddTTask(OnThreadProcess pfnThreadProcess,
	ULONG ulTaskID, USHORT usPriority)
{
	// ���巵��ֵ
	bool bRet = false;
	// ��֤�ص��̷߳����Ƿ�Ϸ�
	if(NULL != pfnThreadProcess)
	{
		// ���ӵ������б�
		bRet = m_CTaskList.e_AddTaskList(pfnThreadProcess,
			ulTaskID, usPriority);
		if(true == bRet)
		{
			// �����߳�����
			e_ProcessTask();
		}
	}
	return bRet;
}

bool CThreadPool::e_DelTTask(ULONG ulTaskID)
{
	// ���巵��ֵ
	bool bRet = false;
	// ɾ��������Ϣ����
	bRet = m_CTaskList.e_DelTask(ulTaskID);
	// ��������
	return bRet;
}

bool CThreadPool::e_ProcessTask(CThreadBase* pThread)
{
	// ���巵��ֵ
	bool bRet = false;
	// ��֤�̶߳����Ƿ�Ϸ�
	if(NULL == pThread)
	{
		// ��ȡ�����б�
		pThread = i_PopFromFreeList();
	}

	// �߳��Ƿ����
	if(NULL != pThread)
	{
		// ��ȡҪ���������
		CTTask* pTTask = m_CTaskList.e_PopTask();
		if(NULL != pTTask)
		{
			// �����߳�
			pThread->e_Run(pTTask);
			// ���ӵ���æ�б�
			i_AddToBusyList(pThread);
			bRet = true;
		}
		else
		{
			// ���û��������, ��ʹ���б����Ƴ�����������߳��б�
			i_DelFromBusyList(pThread);
		}
	}
	return bRet;
}

bool CThreadPool::e_Processed(CThreadBase* pThread)
{
	// ���巵��ֵ
	bool bRet = false;
	// ��֤�̶߳���Ϸ���
	if(NULL != pThread)
	{
		// ��������δ���������
		bRet = e_ProcessTask(pThread);
	}
	return bRet;
}

bool CThreadPool::i_AddToBusyList(CThreadBase* pThread)
{
	// ���巵��ֵ
	bool bRet = false;
	// ��֤�̶߳���ָ��
	if(NULL != pThread)
	{
		// �����б�
		CAutoLock AutoLock(&m_CsTBusyLock);
		// �������ݵ��б�
		if(m_TBusyList.end() == find(m_TBusyList.begin(),
			m_TBusyList.end(), pThread))
		{
			m_TBusyList.push_back(pThread);
		}		
		bRet = true;
	}
	return bRet;
}

bool CThreadPool::i_DelFromBusyList(CThreadBase* pThread)
{
	// ���巵��ֵ
	bool bRet = false;
	// ��֤�̶߳���ָ��
	if(NULL != pThread)
	{
		// �����б�
		m_CsTBusyLock.e_Lock();
		m_TBusyList.remove(pThread);
		m_CsTBusyLock.e_Unlock();
		// ���ӵ������б�
		i_PushToFreeList(pThread);
		bRet = true;
	}
	return bRet;
}

bool CThreadPool::i_PushToFreeList(CThreadBase* pThread)
{
	// ���巵��ֵ
	bool bRet = false;
	// ��ָ֤��Ϸ���
	if(NULL != pThread)
	{
		// ���뵽����ָ�����
		CAutoLock AutoLock(&m_CsTFreeLock);
		// �������ݵ��б�
		if(m_TFreeList.end() == find(m_TFreeList.begin(),
			m_TFreeList.end(), pThread))
		{
			m_TFreeList.push_back(pThread);
		}
		bRet = true;
	}
	return bRet;
}

CThreadBase* CThreadPool::i_PopFromFreeList()
{
	CThreadBase* pThread = NULL;
	// �����б�
	CAutoLock AutoLock(&m_CsTFreeLock);
	// ��֤�б��Ƿ�Ϊ��
	if(false == m_TFreeList.empty())
	{
		// ��ȡ�̳߳����̶߳���
		pThread = m_TFreeList.front();
		m_TFreeList.pop_front();
	}
	// ���ػ�ȡ���̶߳���ָ��
	return pThread;
}

void CThreadPool::i_DestroyPool()
{
	ThreadList::iterator iterBusyPos;
	ThreadQueue::iterator iterFreePos;
	CThreadBase* pThread = NULL;
	m_CsTBusyLock.e_Lock();
	for(iterBusyPos = m_TBusyList.begin(); 
		iterBusyPos != m_TBusyList.end();
		iterBusyPos++)
	{
		// ɾ�������ж���
		pThread = (*iterBusyPos);
		if(NULL != pThread)
		{
			delete pThread;
			pThread = NULL;
		}
	}
	// ����б�
	m_TBusyList.clear();
	m_CsTBusyLock.e_Unlock();

	m_CsTFreeLock.e_Lock();
	for(iterFreePos = m_TFreeList.begin(); 
		iterFreePos != m_TFreeList.end();
		iterFreePos++)
	{
		// ɾ�������ж���
		pThread = (*iterFreePos);
		if(NULL != pThread)
		{
			delete pThread;
			pThread = NULL;
		}
	}
	// ����б�
	m_TFreeList.clear();
	m_CsTFreeLock.e_Unlock();
}