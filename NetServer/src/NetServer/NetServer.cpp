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

bool CNetServer::e_ITCPSendData(ULONG ulContextID, BYTE* pSendData, int nDataLen)
{
	START_DEBUG_INFO
	/// ��֤���ݺϷ���
	if(NULL == pSendData || 0 >= nDataLen)
	{
		END_DEBUG_INFO
		return false;
	}
	/// ��������
	END_DEBUG_INFO
	return e_GetNetServer->e_SendDataToTCPClient(ulContextID, pSendData, nDataLen);
}

/*bool CNetServer::e_ITCPSendStringData(ULONG ulContextID, BYTE* pSendData, int nDataLen)
{
	START_DEBUG_INFO
	/// ��֤���ݺϷ���
	if(NULL == pSendData || 0 >= nDataLen)
	{
		END_DEBUG_INFO
		return false;
	}
	/// ��������
	END_DEBUG_INFO
	return e_GetNetServer->e_SendDataToTCPClient(ulContextID, pSendData, nDataLen);
}

bool CNetServer::e_ITCPSendBData(ULONG ulContextID, BYTE* pSendData, int nDataLen)
{
	START_DEBUG_INFO
	/// ��֤���ݺϷ���
	if(NULL == pSendData || 0 >= nDataLen)
	{
		END_DEBUG_INFO
		return false;
	}
	/// ��������
	END_DEBUG_INFO
	return e_GetNetServer->e_SendDataToTCPClient(ulContextID, pSendData, nDataLen);
}
*/

bool CNetServer::e_IUDPSendData(const char* pszClientIP, USHORT usClientPort, BYTE* pSendData, int nDataLen)
{
	START_DEBUG_INFO
	/// ��֤���ݺϷ���
	if(NULL == pszClientIP || 0 >= usClientPort || NULL == pSendData || 0 >= nDataLen)
	{
		END_DEBUG_INFO
		return false;
	}
	/// ��������
	END_DEBUG_INFO
	return e_GetNetServer->e_SendDataToUDPClient(pszClientIP, usClientPort, pSendData, nDataLen);
}

bool CNetServer::e_ICloseTCPContext(ULONG ulContextID)
{
	START_DEBUG_INFO
	/// �ر�TCP����
	return e_GetNetServer->e_CloseTCPContext(ulContextID);
	END_DEBUG_INFO
}