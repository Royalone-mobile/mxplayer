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

/// ����ͨѶ���Ӷ���
CNetTreatment m_CNetTreatment;

CNetClient::CNetClient()
{
}

CNetClient::~CNetClient()
{
}

int CNetClient::e_ConnectionServer(UINT nServerPort, int nClientType, 
	OnRecvDataCallBack pReceiveDataCallBack, UINT nConnectType, char* pszServerIp)
{
	START_DEBUG_INFO

	/// ����ص����������ڣ��򷵻�
	if(NULL == pReceiveDataCallBack)
	{
		END_DEBUG_INFO
		return 0;
	}

	/// ��������ļ���������IP����ָ�����õ�IP
	m_CNetTreatment.e_ConnectServer(szServiceIp, nServerPort, nClientType, 2);		
	/// ���ûص�����
	m_CNetTreatment.e_SetReceiveDataCallBack(pReceiveDataCallBack);
	/// ���������߳�
	m_CNetTreatment.e_StartSend();
	/// ���������߳�
	m_CNetTreatment.e_StartReceive();

	END_DEBUG_INFO
	return 1;
}

int CNetClient::e_DisconnectServer()
{
	START_DEBUG_INFO
	/// �Ͽ������߳�
	m_CNetTreatment.e_StopReceive();
	/// �Ͽ������߳�
	m_CNetTreatment.e_StopSend();
	END_DEBUG_INFO
	return 1;
}

int CNetClient::e_SendDataByStruct(UINT nSSDType, UINT nStructSize, int nCount, void* pData)
{
	START_DEBUG_INFO
	/// �������ͽ�����ʱ����
	T_RSStructData* pRecvClient = NULL;
	/// ��ȡ�ṹ�����ݴ�С
	long lAllStructlen = 0;
	/// ��ȡ���ݲ�������
	UINT nJobDataType = JDT_StructData;
	if(nCount > 0)
	{
		lAllStructlen = nStructSize * nCount;
		if(nCount > 1)
		{
			nJobDataType = JDT_ArrStructData;
		}
	}
	/// ��ȡ���ݰ���С
	long lAllSize = 0;
	if(nCount > 0)
	{
		/// �������������
		lAllSize = sizeof(T_RSStructData) + lAllStructlen - 1;
	}
	else
	{
		/// ���û����������
		lAllSize = sizeof(T_RSStructData);
	}
	/// ��ȡ��������
	pRecvClient = (T_RSStructData*)new char[lAllSize];
	/// ��ֵ���ݰ�
	pRecvClient->nClientType = m_CNetTreatment.m_nClientType;
	pRecvClient->nJobDataType = nJobDataType;
	pRecvClient->nSRStructType = nSSDType;
	pRecvClient->nDataSize = nStructSize;
	pRecvClient->nDataCount = nCount;
	pRecvClient->nErrorCode = ECT_None;
	/// ��ֵ����
	if(0 < lAllStructlen)
	{
		memcpy(pRecvClient->szData, pData, lAllStructlen);
	}
	/// ��������
	m_CNetTreatment.m_tSendTime = time(NULL);
	if(false == m_CNetTreatment.e_SendStructData(nJobDataType, nSSDType, lAllSize, pRecvClient))
	{
		m_CNetTreatment.m_tSendTime = 0;
		END_DEBUG_INFO
		return 0;
	}
	END_DEBUG_INFO
	return 1;
}