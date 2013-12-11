////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 
/// @par ģ����
/// �߳������б�������
/// @par ����ļ�
/// TTaskList.h
/// @par ������ϸ����
/// @par ���̰߳�ȫ��
/// [��˵��]
/// @par �쳣ʱ��ȫ��
/// [��˵��]
/// @note         -
/// @file         TTaskList.cpp
/// @brief        -
/// @author       Li.xl
/// @version      1.0
/// @date         2013/11/19
/// @todo         -
/// 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "TTaskList.h"

CTTaskList::CTTaskList()
{
	// ��ʼ�������б��Ա��Ϣ
	m_ulCurTaskOrder = 0;
	m_CTaskQueue.clear();
}

CTTaskList::~CTTaskList()
{
	// ��ʼ�������б��Ա��Ϣ
	m_ulCurTaskOrder = 0;
	// ����б�
	e_ClearList();
}

ULONG CTTaskList::e_CreatTaskID()
{
	// �����������
	CAutoLock AutoLock(&m_CsOrderLock);
	if(0 == ++m_ulCurTaskOrder)
	{
		m_ulCurTaskOrder++;
	}
	return m_ulCurTaskOrder;
}

bool CTTaskList::e_AddTaskList(OnThreadProcess pfnThreadProcess, ULONG ulTaskID, USHORT usPriority)
{
	bool bRet = false;
	// ��ͨȨ��
	if(ETTP_NORMAL == usPriority)
	{
		bRet = e_PushTask(pfnThreadProcess, ulTaskID);
	}
	else
	{
		bRet = e_FrontTask(pfnThreadProcess, ulTaskID);
	}
	return bRet;
}

bool CTTaskList::e_PushTask(OnThreadProcess pfnThreadProcess, ULONG ulTaskID)
{
	bool bRet = false;
	// ����������Ϣ, ��ͨ���ȼ�
	CTTask* pTTask = new CTTask(pfnThreadProcess);
	if(NULL != pTTask)
	{
		pTTask->e_SetTaskID(ulTaskID);
		// �������񵽶���β��
		CAutoLock AutoLock(&m_CsTaskLock);
		m_CTaskQueue.push_back(pTTask);
		bRet = true;
	}
	return bRet;
}

bool CTTaskList::e_FrontTask(OnThreadProcess pfnThreadProcess, ULONG ulTaskID)
{
	bool bRet = false;
	// ����������Ϣ, �߼����ȼ�
	CTTask* pTTask = new CTTask(pfnThreadProcess);
	if(NULL != pTTask)
	{
		pTTask->e_SetTaskID(ulTaskID);
		pTTask->e_SetPriority(ETTP_HIGHT);
		// �������񵽶���ͷ��
		CAutoLock AutoLock(&m_CsTaskLock);
		m_CTaskQueue.push_front(pTTask);
		bRet = true;
	}
	return bRet;
}

CTTask* CTTaskList::e_PopTask()
{
	CTTask* pTTask = NULL;
	// �Ӷ����л�ȡ�������
	CAutoLock AutoLock(&m_CsTaskLock);
	// �ж϶����Ƿ�Ϊ��
	if(false == m_CTaskQueue.empty())
	{
		// ȡ������ͷ����
		pTTask = m_CTaskQueue.front();
		// �Ƴ�����ͷ
		m_CTaskQueue.pop_front();
	}
	// ����������Ϣ
	return pTTask;
}

bool CTTaskList::e_DelTask(ULONG ulTaskID)
{
	// ���巵��ֵ
	bool bRet = false;
	TQueue::iterator iterPos;
	CTTask* pTTask = NULL;
	CAutoLock AutoLock(&m_CsTaskLock);
	// ���Ҷ�Ӧ������
	for(iterPos = m_CTaskQueue.begin(); 
		iterPos != m_CTaskQueue.end();
		iterPos++)
	{
		pTTask = (*iterPos);
		if(NULL != pTTask && ulTaskID == pTTask->e_GetTaskID())
		{
			m_CTaskQueue.erase(iterPos);
			// ɾ���������
			delete pTTask;
			pTTask = NULL;
			bRet = true;
			break;
		}
	}
	return bRet;
}

bool CTTaskList::e_ClearList()
{
	TQueue::iterator iterPos;
	CTTask* pTTask = NULL;
	CAutoLock AutoLock(&m_CsTaskLock);
	for(iterPos = m_CTaskQueue.begin(); 
		iterPos != m_CTaskQueue.end();
		iterPos++)
	{
		// ɾ�������ж���
		pTTask = (*iterPos);
		if(NULL != pTTask)
		{
			delete pTTask;
			pTTask = NULL;
		}
	}
	// ����б�
	m_CTaskQueue.clear();
	return true;
}