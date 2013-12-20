////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 
/// @par ģ����
/// �ͻ��������б�������
/// @par ����ļ�
/// ContextList.h
/// @par ������ϸ����
/// @par ���̰߳�ȫ��
/// [��˵��]
/// @par �쳣ʱ��ȫ��
/// [��˵��]
/// @note         -
/// @file         ContextList.cpp
/// @brief        -
/// @author       Li.xl
/// @version      1.0
/// @date         2013/06/14
/// @todo         -
/// 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "ContextList.h"

CContextList::CContextList()
{
	START_DEBUG_INFO
	m_ulContextID = 0;
	m_usConFreeNum = NO_NUM_FREE_LIST;
	m_ContextFreeArray.clear();
	m_ContextBusyArray.clear();
	m_pBufferManager = NULL;
	END_DEBUG_INFO
}

/// ����ָ���������б��캯��
CContextList::CContextList(USHORT usConFreeNum)
{
	START_DEBUG_INFO
	m_ulContextID = 0;
	m_ContextFreeArray.clear();
	m_ContextBusyArray.clear();
	m_pBufferManager = NULL;
	/// ��ֵ�б�̶�����
	m_usConFreeNum = usConFreeNum;
	/// ��������
	for(USHORT i = 0; i < m_usConFreeNum; i++)
	{
		T_ClientContext* psttContext = new T_ClientContext();
		if(NULL != psttContext)
		{
			/// �������ݵ��б�
			m_ContextFreeArray.push_back(psttContext);
		}
	}
	END_DEBUG_INFO
}

CContextList::~CContextList()
{
	START_DEBUG_INFO
	/// �ͷ������б�
	i_ClearContextList();
	m_pBufferManager = NULL;
	END_DEBUG_INFO
}

/// ��ʼ��������Ϣ
void CContextList::e_InitContextList(CBufferPool* pBufferPool)
{
	START_DEBUG_INFO
	/// ��֤���ݺϷ���
	if(NULL != pBufferPool)
	{
		m_pBufferManager = pBufferPool;
	}
	END_DEBUG_INFO
}

/// ���ÿ�����������
void CContextList::e_SetFreeConNum(USHORT usConFreeNum)
{
	START_DEBUG_INFO;
	/// ��ֵ�б�̶�����
	m_usConFreeNum = usConFreeNum;
	m_CFreeListCrit.e_Lock();
	if(m_usConFreeNum > m_ContextFreeArray.size())
	{
		USHORT usCreatNum = m_usConFreeNum - m_ContextFreeArray.size();
		/// ��������
		for(USHORT i = 0; i < usCreatNum; i++)
		{
			T_ClientContext* psttContext = new T_ClientContext();
			if(NULL != psttContext)
			{
				/// �������ݵ��б�
				m_ContextFreeArray.push_back(psttContext);
			}
		}
	}
	m_CFreeListCrit.e_Unlock();
	END_DEBUG_INFO;
}

/// ��ȡ���������б��С
USHORT CContextList::e_GetFreeListSize()
{
	START_DEBUG_INFO
	USHORT usListSize = 0;
	m_CFreeListCrit.e_Lock();
	usListSize = m_ContextFreeArray.size();
	m_CFreeListCrit.e_Unlock();
	END_DEBUG_INFO
	return usListSize;
}

/// ��ȡʹ�������б��С
USHORT CContextList::e_GetBusyListSize()
{
	START_DEBUG_INFO
	USHORT usListSize = 0;
	m_CBusyListCrit.e_Lock();
	usListSize = m_ContextBusyArray.size();
	m_CBusyListCrit.e_Unlock();
	END_DEBUG_INFO
	return usListSize;
}

/// ���б��л�ȡ����������Ϣ
bool CContextList::e_AllocateConntext(T_ClientContext*& psttContext)
{
	START_DEBUG_INFO
	bool bRet = false;
	/// ��ʼ��
	psttContext = NULL;
	CContextPtrArray::iterator	ItemFreePos;
	/// ��ȡ���е�������Ϣ����
	m_CFreeListCrit.e_Lock();
	if(0 < m_ContextFreeArray.size())
	{
		ItemFreePos = m_ContextFreeArray.begin();
		psttContext = *ItemFreePos;
		m_ContextFreeArray.erase(ItemFreePos);
	}
	else
	{
		psttContext = new T_ClientContext();
	}
	m_CFreeListCrit.e_Unlock();

	/// ������Ϣ����ʹ�������б�
	if(NULL != psttContext)
	{
		m_CBusyListCrit.e_Lock();
		/// ��ȡ����ID
		psttContext->ulContextID = i_BuildContextID();
		/// �������ü���
		psttContext->e_EnterRefNum();
		/// ��ֵ���ݱ���
		m_ContextBusyArray[psttContext->ulContextID] = psttContext;
		/// ��ֵ����ֵ
		bRet = true;
		m_CBusyListCrit.e_Unlock();
	}
	END_DEBUG_INFO
	return bRet;
}

/// �ͷ�ʹ��������Ϣ
bool CContextList::e_ReleaseConntext(T_ClientContext*& psttContext)
{
	START_DEBUG_INFO
	bool bRet = false;
	/// ��ָ֤�����
	if(NULL == psttContext)
	{
		END_DEBUG_INFO
		return bRet;
	}

	/// ����ʹ���б�
	m_CBusyListCrit.e_Lock();
	CContextMapULONGToPtr::iterator ItemPos;
	ItemPos = m_ContextBusyArray.find(psttContext->ulContextID);
	if(ItemPos != m_ContextBusyArray.end())
	{
		if(0 == psttContext->e_ExitRefNum())
		{
			/// ��֤���ݺϷ���
			if(NULL != m_pBufferManager)
			{
				m_pBufferManager->e_ReleaseBuffer(psttContext->pCurrentBuffer);
			}
			psttContext->pCurrentBuffer = NULL;
			/// �ͷ��б�
			i_ReleaseBufferMap(&psttContext->CReadBufferMap);
			psttContext->CReadBufferMap.clear();
			i_ReleaseBufferMap(&psttContext->CSendBufferMap);
			psttContext->CSendBufferMap.clear();
			///�Ƴ��б��ͷ�����
			m_ContextBusyArray.erase(ItemPos);
			bRet = true;
		}
	}
	m_CBusyListCrit.e_Unlock();

	/// �����ʹ���б����Ƴ�����ɹ�
	if(true == bRet)
	{
		/// ��������б�
		if(false == i_AddContextToFreeList(psttContext))
		{
			/// ɾ������
			delete psttContext;
			psttContext = NULL;
		}
	}
	END_DEBUG_INFO
	return bRet;
}

/// ���б��л�ȡ����������Ϣ
T_ClientContext* CContextList::e_GetConntext(ULONG ulContextID)
{
	START_DEBUG_INFO
	/// ��ʼ��
	T_ClientContext* psttContext = NULL;
	CContextMapULONGToPtr::iterator ItemPos;
	/// �����б�
	m_CBusyListCrit.e_Lock();
	ItemPos = m_ContextBusyArray.find(ulContextID);
	if(ItemPos != m_ContextBusyArray.end())
	{
		/// ��ֵ����
		psttContext = ItemPos->second;
		/// �������ü���
		psttContext->e_EnterRefNum();
	}
	m_CBusyListCrit.e_Unlock();
	END_DEBUG_INFO
	return psttContext;
}

/// ����������Ϣ�������б�
bool CContextList::i_AddContextToFreeList(T_ClientContext* psttContext)
{
	START_DEBUG_INFO
	bool bRet = false;
	/// ��֤���ݺϷ���
	if(NULL == psttContext)
	{
		END_DEBUG_INFO
		return bRet;
	}
	/// �����б�
	m_CFreeListCrit.e_Lock();
	/// ����޹̶��б�����������δ�ﵽ�̶�����
	if(NO_NUM_FREE_LIST == m_usConFreeNum || m_usConFreeNum > m_ContextFreeArray.size())
	{
		/// ����������Ϣ�ṹ
		psttContext->e_InitStruct();
		m_ContextFreeArray.push_back(psttContext);
		bRet = true;
	}
	m_CFreeListCrit.e_Unlock();
	END_DEBUG_INFO
	return bRet;
}

/// ���������Ϣ�б�
void CContextList::i_ClearContextList()
{
	START_DEBUG_INFO
	T_ClientContext* psttContext = NULL;
	CContextMapULONGToPtr::iterator iterPos;
	m_CFreeListCrit.e_Lock();
	/// ��ȡ�б��С
	USHORT usFreeSize = m_ContextFreeArray.size();
	/// ѭ���ͷ�����
	for(USHORT i = 0; i < usFreeSize; i++)
	{
		/// ��ȡ���ݶ���
		psttContext = m_ContextFreeArray.at(i);
		if(NULL != psttContext)
		{
			delete psttContext;
			psttContext = NULL;
		}
	}
	m_ContextFreeArray.clear();
	m_CFreeListCrit.e_Unlock();

	m_CBusyListCrit.e_Lock();
	/// ѭ���ͷ�����
	for(iterPos = m_ContextBusyArray.begin(); iterPos != m_ContextBusyArray.end(); iterPos++)
	{
		/// ��ȡ���ݶ���
		psttContext = iterPos->second;
		if(NULL != psttContext)
		{
			delete psttContext;
			psttContext = NULL;
		}
	}
	m_ContextBusyArray.clear();
	m_CBusyListCrit.e_Unlock();
	END_DEBUG_INFO
}

/// ����������ϢID
ULONG CContextList::i_BuildContextID()
{
	START_DEBUG_INFO
	if(0 == ++m_ulContextID)
	{
		m_ulContextID++;
	}
	/// �������ɵ������Ա��
	END_DEBUG_INFO
	return m_ulContextID;
}

void CContextList::i_ReleaseBufferMap(CBufferMapToPtr* pBufferMap)
{
	START_DEBUG_INFO;
	/// ��֤���ݺϷ���
	if(NULL == m_pBufferManager || NULL == pBufferMap)
	{		
		END_DEBUG_INFO
		return;
	}

	CNetBuffer* pNetBuffer = NULL;
	CBufferMapToPtr::iterator ItemPos;
	for(ItemPos = pBufferMap->begin(); ItemPos != pBufferMap->end(); ItemPos++)
	{
		pNetBuffer = ItemPos->second;
		if(NULL != pNetBuffer)
		{
			/// �ͷ����ݻ������
			m_pBufferManager->e_ReleaseBuffer(pNetBuffer);
		}
	}
	END_DEBUG_INFO;
}