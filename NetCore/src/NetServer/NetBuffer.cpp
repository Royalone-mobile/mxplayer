////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 
/// @par ģ����
/// �������ݻ������
/// @par ����ļ�
/// CNetBuffer.h
/// @par ������ϸ����
/// @par ���̰߳�ȫ��
/// [��˵��]
/// @par �쳣ʱ��ȫ��
/// [��˵��]
/// @note         -
/// @file         CNetBuffer.cpp
/// @brief        -
/// @author       Li.xl
/// @version      1.0
/// @date         2013/06/13
/// @todo         -
/// 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "NetBuffer.h"

CNetBuffer::CNetBuffer()
{
	START_DEBUG_INFO
	/// ��ʼ�����ݱ���
	m_ulBufferID = 0;
	m_ulBuffSeqNum = 0;
	m_unUsed = 0;
	m_usIOProType = IOPT_None;
	memset(m_Buffer, 0x00, MAX_NET_BUFFER_SIZE);
	memset(&m_sttWsaBuf, 0x00, sizeof(WSABUF));	
	memset(&m_sttIOOverLapped, 0x00, sizeof(OVERLAPPED));
	memset(&m_sttUDPAddrInfo, 0x00, sizeof(SOCKADDR_IN));
	m_nAddressLen = sizeof(SOCKADDR_IN);
	END_DEBUG_INFO
}

CNetBuffer::~CNetBuffer()
{
	START_DEBUG_INFO
	END_DEBUG_INFO
}

/// �������ݵ����ݻ���
bool CNetBuffer::e_AddData(const BYTE* pBtData, UINT unSize)
{
	START_DEBUG_INFO
	/// ���巵��ֵ
	bool bRet = false;
	if(NULL == pBtData || 0 == unSize)
	{
		END_DEBUG_INFO
		return bRet;
	}
	/// ��֤���ݺϷ���
	if(unSize <= MAX_NET_BUFFER_SIZE - m_unUsed)
	{
		/// �������ݵ�����
		memcpy(m_Buffer + m_unUsed, pBtData, unSize);
		m_unUsed += unSize;
		bRet = true;
	}
	END_DEBUG_INFO
	return bRet;
}

/// �ӻ����л�ȡ����
bool CNetBuffer::e_GetData(BYTE*& pBtData, UINT unSize)
{
	START_DEBUG_INFO
	/// ��֤���ݺϷ���
	if(NULL == pBtData || m_unUsed < unSize)
	{
		END_DEBUG_INFO
		return false;
	}
	/// ��ȡ���ݰ�����
	memmove(pBtData, m_Buffer, unSize);	
	m_unUsed -= unSize;
	memmove(m_Buffer, m_Buffer + unSize, m_unUsed);
	/// �����Ƴ��Ĳ�������
	memset(m_Buffer + m_unUsed, 0x00, unSize);
	END_DEBUG_INFO
	return true;
}

/// �ӻ����л�ȡ����
BYTE* CNetBuffer::e_GetBuffer()
{
	START_DEBUG_INFO
	END_DEBUG_INFO
	return m_Buffer;
}

/// �ӻ������Ƴ�����
bool CNetBuffer::e_FlushBuffer(UINT unRemoveSize)
{
	START_DEBUG_INFO
	/// ��ȡ���ݰ�����, ��֤���ݴ�С�Ƿ����ʹ�õ����ݻ���
	if(MAX_NET_BUFFER_SIZE < unRemoveSize || m_unUsed < unRemoveSize)
	{
		END_DEBUG_INFO
		return false;
	}
	/// ���ڴ����Ƴ�һ����С�Ļ���
	m_unUsed -= unRemoveSize;
	memmove(m_Buffer, m_Buffer + unRemoveSize, m_unUsed);
	/// �����Ƴ��Ĳ�������
	memset(m_Buffer + m_unUsed, 0x00, unRemoveSize);
	END_DEBUG_INFO
	return true;
}

bool CNetBuffer::e_AddMoveOtherBuffer(CNetBuffer*& pOtherBuffer, UINT usMoveSize)
{
	START_DEBUG_INFO
	bool bRet = false;
	/// ��֤���ݺϷ���
	if(NULL == pOtherBuffer)
	{
		END_DEBUG_INFO
		return bRet;
	}

	/// ��ֵ���󻺳�
	if(usMoveSize <= MAX_NET_BUFFER_SIZE - m_unUsed)
	{
		/// ��ȡ�������������
		if(true == e_AddData(pOtherBuffer->e_GetBuffer(), usMoveSize))
		{
			/// �Ƴ����������ݿ�
			if(true == pOtherBuffer->e_FlushBuffer(usMoveSize))
			{
				bRet = true;
			}
		}
	}
	END_DEBUG_INFO
	return bRet;
}

/// ��ȡ��ǰʹ�õĻ����С
UINT CNetBuffer::e_GetUsed()
{
	START_DEBUG_INFO
	END_DEBUG_INFO
	return m_unUsed;	
}

/// ���ӵ�ǰʹ�õĻ����С
UINT CNetBuffer::e_AddUsed(UINT unAddUsed)
{
	START_DEBUG_INFO
	m_unUsed += unAddUsed;
	END_DEBUG_INFO
	return m_unUsed;	
}

/// ���û������к�
void CNetBuffer::e_SetBufferSeqNum(ULONG ulBuffSeqNum)
{
	START_DEBUG_INFO
	m_ulBuffSeqNum = ulBuffSeqNum;
	END_DEBUG_INFO
}

/// ��ȡ�������к�
ULONG CNetBuffer::e_GetBufferSeqNum()
{
	START_DEBUG_INFO
	END_DEBUG_INFO
	return m_ulBuffSeqNum;
}

/// ���û���ID
void CNetBuffer::e_SetBufferID(ULONG ulBufferID)
{
	START_DEBUG_INFO
	m_ulBufferID = ulBufferID;
	END_DEBUG_INFO
}

/// ��ȡ����ID
ULONG CNetBuffer::e_GetBufferID()
{
	START_DEBUG_INFO
	END_DEBUG_INFO
	return m_ulBufferID;
}

/// ��ȡWSABuffer
WSABUF*	CNetBuffer::e_GetWSABuffer()
{
	START_DEBUG_INFO
	/// ��ȡ��������
	USHORT usIOType = e_GetOperation();
	if(IOPT_TCPRECV == usIOType || IOPT_TRECVED == usIOType || IOPT_UDPRECV == usIOType)
	{
		/// ���ÿɶ��ڴ����ݣ����͵����ݣ�
		m_sttWsaBuf.buf = reinterpret_cast<char*>(m_Buffer) + m_unUsed;
		m_sttWsaBuf.len = MAX_NET_BUFFER_SIZE - m_unUsed;
	}
	else if(IOPT_TCPSEND == usIOType || IOPT_TSENDED == usIOType || IOPT_UDPSEND == usIOType)
	{
		/// ���ÿ�д�ڴ����ݣ����͵����ݣ�
		m_sttWsaBuf.buf = reinterpret_cast<char*>(m_Buffer);
		m_sttWsaBuf.len = m_unUsed;
	}
	else
	{
		m_sttWsaBuf.buf = reinterpret_cast<char*>(m_Buffer);
		m_sttWsaBuf.len = 0;
	}
	END_DEBUG_INFO
	return &m_sttWsaBuf;
}

void CNetBuffer::e_SetOperation(USHORT usIOType)
{
	START_DEBUG_INFO
	/// ����ص�����
	memset(&m_sttIOOverLapped, 0x00, sizeof(OVERLAPPED));
	/// ���ò�������
	m_usIOProType = usIOType;
	END_DEBUG_INFO
}

USHORT CNetBuffer::e_GetOperation()
{
	START_DEBUG_INFO
	END_DEBUG_INFO
	/// ���ص�ǰ��������
	return m_usIOProType;
}

void CNetBuffer::e_ReBuffValue()
{
	START_DEBUG_INFO
	/// ������������
	memset(m_Buffer, 0x00, e_GetUsed());
	m_unUsed = 0;
	memset(&m_sttWsaBuf, 0x00, sizeof(WSABUF));
	END_DEBUG_INFO
}

/// ���û���
void CNetBuffer::e_ReSetBuffer()
{
	START_DEBUG_INFO
	/// �������ݱ���
	m_ulBufferID = 0;
	m_ulBuffSeqNum = 0;
	memset(m_Buffer, 0x00, e_GetUsed());
	m_unUsed = 0;
	m_usIOProType = IOPT_None;
	memset(&m_sttWsaBuf, 0x00, sizeof(WSABUF));
	memset(&m_sttIOOverLapped, 0x00, sizeof(OVERLAPPED));
	memset(&m_sttUDPAddrInfo, 0x00, sizeof(SOCKADDR_IN));
	END_DEBUG_INFO
}