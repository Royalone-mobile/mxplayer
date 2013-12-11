////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 
/// @par ģ����
/// �߳�������Ϣ����
/// @par ����ļ�
/// TTask.h
/// @par ������ϸ����
/// @par ���̰߳�ȫ��
/// [��˵��]
/// @par �쳣ʱ��ȫ��
/// [��˵��]
/// @note         -
/// @file         TTask.cpp
/// @brief        -
/// @author       Li.xl
/// @version      1.0
/// @date         2013/11/19
/// @todo         -
/// 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "TTask.h"

CTTask::CTTask(void* pFunction)
{
	// ��ʼ�������Ա
	m_ulTaskID = 0;
	m_usPriority = ETTP_NORMAL;
	m_pfnThreadProcess = (OnThreadProcess)pFunction;
}

CTTask::~CTTask()
{
	// ���������Ա��Ϣ
	m_ulTaskID = 0;
	m_usPriority = ETTP_NORMAL;
	m_pfnThreadProcess = NULL;
}

ULONG CTTask::e_GetTaskID()
{
	return m_ulTaskID;
}

void CTTask::e_SetTaskID(ULONG ulTaskID)
{
	m_ulTaskID = ulTaskID;
}

USHORT CTTask::e_GetPriority()
{
	return m_usPriority;
}

void CTTask::e_SetPriority(USHORT usPriority)
{
	m_usPriority = usPriority;
}

bool CTTask::e_TTaskProcess()
{
	// ���巵��ֵ
	bool bRet = false;
	// ��֤����ָ����ȷ��
	if (NULL != m_pfnThreadProcess)
	{
		// �ص��¼���������
		bRet = m_pfnThreadProcess(m_ulTaskID);
	}
	// ���ش�����
	return bRet;
}