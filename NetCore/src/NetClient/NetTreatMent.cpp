////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 
/// @par ģ����
/// �ͻ�������ͨѶҵ����
/// @par ����ļ�
/// NetTreatment.h
/// @par ������ϸ����
/// @par ���̰߳�ȫ��
/// [��˵��]
/// @par �쳣ʱ��ȫ��
/// [��˵��]
/// @note         -
/// @file         NetTreatMent.cpp
/// @brief        -
/// @author       Li.xl
/// @version      1.0
/// @date         2013/12/20
/// @todo         -
/// 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "NetTreatMent.h"

CNetTreatment::CNetTreatment()
{
	/// ��ʼ���������ݻ���
	memset(m_szTCPRecvBuffer, 0x00, MAX_NET_BUFFER_SIZE);
	memset(m_szUDPRecvBuffer, 0x00, MAX_NET_BUFFER_SIZE);
}

CNetTreatment::~CNetTreatment()
{
}

void CNetTreatment::e_StopTCPProcess()
{
	/// ֹͣTCP�߳�
	e_StopTCPReceive();
	e_StopTCPSend();
	/// �ͷ��׽���
	e_CloseTCPSocket();
}

void CNetTreatment::e_StopUDPProcess()
{
	/// ֹͣUDP�߳�
	e_StopUDPReceive();
	e_StopUDPSend();
	/// �ͷ��׽���
	e_CloseUDPSocket();
}

void CNetTreatment::e_SendTCPLoop()
{
	START_DEBUG_INFO
	/// �����¼�����ֵ
	int nWaitRet = 0;
	T_TCPSendBuffer* psttSendBuffer = NULL;
	/// ����TCP��������
	while(true)
	{
		nWaitRet = WaitForSingleObject(m_HSendTCPStopEven, 5);
		/// ֹͣ�߳��źţ��˳��߳�
		if(WAIT_OBJECT_0 == nWaitRet)
		{
			break;
		}
		/// �ڻ����в���Ҫ���͵�����
		psttSendBuffer = NULL;
		m_SendTCPBufferLock.e_Lock();
		if(0 < (int)m_SendTCPBufferArray.size())
		{			
			psttSendBuffer = (T_TCPSendBuffer* )*m_SendTCPBufferArray.begin();
			if(NULL != psttSendBuffer)
			{
				m_SendTCPBufferArray.erase(m_SendTCPBufferArray.begin());
			}
		}
		m_SendTCPBufferLock.e_Unlock();

		/// ���ݺϷ���������
		if(NULL != psttSendBuffer)
		{			
			/// ���������Ƿ�Ϸ�
			if(NULL != psttSendBuffer->pszBuffer)
			{
				/// ��������
				e_SendTCPData(psttSendBuffer->pszBuffer, psttSendBuffer->nBufferSize);
				/// �ͷ���������
				delete psttSendBuffer->pszBuffer;
				psttSendBuffer->pszBuffer = NULL;
			}
			delete psttSendBuffer;
			psttSendBuffer = NULL;
		}
	}
	END_DEBUG_INFO;
}

void CNetTreatment::e_ReceiveTCPLoop()
{
	START_DEBUG_INFO
	/// �����¼�����ֵ
	int nWaitRet = 0;
	/// ����TCP��������
	while(true)
	{
		nWaitRet = WaitForSingleObject(m_HRecvTCPStopEven, 5);
		/// ֹͣ�߳��źţ��˳��߳�
		if(WAIT_OBJECT_0 == nWaitRet)
		{
			break;
		}
		/// ����TCP�������ݰ�
		i_ProcessTCPRecData();
	}
	END_DEBUG_INFO
}

void CNetTreatment::e_SendUDPLoop()
{
	START_DEBUG_INFO
	/// �����¼�����ֵ
	int nWaitRet = 0;
	T_UDPSendBuffer* psttSendBuffer = NULL;
	/// ����TCP��������
	while(true)
	{
		nWaitRet = WaitForSingleObject(m_HSendUDPStopEven, 5);
		/// ֹͣ�߳��źţ��˳��߳�
		if(WAIT_OBJECT_0 == nWaitRet)
		{
			break;
		}
		/// �ڻ����в���Ҫ���͵�����
		psttSendBuffer = NULL;
		m_SendUDPBufferLock.e_Lock();
		if(0 < (int)m_SendUDPBufferArray.size())
		{			
			psttSendBuffer = (T_UDPSendBuffer* )*m_SendUDPBufferArray.begin();
			if(NULL != psttSendBuffer)
			{
				m_SendUDPBufferArray.erase(m_SendUDPBufferArray.begin());
			}
		}
		m_SendUDPBufferLock.e_Unlock();

		/// ���ݺϷ���������
		if(NULL != psttSendBuffer)
		{			
			/// ���������Ƿ�Ϸ�
			if(NULL != psttSendBuffer->pszBuffer)
			{
				/// ��������
				e_SendUDPData(psttSendBuffer->szRemoteIP, psttSendBuffer->szRemotePort, 
					psttSendBuffer->pszBuffer, psttSendBuffer->nBufferSize);
				/// �ͷ���������
				delete psttSendBuffer->pszBuffer;
				psttSendBuffer->pszBuffer = NULL;
			}
			delete psttSendBuffer;
			psttSendBuffer = NULL;
		}
	}
	END_DEBUG_INFO
}

void CNetTreatment::e_ReceiveUDPLoop()
{
	/// �����¼�����ֵ
	int nWaitRet = 0;
	/// ����UDP��������
	while(true)
	{
		nWaitRet = WaitForSingleObject(m_HRecvUDPStopEven, 5);
		/// ֹͣ�߳��źţ��˳��߳�
		if(WAIT_OBJECT_0 == nWaitRet)
		{
			break;
		}
		/// ����UDP�������ݰ�
		i_ProcessUDPRecData();
	}
}

void CNetTreatment::i_ProcessTCPRecData()
{
	START_DEBUG_INFO
	/// ���TCP�Ƿ�����
	if(false == e_IsTCPConnect())
	{
		/// ��������
		e_ReconnectServer();
		END_DEBUG_INFO
		return;
	}

	/// Ҫ��ȡ�����ݳ���
	int nDataLen = 0;
	/// �Ѷ�ȡ�����ݳ���
	int nReadCount = 0;
	/// ��ȡ���ݳ���
	nReadCount = e_RecTCPData((char* )&nDataLen, sizeof(UINT));
	/// û�ж�ȡ�����ݣ�������Ч���ȴ���
	if(-1 == nReadCount || nDataLen < sizeof(T_TCPBufferHead) - sizeof(INT) ||
		nDataLen > MAX_PAL_BUFFER_SIZE)
	{
		printf("NetClient Read TCP Data Len Error, ReadCount = %d, DataLen = %d .\r\n",
			nReadCount, nDataLen);
		/// �Ͽ�����
		e_CloseTCPSocket();
		END_DEBUG_INFO
		return;
	}

	/// ��ȷ��ȡ�������ݳ���
	if(0 < nDataLen)
	{
		/// ���ý������ݱ���
		memset(m_szTCPRecvBuffer, 0x00, sizeof(MAX_NET_BUFFER_SIZE));
		/// �������ݳ���
		memcpy(m_szTCPRecvBuffer, &nDataLen, sizeof(INT));
		/// �Ѿ�������sizeof(nDataLen)��С��������������
		char* pszRecvBuffer = m_szTCPRecvBuffer + sizeof(INT);
		/// ������������
		nReadCount = e_RecTCPData(pszRecvBuffer, nDataLen);
		/// �����ȡ�����ݲ���ȷ����ر�����
		if(nReadCount != nDataLen)
		{
			printf("NetClient Read TCP Data Content Error, ReadCount = %d, DataLen = %d .\r\n",
				nReadCount, nDataLen);
			/// �Ͽ��׽�������
			e_CloseTCPSocket();
			END_DEBUG_INFO
			return;
		}
		
		/// = �յ����������ݰ�
		/// = ��ȡ����ͷ
		T_TCPBufferHead sttBufferHead;
		memset(&sttBufferHead, 0x00, sizeof(T_TCPBufferHead));
		memcpy(&sttBufferHead, m_szTCPRecvBuffer, sizeof(T_TCPBufferHead));
		/// = ��ȡ���ݲ���
		int nDataSize = nDataLen - sizeof(T_TCPBufferHead);
		memmove(m_szTCPRecvBuffer, m_szTCPRecvBuffer + sizeof(T_TCPBufferHead), nDataSize);
		memset(m_szTCPRecvBuffer + nDataLen, 0x00, sizeof(T_TCPBufferHead));
		/// �����յ��������ݰ�
		i_ProcessReceivePackage(NTT_TCPData, m_sttInitNetClient.szTCPServerIP,
			m_sttInitNetClient.usTCPServerPort, sttBufferHead.sDataType,
			sttBufferHead.nOBJType, sttBufferHead.sOBJCount, sttBufferHead.sSNum, 
			sttBufferHead.sENum, nDataSize, m_szTCPRecvBuffer);
	}
	END_DEBUG_INFO
}

void CNetTreatment::i_ProcessUDPRecData()
{
	START_DEBUG_INFO
	if(false == e_IsUDPCreated())
	{
		END_DEBUG_INFO
		return;
	}
	/// Զ��IP�Ͷ˿ڶ���
	string strRemoteIP = "";
	USHORT usRemotePort = 0;
	/// �Ѷ�ȡ�����ݳ���
	int nReadCount = 0;
	/// ��ʼ�����ջ���
	memset(m_szUDPRecvBuffer, 0x00, sizeof(MAX_NET_BUFFER_SIZE));
	/// ��ȡ���ݳ���
	nReadCount = e_RecUDPData((char* )&m_szUDPRecvBuffer, MAX_NET_BUFFER_SIZE,
		strRemoteIP, usRemotePort);
	/// ��֤���ݺϷ���
	if(0 >= nReadCount || nReadCount < sizeof(T_UDPBufferHead) ||
		nReadCount > MAX_NET_BUFFER_SIZE)
	{
		printf("NetClient Read UDP Data Content Error, ReadCount = %d.\r\n",
			nReadCount);
		END_DEBUG_INFO
		return;
	}

	/// = �յ����������ݰ�
	/// = ��ȡ����ͷ
	T_UDPBufferHead sttBufferHead;
	memset(&sttBufferHead, 0x00, sizeof(T_UDPBufferHead));
	memcpy(&sttBufferHead, m_szUDPRecvBuffer, sizeof(T_UDPBufferHead));
	/// = ��ȡ���ݲ���
	int nDataSize = nReadCount - sizeof(T_UDPBufferHead);
	memmove(m_szUDPRecvBuffer, m_szUDPRecvBuffer + sizeof(T_UDPBufferHead), nDataSize);
	memset(m_szUDPRecvBuffer + nReadCount, 0x00, sizeof(T_UDPBufferHead));
	/// �����յ��������ݰ�
	i_ProcessReceivePackage(NTT_UDPData, strRemoteIP.c_str(), usRemotePort,
		sttBufferHead.sDataType, sttBufferHead.nOBJType, sttBufferHead.sOBJCount,
		sttBufferHead.sSNum, sttBufferHead.sENum, nDataSize, m_szUDPRecvBuffer);
	END_DEBUG_INFO
}

bool CNetTreatment::i_ProcessReceivePackage(USHORT usNetType, const char* pszClientIP, 
	USHORT usClientPort, SHORT sDataType, int nOBJType, SHORT sOBJCount, 
	SHORT sSNum, SHORT sENum, int nDatalen, void* pData)
{
	START_DEBUG_INFO
	/// ���巵��ֵ
	bool bRet = false;
	/// ��֤���ݺϷ���
	if(NULL == pszClientIP || 0 >= usClientPort 
		|| 0 >= nDatalen || NULL == pData)
	{
		printf("i_ProcessReceivePackage Get The Data Error.\r\n");
		END_DEBUG_INFO
		return bRet;
	}

	/// ������ڻص�����
	if(NULL != m_pfnRecDataCallBack)
	{
		/// �������ݸ��ص�����
		m_pfnRecDataCallBack(usNetType, pszClientIP, usClientPort, sDataType,
			nOBJType, sOBJCount, sSNum, sENum, nDatalen, pData);
		bRet = true;
	}
	END_DEBUG_INFO
	return bRet;
}

bool CNetTreatment::e_CreatSendTCPData(T_TCPBufferHead& sttBufferHead,
	BYTE* pSendData, int nDataLen)
{
	START_DEBUG_INFO
	/// ���巵��ֵ
	bool bRet = false;
	/// ��֤���ݺϷ���, ���δ���ӷ���������������
	if(NULL != pSendData || 0 >= nDataLen || nDataLen > \
		MAX_NET_BUFFER_SIZE - sizeof(T_TCPBufferHead) || 
		false == e_IsTCPConnect())
	{
		END_DEBUG_INFO
		return bRet;
	}
	/// Ҫ���͵����ݳ���
	int nSendLen = sizeof(T_TCPBufferHead) + nDataLen;
	/// ����TCP���ͽṹ
	T_TCPSendBuffer* psttSendBuffer = new T_TCPSendBuffer();
	if(NULL == psttSendBuffer)
	{
		END_DEBUG_INFO
		return bRet;
	}
	psttSendBuffer->nBufferSize = nSendLen;
	psttSendBuffer->pszBuffer = new char[nSendLen + EXTRA_BUFFER_SIZE];
	if(NULL == psttSendBuffer->pszBuffer)
	{
		delete psttSendBuffer;
		psttSendBuffer = NULL;
		END_DEBUG_INFO
		return bRet;
	}

	/// ��������
	memcpy(psttSendBuffer->pszBuffer, &sttBufferHead, sizeof(T_TCPBufferHead));
	memcpy(psttSendBuffer->pszBuffer + sizeof(T_TCPBufferHead), pSendData, nDataLen);
	/// ���뷢�����ݵ��б�
	CAutoLock AutoLock(&m_SendTCPBufferLock);
	m_SendTCPBufferArray.push_back(psttSendBuffer);
	bRet = true;
	END_DEBUG_INFO
	return bRet;
}

bool CNetTreatment::e_CreatSendUDPData(const char* pszRemoteIP, USHORT usRemotePort,
	T_UDPBufferHead& sttBufferHead, BYTE* pSendData, int nDataLen)
{
	START_DEBUG_INFO
	/// ���巵��ֵ
	bool bRet = false;
	/// ��֤���ݺϷ���, ���UDP�ͻ���δ�����������ͻ���
	if(NULL != pszRemoteIP || 0 >= usRemotePort || 
		NULL != pSendData || 0 >= nDataLen || 
		false == e_IsUDPCreated())
	{
		END_DEBUG_INFO
		return bRet;
	}

	/// Ҫ���͵����ݳ���
	int nSendLen = sizeof(T_UDPBufferHead) + nDataLen;
	/// ����TCP���ͽṹ
	T_UDPSendBuffer* psttSendBuffer = new T_UDPSendBuffer();
	if(NULL == psttSendBuffer)
	{
		END_DEBUG_INFO
		return bRet;
	}
	strncpy_s(psttSendBuffer->szRemoteIP, pszRemoteIP, sizeof(psttSendBuffer->szRemoteIP) - 1);
	psttSendBuffer->szRemotePort = usRemotePort;
	psttSendBuffer->nBufferSize = nSendLen;
	psttSendBuffer->pszBuffer = new char[nSendLen + EXTRA_BUFFER_SIZE];
	if(NULL == psttSendBuffer->pszBuffer)
	{
		delete psttSendBuffer;
		psttSendBuffer = NULL;
		END_DEBUG_INFO
		return bRet;
	}

	/// ��������
	memcpy(psttSendBuffer->pszBuffer, &sttBufferHead, sizeof(T_UDPBufferHead));
	memcpy(psttSendBuffer->pszBuffer + sizeof(T_UDPBufferHead), pSendData, nDataLen);
	/// ���뷢�����ݵ��б�
	CAutoLock AutoLock(&m_SendUDPBufferLock);
	m_SendUDPBufferArray.push_back(psttSendBuffer);
	bRet = true;
	END_DEBUG_INFO
	return bRet;
}