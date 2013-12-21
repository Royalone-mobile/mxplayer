////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 
/// @par ģ����
/// ����ӿں�������
/// @par ����ļ�
/// NetClient.h NetTreatMent.h
/// @par ������ϸ����
/// @par ���̰߳�ȫ��
/// [��˵��]
/// @par �쳣ʱ��ȫ��
/// [��˵��]
/// @note         -
/// @file         NetClient.cpp
/// @brief        -
/// @author       Li.xl
/// @version      1.0
/// @date         2013/12/20
/// @todo         -
/// 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "common/CommTypes.h"
#include "NetClient/NetClient.h"
#include "NetTreatMent.h"

#define e_GetNetClient	((CNetTreatment* )(m_pProNetClient))

CNetClient::CNetClient()
{
	m_pProNetClient = new CNetTreatment();
}

CNetClient::~CNetClient()
{
	// �ͷ����ݶ���
	if(NULL != e_GetNetClient)
	{
		delete e_GetNetClient;
		m_pProNetClient = NULL;
	}
}

int CNetClient::e_IInitNetClient(T_InitNetClient* psttInitNetClient, 
	OnRecvDataCallBack pfnRecvDataCallBack)
{
	START_DEBUG_INFO
	/// ���巵��ֵ
	int nRet = 0;
	/// ��ʼ������
	e_GetNetClient->e_InitNetClient(psttInitNetClient);
	e_GetNetClient->e_SetReceiveDataCallBack(pfnRecvDataCallBack);
	END_DEBUG_INFO
	return nRet;
}

int CNetClient::e_IConnectionServer()
{
	START_DEBUG_INFO
	/// ���巵��ֵ
	int nRet = 0;
	/// ���ӷ�����
	if(true == e_GetNetClient->e_ConnectServer())
	{
		nRet = 1;
	}
	END_DEBUG_INFO
	return nRet;
}

int CNetClient::e_IDisconnectServer()
{
	START_DEBUG_INFO
	/// ���巵��ֵ
	int nRet = 0;
	e_GetNetClient->e_StopTCPProcess();
	nRet = 1;
	END_DEBUG_INFO
	return nRet;
}

int CNetClient::e_ICreatUDPClient()
{
	START_DEBUG_INFO
	/// ���巵��ֵ
	int nRet = 0;
	/// ����UDP�ͻ���
	if(true == e_GetNetClient->e_CreatUDPClient())
	{
		nRet = 1;
	}
	END_DEBUG_INFO
	return nRet;
}

int CNetClient::e_IDestroyUDPClient()
{
	START_DEBUG_INFO
	/// ���巵��ֵ
	int nRet = 0;
	e_GetNetClient->e_StopUDPProcess();
	nRet = 1;
	END_DEBUG_INFO
	return nRet;
}

int CNetClient::e_ISendTCPStringData(char* pszSendData, UINT unDatalen)
{
	START_DEBUG_INFO
	/// ���巵��ֵ
	int nRet = 0;
	/// ��֤���ݺϷ���
	if(NULL == pszSendData || 0 >= unDatalen)
	{
		END_DEBUG_INFO
		return nRet;
	}

	T_TCPBufferHead sttBufferHead;
	memset(&sttBufferHead, 0x00, sizeof(T_TCPBufferHead));
	/// ��ֵ����
	sttBufferHead.nTotalLen = sizeof(T_TCPBufferHead) + unDatalen\
		- sizeof(sttBufferHead.nTotalLen);
	sttBufferHead.sDataType = JDT_StringData;
	sttBufferHead.nOBJSize = unDatalen;
	sttBufferHead.sOBJCount = 1;

	if(true == e_GetNetClient->e_CreatSendTCPData(sttBufferHead,
		(BYTE* )pszSendData, unDatalen))
	{
		nRet = 1;
	}
	/// ��������
	END_DEBUG_INFO
	return nRet;
}

int CNetClient::e_ISendTCPBinaryData(UINT unSSDType, UINT unOBJSize,
	USHORT usOBJCount, BYTE* pSendData)
{
	START_DEBUG_INFO
	/// ���巵��ֵ
	int nRet = 0;
	/// ��ȡ���ݳ���
	int nDataLen = unOBJSize * usOBJCount;
	/// ��֤���ݺϷ���
	if(NULL == pSendData || 0 >= nDataLen)
	{
		END_DEBUG_INFO
		return nRet;
	}

	T_TCPBufferHead sttBufferHead;
	memset(&sttBufferHead, 0x00, sizeof(T_TCPBufferHead));
	/// ��ֵ����
	sttBufferHead.nTotalLen = sizeof(T_TCPBufferHead) + nDataLen\
		- sizeof(sttBufferHead.nTotalLen);
	sttBufferHead.sDataType = JDT_StructData;
	sttBufferHead.nOBJType = unSSDType;
	sttBufferHead.nOBJSize = unOBJSize;
	sttBufferHead.sOBJCount = usOBJCount;
	sttBufferHead.sSNum = 1;
	sttBufferHead.sENum = usOBJCount;
	/// ��������
	if(true == e_GetNetClient->e_CreatSendTCPData(sttBufferHead,
		(BYTE* )pSendData, nDataLen))
	{
		nRet = 1;
	}
	END_DEBUG_INFO
	return nRet; 
}

int CNetClient::e_ISendUDPStringData(const char* pRemoteIP, USHORT usRemotePort, 
	char* pszSendData, UINT unDatalen)
{
	START_DEBUG_INFO
	/// ���巵��ֵ
	int nRet = 0;
	/// ��֤���ݺϷ���
	if(NULL == pRemoteIP || 0 >= usRemotePort ||
		NULL == pszSendData || 0 >= unDatalen)
	{
		END_DEBUG_INFO
		return nRet;
	}

	/// ����UDP��������ͷ
	T_UDPBufferHead sttBufferHead;
	memset(&sttBufferHead, 0x00, sizeof(T_UDPBufferHead));
	/// ��ֵ����
	sttBufferHead.sDataType = JDT_StringData;
	sttBufferHead.nOBJSize = unDatalen;
	sttBufferHead.sOBJCount = 1;
	/// ��������
	if(true == e_GetNetClient->e_CreatSendUDPData(pRemoteIP, usRemotePort, sttBufferHead,
		(BYTE* )pszSendData, unDatalen))
	{
		nRet = 1;
	}
	END_DEBUG_INFO
	return nRet;
}

int CNetClient::e_ISendUDPBinaryData(const char* pRemoteIP, USHORT usRemotePort,
	UINT unSSDType, UINT unOBJSize, USHORT usOBJCount, BYTE* pSendData)
{
	START_DEBUG_INFO
	/// ���巵��ֵ
	int nRet = 0;
	/// ��ȡ���ݳ���
	int nDataLen = unOBJSize * usOBJCount;
	/// ��֤���ݺϷ���
	if(NULL == pRemoteIP || 0 >= usRemotePort ||
		NULL == pSendData || 0 >= nDataLen)
	{
		END_DEBUG_INFO
		return false;
	}

	T_UDPBufferHead sttBufferHead;
	memset(&sttBufferHead, 0x00, sizeof(T_UDPBufferHead));
	/// ��ֵ����
	sttBufferHead.sDataType = JDT_StructData;
	sttBufferHead.nOBJType = unSSDType;
	sttBufferHead.nOBJSize = unOBJSize;
	sttBufferHead.sOBJCount = usOBJCount;
	sttBufferHead.sSNum = 1;
	sttBufferHead.sENum = usOBJCount;
	/// ��������
	if(true == e_GetNetClient->e_CreatSendUDPData(pRemoteIP,
		usRemotePort, sttBufferHead, pSendData, nDataLen))
	{
		nRet = 1;
	}
	END_DEBUG_INFO
	return nRet;
}