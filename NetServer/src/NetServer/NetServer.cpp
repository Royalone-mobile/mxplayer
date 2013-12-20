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
#include "NetCore.h"

/// �����������
CNetIOCP* m_pCNetServer = NULL;

CNetCore::CNetCore()
{
	// ��ʼ���������
	m_pCNetServer = new CNetIOCP();
}

CNetCore::~CNetCore()
{
	// �ͷ����ݶ���
	if(NULL != m_pCNetServer)
	{
		delete m_pCNetServer;
		m_pCNetServer = NULL;
	}
}

bool CNetCore::e_IStartServer(T_NetInitStruct* psttNetInitStruct,
	OnRecvDataCallBack pFunRecvData, OnSendDataCallBack pFunSendData, 
	OnConectionCallBack pFunConection, OnDisConectionCallBack pFunDisConection,
	OnPrintLog pFunPrintLog)
{
	START_DEBUG_INFO
	/// ��ʼ������ģ��
	if(true == m_pCNetServer->e_InitNetModel(*psttNetInitStruct, pFunRecvData,
		pFunSendData, pFunConection, pFunDisConection, pFunPrintLog))
	{
		END_DEBUG_INFO
		return m_pCNetServer->e_StartNetService();
	}
	END_DEBUG_INFO
	return false;
}

bool CNetCore::e_IStopdServer()
{
	START_DEBUG_INFO
	END_DEBUG_INFO
	return m_pCNetServer->e_StopdNetService();
}

bool CNetCore::e_ITCPSendData(ULONG ulContextID, BYTE* pSendData, int nDataLen)
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
	return m_pCNetServer->e_SendDataToTCPClient(ulContextID, pSendData, nDataLen);
}

bool CNetCore::e_IUDPSendData(const char* pszClientIP, USHORT usClientPort, BYTE* pSendData, int nDataLen)
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
	return m_pCNetServer->e_SendDataToUDPClient(pszClientIP, usClientPort, pSendData, nDataLen);
}

bool CNetCore::e_ICloseTCPContext(ULONG ulContextID)
{
	START_DEBUG_INFO
	/// �ر�TCP����
	return m_pCNetServer->e_CloseTCPContext(ulContextID);
	END_DEBUG_INFO
}