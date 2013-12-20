////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 
/// @par ģ����
/// ���ݻ����б������󣨻���أ�
/// @par ����ļ�
/// BufferPool.h
/// @par ������ϸ����
/// @par ���̰߳�ȫ��
/// [��˵��]
/// @par �쳣ʱ��ȫ��
/// [��˵��]
/// @note         -
/// @file         BufferPool.cpp
/// @brief        -
/// @author       Li.xl
/// @version      1.0
/// @date         2013/06/14
/// @todo         -
/// 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "BufferPool.h"

CBufferPool::CBufferPool()
{
	START_DEBUG_INFO
	/// ��ʼ���������
	m_ulConstomBufferID = 0;
	m_usFreeBufCount = NO_NUM_FREE_LIST;
	m_ArrayFreeBufList.clear();
	m_ArrayBusyBufList.clear();
	END_DEBUG_INFO
}

/// ����ָ���������б��캯��
CBufferPool::CBufferPool(USHORT usBufCount)
{
	START_DEBUG_INFO
	/// ��ʼ������������ֵ
	m_ulConstomBufferID = 0;
	m_ArrayFreeBufList.clear();
	m_ArrayBusyBufList.clear();
	/// ��ֵ�б�̶�����
	m_usFreeBufCount = usBufCount;
	/// �������ݶ���
	for(USHORT i = 0; i < m_usFreeBufCount; i++)
	{
		CNetBuffer* pNetBuffer = new CNetBuffer();
		if(NULL != pNetBuffer)
		{
			/// �������ݵ��б�
			m_ArrayFreeBufList.push_back(pNetBuffer);
		}
	}
	END_DEBUG_INFO
}

CBufferPool::~CBufferPool()
{
	START_DEBUG_INFO
	i_ClearBufferList();
	END_DEBUG_INFO
}

/// ���ÿ��л�������
void CBufferPool::e_SetBuffFreeCount(USHORT usBufCount)
{
	START_DEBUG_INFO;
	/// ��ֵ�б�̶�����
	m_usFreeBufCount = usBufCount;
	m_CFreeListCrit.e_Lock();
	if(m_usFreeBufCount > m_ArrayFreeBufList.size())
	{
		USHORT usCreatNum = m_usFreeBufCount - m_ArrayFreeBufList.size();
		/// ��������
		for(USHORT i = 0; i < usCreatNum; i++)
		{
			CNetBuffer* pNetBuffer = new CNetBuffer();
			if(NULL != pNetBuffer)
			{
				/// �������ݵ��б�
				m_ArrayFreeBufList.push_back(pNetBuffer);
			}
		}
	}
	m_CFreeListCrit.e_Unlock();
	END_DEBUG_INFO;
}

/// ��ȡ���л����б��С
USHORT CBufferPool::e_GetFreeListSize()
{
	START_DEBUG_INFO
	USHORT usListSize = 0;
	m_CFreeListCrit.e_Lock();
	usListSize = m_ArrayFreeBufList.size();
	m_CFreeListCrit.e_Unlock();
	END_DEBUG_INFO
	return usListSize;
}

/// ��ȡʹ�û����б��С
USHORT CBufferPool::e_GetBusyListSize()
{
	START_DEBUG_INFO
	USHORT usListSize = 0;
	m_CBusyListCrit.e_Lock();
	usListSize = m_ArrayBusyBufList.size();
	m_CBusyListCrit.e_Unlock();
	END_DEBUG_INFO
	return usListSize;
}

/// ���б��л�ȡ���û������ݶ�����Ϣ
bool CBufferPool::e_AllocateBuffer(CNetBuffer*& pNetBuffer, USHORT usIOType)
{
	START_DEBUG_INFO
	bool bRet = false;
	/// ��ʼ��
	pNetBuffer = NULL;
	CBufferPtrArray::iterator FreeItemPos;
	/// ��ȡ���е�������Ϣ����
	m_CFreeListCrit.e_Lock();
	if(0 < m_ArrayFreeBufList.size())
	{
		FreeItemPos = m_ArrayFreeBufList.begin();
		pNetBuffer = *FreeItemPos;
		m_ArrayFreeBufList.erase(FreeItemPos);
	}
	else
	{
		pNetBuffer = new CNetBuffer();
	}
	m_CFreeListCrit.e_Unlock();

	/// ������Ϣ����ʹ�������б�
	if(NULL != pNetBuffer)
	{
		m_CBusyListCrit.e_Lock();
		/// ���û�������б�ID
		pNetBuffer->e_SetBufferID(i_BuildNetBufferID());
		pNetBuffer->e_SetOperation(usIOType);
		/// ��ֵ���ݱ���
		m_ArrayBusyBufList[pNetBuffer->e_GetBufferID()] = pNetBuffer;
		/// ��ֵ����ֵ
		bRet = true;
		m_CBusyListCrit.e_Unlock();
	}
	END_DEBUG_INFO
	return bRet;
}

bool CBufferPool::e_SplitNetBuffer(CNetBuffer*& pSrcBuffer, CNetBuffer*& pDesBuffer, UINT unSplitSize)
{
	START_DEBUG_INFO
	if(NULL == pSrcBuffer || unSplitSize > pSrcBuffer->e_GetUsed())
	{
		END_DEBUG_INFO
		return false;
	}
	/// ��ȡ�������ݶ���
	e_AllocateBuffer(pDesBuffer, pSrcBuffer->e_GetOperation());
	/// ������󲻴����򷵻�
	if(NULL == pDesBuffer)
	{
		END_DEBUG_INFO
		return false;
	}

	/// ���û������к�
	pDesBuffer->e_SetBufferSeqNum(pSrcBuffer->e_GetBufferSeqNum());
	/// ����һ����С�Ļ������ݵ������Ļ������
	if(false == pDesBuffer->e_AddData(pSrcBuffer->e_GetBuffer(), unSplitSize))
	{
		/// ����ʧ�ܣ���ɾ��
		e_ReleaseBuffer(pDesBuffer);
		/// ����NULL
		END_DEBUG_INFO
		return false;
	}
	/// �ͷ�һ����С��ԭ������
	if(false == pSrcBuffer->e_FlushBuffer(unSplitSize))
	{
		/// �ͷ�һ����Сԭ������ʧ����ɾ����������ʱ�������
		e_ReleaseBuffer(pDesBuffer);
		/// ����NULL
		END_DEBUG_INFO
		return false;
	}
	/// ���ز�ֺ�����ݻ������
	END_DEBUG_INFO
	return true;
}

/// �ͷ�ʹ�õĻ������
bool CBufferPool::e_ReleaseBuffer(CNetBuffer*& pNetBuffer)
{
	START_DEBUG_INFO
	bool bRet = false;
	/// ��ָ֤�����
	if(NULL == pNetBuffer)
	{
		END_DEBUG_INFO
		return bRet;
	}

	/// ����MAP����
	CBufferMapToPtr::iterator ItemPos;
	/// ����ʹ���б�
	m_CBusyListCrit.e_Lock();
	ItemPos = m_ArrayBusyBufList.find(pNetBuffer->e_GetBufferID());
	if(ItemPos != m_ArrayBusyBufList.end())
	{
		///�Ƴ��б��ͷ�����
		ItemPos = m_ArrayBusyBufList.erase(ItemPos);
		bRet = true;
	}
	m_CBusyListCrit.e_Unlock();

	/// �����ʹ���б����Ƴ�����ɹ�
	if(true == bRet)
	{
		/// ��������б�
		if(false == i_AddBufferToFreeList(pNetBuffer))
		{
			/// ɾ������
			delete pNetBuffer;
			pNetBuffer = NULL;
		}
	}
	END_DEBUG_INFO
	return bRet;
}

bool CBufferPool::i_AddBufferToFreeList(CNetBuffer*& pNetBuffer)
{
	START_DEBUG_INFO
	bool bRet = false;
	/// ��֤���ݺϷ���
	if(NULL == pNetBuffer)
	{
		END_DEBUG_INFO
		return bRet;
	}
	/// �����б�
	m_CFreeListCrit.e_Lock();
	/// ����޹̶��б�����������δ�ﵽ�̶�����
	if(NO_NUM_FREE_LIST == m_usFreeBufCount || m_usFreeBufCount > m_ArrayFreeBufList.size())
	{
		pNetBuffer->e_ReSetBuffer();
		m_ArrayFreeBufList.push_back(pNetBuffer);
		bRet = true;
	}
	m_CFreeListCrit.e_Unlock();
	END_DEBUG_INFO
	return bRet;
}

/// ������ݻ����б�
void CBufferPool::i_ClearBufferList()
{
	START_DEBUG_INFO
	CNetBuffer* pNetBuffer = NULL;
	CBufferMapToPtr::iterator iterPos;
	m_CFreeListCrit.e_Lock();
	/// ѭ���ͷ�����
	int nBufferSize = (int)m_ArrayFreeBufList.size();
	for(int i = 0; i < nBufferSize; i++)
	{
		pNetBuffer = m_ArrayFreeBufList.at(i);
		if(NULL != pNetBuffer)
		{
			delete pNetBuffer;
			pNetBuffer = NULL;
		}
	}
	m_ArrayFreeBufList.clear();
	m_CFreeListCrit.e_Unlock();

	m_CBusyListCrit.e_Lock();
	/// ѭ���ͷ�����
	for(iterPos = m_ArrayBusyBufList.begin(); iterPos != m_ArrayBusyBufList.end(); iterPos++)
	{
		/// ��ȡ���ݶ���
		pNetBuffer = iterPos->second;
		if(NULL != pNetBuffer)
		{
			delete pNetBuffer;
			pNetBuffer = NULL;
		}
	}
	m_ArrayBusyBufList.clear();
	m_CBusyListCrit.e_Unlock();
	END_DEBUG_INFO
}

/// ���ɻ���ID
ULONG CBufferPool::i_BuildNetBufferID()
{
	START_DEBUG_INFO
	m_ulConstomBufferID++;
	/// �������ɵ������Ա��
	END_DEBUG_INFO
	return m_ulConstomBufferID;
}