/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 
/// @par ģ����
/// IOCP����ģ��
/// @par ����ļ�
/// NetIOCP.cpp
/// @par ������ϸ����
/// @par ���̰߳�ȫ��
/// [��˵��]
/// @par �쳣ʱ��ȫ��
/// [��˵��]
/// @note         -
/// @file         NetIOCP.h
/// @brief        -
/// @author       Li.xl
/// @version      1.0
/// @date         2013/06/08
/// @todo         -
/// 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "NetIOCP.h"
#include "NetServer.h"

#define e_GetNetServer	((CNetIOCP* )(m_pCNetProcess))

CNetServer::CNetServer()
{
	// ��ʼ���������
	m_pCNetProcess = new CNetIOCP();
}

CNetServer::~CNetServer()
{
	// �ͷ����ݶ���
	if(NULL != e_GetNetServer)
	{
		delete e_GetNetServer;
		m_pCNetProcess = NULL;
	}
}

bool CNetServer::e_IStartServer(T_NetInitStruct* psttNetInitStruct,
	OnRecvDataCallBack pFunRecvData, OnSendDataCallBack pFunSendData, 
	OnConectionCallBack pFunConection, OnDisConectionCallBack pFunDisConection,
	OnPrintLog pFunPrintLog)
{
	START_DEBUG_INFO
	/// ��ʼ������ģ��
	if(true == e_GetNetServer->e_InitNetModel(*psttNetInitStruct, pFunRecvData,
		pFunSendData, pFunConection, pFunDisConection, pFunPrintLog))
	{
		END_DEBUG_INFO
		return e_GetNetServer->e_StartNetService();
	}
	END_DEBUG_INFO
	return false;
}

bool CNetServer::e_IStopdServer()
{
	START_DEBUG_INFO
	END_DEBUG_INFO
	return e_GetNetServer->e_StopdNetService();
}

bool CNetServer::e_ITCPSendStringData(ULONG ulContextID, char* pszSendData, int nDataLen)
{
	START_DEBUG_INFO
	/// ��֤���ݺϷ���
	if(NULL == pszSendData || 0 >= nDataLen)
	{
		END_DEBUG_INFO
		return false;
	}

	T_TCPBufferHead sttBufferHead;
	memset(&sttBufferHead, 0x00, sizeof(T_TCPBufferHead));
	/// ��ֵ����
	sttBufferHead.nTotalLen = sizeof(T_TCPBufferHead) + nDataLen\
		- sizeof(sttBufferHead.nTotalLen);
	sttBufferHead.sDataType = JDT_StringData;
	sttBufferHead.nOBJSize = nDataLen;
	sttBufferHead.sOBJCount = 1;
	/// ��������
	END_DEBUG_INFO
	return e_GetNetServer->e_SendDataToTCPClient(ulContextID, 
	sttBufferHead, (BYTE* )pszSendData, nDataLen);
}

bool CNetServer::e_ITCPSendBinaryData(ULONG ulContextID, UINT unSSDType, 
	UINT unOBJSize, USHORT usOBJCount, BYTE* pSendData)
{
	START_DEBUG_INFO
	/// ��ȡ���ݳ���
	int nDataLen = unOBJSize * usOBJCount;
	/// ��֤���ݺϷ���
	if(NULL == pSendData || 0 >= nDataLen)
	{
		END_DEBUG_INFO
		return false;
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
	END_DEBUG_INFO
	return e_GetNetServer->e_SendDataToTCPClient(ulContextID,
	sttBufferHead, pSendData, nDataLen);
}

bool CNetServer::e_IUDPSendStringData(const char* pszClientIP, 
	USHORT usClientPort, char* pszSendData, int nDataLen)
{
	START_DEBUG_INFO
	/// ��֤���ݺϷ���
	if(NULL == pszClientIP || 0 >= usClientPort ||
		NULL == pszSendData || 0 >= nDataLen)
	{
		END_DEBUG_INFO
		return false;
	}

	T_UDPBufferHead sttBufferHead;
	memset(&sttBufferHead, 0x00, sizeof(T_UDPBufferHead));
	/// ��ֵ����
	sttBufferHead.sDataType = JDT_StringData;
	sttBufferHead.nOBJSize = nDataLen;
	sttBufferHead.sOBJCount = 1;
	/// ��������
	END_DEBUG_INFO
	return e_GetNetServer->e_SendDataToUDPClient(pszClientIP,
	usClientPort, sttBufferHead, (BYTE* )pszSendData, nDataLen);
}

bool CNetServer::e_IUDPSendBinaryData(const char* pszClientIP, USHORT usClientPort, 
	UINT unSSDType, UINT unOBJSize, USHORT usOBJCount, BYTE* pSendData)
{
	START_DEBUG_INFO
	/// ��ȡ���ݳ���
	int nDataLen = unOBJSize * usOBJCount;
	/// ��֤���ݺϷ���
	if(NULL == pszClientIP || 0 >= usClientPort ||
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
	END_DEBUG_INFO
	return e_GetNetServer->e_SendDataToUDPClient(pszClientIP,
		usClientPort, sttBufferHead, pSendData, nDataLen);
}

bool CNetServer::e_ICloseTCPContext(ULONG ulContextID)
{
	START_DEBUG_INFO
	/// �ر�TCP����
	return e_GetNetServer->e_CloseTCPContext(ulContextID);
	END_DEBUG_INFO
}