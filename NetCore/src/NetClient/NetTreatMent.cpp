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
	memset(m_szRecvBuffer, 0x00, MAX_NET_BUFFER_SIZE);
}

CNetTreatment::~CNetTreatment()
{
}

void CNetTreatment::e_SendLoop()
{
	/// �����̴߳���������
	/*while(true)
	{
		/// ��֤���ӺϷ���
		if(0 >= (int)m_SendBufferArray.size())
		{
			Sleep(1);
			continue;
		}

		/// �ڻ����в���Ҫ���͵�����
		T_BufferStruct* pBuffer = NULL;
		if(0 < (int)m_SendBufferArray.size())
		{
			CAutoLock lckSendBufferLock(&m_SendBufferLock);
			pBuffer = (T_BufferStruct* )*m_SendBufferArray.begin();
			if(NULL != pBuffer)
			{
				m_SendBufferArray.erase(m_SendBufferArray.begin());
			}
		}

		/// ������ݴ�������
		if(NULL != pBuffer)
		{
			e_SendStruct(m_nSocketId, pBuffer->nJobType, pBuffer->nSST, pBuffer->nSize, pBuffer->pBuf);

			if(pBuffer->pBuf)
			{
				delete pBuffer->pBuf;
				pBuffer->pBuf = NULL;
			}
			delete pBuffer;
			pBuffer = NULL;
		}
	}*/
}

void CNetTreatment::e_ReceiveLoop()
{
	BYTE* pBuffer = NULL;
	BYTE* pBufferTmp = NULL;

	/// �������ͣ�������1���ֽڴ�С
	BYTE BtDataType = 0;

	while(true)
	{
		/// ����TCP�������ݰ�
		i_ProcessTCPRecData();
		/// ����UDP�������ݰ�
		i_ProcessUDPRecData();
	}
}

void CNetTreatment::i_ProcessTCPRecData()
{
	/// ���TCP�Ƿ�����
	if(0 >= e_IsConnect())
	{
		/// ��������
		e_ReconnectServer();
		return;
	}

	/// Ҫ��ȡ�����ݳ���
	int nDataLen = 0;
	/// �Ѷ�ȡ�����ݳ���
	int nReadCount = 0;
	/// ��ȡ���ݳ���
	nReadCount = e_RecTCPData((char* )&nDataLen, sizeof(UINT));

	/// û�ж�ȡ������
	if(-1 == nReadCount)
	{
		printf("NetClient Read TCP Data Len Error, ReadCount = %d, DataLen = %d .\r\n",
			nReadCount, nDataLen);
		/// �Ͽ�����
		e_CloseTCPSocket();
		return;
	}

	/// ��ȷ��ȡ�������ݳ���
	if(0 < nDataLen)
	{
		/// ��ȥ�Ѿ�������sizeof(DataType)��С��������������
		nReadCount = e_RecTCPData(m_szRecvBuffer, nDataLen);
		/// �����ȡ�����ݲ���ȷ����ر�����
		if(nReadCount != nDataLen)
		{
			printf("NetClient Read TCP Data Content Error, ReadCount = %d, DataLen = %d .\r\n",
				nReadCount, nDataLen);
			/// �Ͽ��׽�������
			e_CloseTCPSocket();
			return;
		}
		
		T_TCPBufferHead sttBufferHead;
		memset(&sttBufferHead, 0x00, sizeof(T_TCPBufferHead));

		mem
	}
}

void CNetTreatment::i_ProcessUDPRecData()
{
	/// ���TCP�Ƿ�����
	if(0 >= e_IsConnect())
	{
		/// ��������
		e_ReconnectServer();
		return;
	}
	/// �Ѷ�ȡ�����ݳ���
	int nReadCount = 0;

	string strRemoteIP = "";
	USHORT usRemotePort = 0;

	/// ��ȡ���ݳ���
	nReadCount = e_RecUDPData((char* )&m_szRecvBuffer, MAX_PAL_BUFFER_SIZE - 1,
		strRemoteIP, usRemotePort);
	if(0 < nReadCount)
	{

	}
}

void CNetTreatment::i_ProcessReceivePackage(USHORT usNetType, const char* pszClientIP, 
	USHORT usClientPort, SHORT sDataType, int nOBJType, SHORT sOBJCount, 
	SHORT sSNum, SHORT sENum, int nDatalen, void* pData)
{
	/// ������յ��Ľṹ��
	T_RSStructData* psttRSStructData = (T_RSStructData* )pData;

	if(NULL == psttRSStructData)
	{
		printf("Get The T_RSStructData Error.\r\n");
		return;
	}

	/// ������ڻص�����
	if(NULL != m_pReceiveStructCallBack)
	{
		/// �������ݸ��ص�����
		m_pReceiveStructCallBack(psttRSStructData->nSRStructType,
		psttRSStructData->nDataCount, psttRSStructData->szData);
	}
}