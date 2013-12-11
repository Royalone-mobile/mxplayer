////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 
/// @par ģ����
/// �̳߳ص��ýӿ�ģ��
/// @par ����ļ�
/// ITPool.h
/// @par ������ϸ����
/// @par ���̰߳�ȫ��
/// [��˵��]
/// @par �쳣ʱ��ȫ��
/// [��˵��]
/// @note         -
/// @file         ITPool.cpp
/// @brief        -
/// @author       Li.xl
/// @version      1.0
/// @date         2013/11/19
/// @todo         -
/// 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "ThreadPool.h"
#include "TPool/ITPool.h"

CThreadPool* m_pThreadPool = NULL;

CITPool::CITPool()
{
	m_pThreadPool = new CThreadPool();
}

CITPool::~CITPool()
{
	if(NULL != m_pThreadPool)
	{
		delete m_pThreadPool;
		m_pThreadPool = NULL;
	}
}

bool CITPool::e_IInitTPool(USHORT usThreadCount)
{
	// ��ʼ���̳߳�
	return m_pThreadPool->e_InitTPool(usThreadCount);
}

ULONG CITPool::e_IGetTTaskID()
{
	// ��ȡ����Ψһ��ʶ
	return m_pThreadPool->e_GetTaskOrderID();
}

bool CITPool::e_IAddPoolTask(OnThreadProcess pfnThreadProcess, 
	ULONG ulTaskID, USHORT usPriority)
{
	// ����������Ϣ
	return m_pThreadPool->e_AddTTask(pfnThreadProcess, ulTaskID, usPriority);
}

bool CITPool::e_IDelPoolTask(ULONG ulTaskID)
{
	// ɾ������ID��Ӧ������
	return m_pThreadPool->e_DelTTask(ulTaskID);
}