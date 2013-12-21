////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 
/// @par ģ����
/// FWAY�ͻ���ͨѶ����
/// @par ����ļ�
/// NetBase.h
/// @par ������ϸ����
/// @par ���̰߳�ȫ��
/// [��˵��]
/// @par �쳣ʱ��ȫ��
/// [��˵��]
/// @note         -
/// @file         NetBase.cpp
/// @brief        -
/// @author       Li.xl
/// @version      1.0
/// @date         2013/12/20
/// @todo         -
/// 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "NetBase.h"

CNetBase::CNetBase()
{
	START_DEBUG_INFO
	/// ��ʼ���׽���
	m_sClientTCPSocket = 0;
	m_sClientUDPSocket = 0;
	/// ��ʼ���ͻ���������Ϣ
	m_sttInitNetClient.e_InitStruct();
	/// ��ʼ���߳̾��
	m_HRecvTCPThreadHandle = NULL;
	m_HSendTCPThreadHandle = NULL;
	m_HRecvUDPThreadHandle = NULL;
	m_HSendUDPThreadHandle = NULL;
	/// ��ʼ���߳̽����¼�
	m_HRecvTCPStopEven = CreateEvent(NULL, FALSE, FALSE, NULL);
	m_HSendTCPStopEven = CreateEvent(NULL, FALSE, FALSE, NULL);
	m_HRecvUDPStopEven = CreateEvent(NULL, FALSE, FALSE, NULL);
	m_HSendUDPStopEven = CreateEvent(NULL, FALSE, FALSE, NULL);
	/// ��ʼ����������
	m_SendTCPBufferArray.clear();
	m_SendUDPBufferArray.clear();
	/// ��ʼ���ص�����
	m_pfnRecDataCallBack = NULL;
	/// ��ʼ�����ӷ�����״̬
	m_bIsTCPConnect = false;
	m_bIsUDPCreated = false;
	END_DEBUG_INFO
}

CNetBase::~CNetBase()
{
	START_DEBUG_INFO
	/// ֹͣ�����߳�
	e_StopTCPReceive();
	e_StopUDPReceive();
	/// ֹͣ�����߳�
	e_StopTCPSend();
	e_StopUDPSend();
	/// �����¼�
	i_CancelEvens();
	/// �ͷ��׽���
	e_CloseSocket();
	/// ��շ��͵Ļ�����������
	i_DestroyTCPSendBufferArray();
	i_DestroyUDPSendBufferArray();
	END_DEBUG_INFO
}

bool CNetBase::e_InitNetClient(T_InitNetClient* psttNetClient)
{
	START_DEBUG_INFO
	/// ���巵��ֵ
	bool bRet = false;
	/// ��֤���ݺϷ���
	if(NULL == psttNetClient)
	{
		END_DEBUG_INFO
		return bRet;
	}
	/// ������·��ʼ������
	memcpy(&m_sttInitNetClient, psttNetClient, sizeof(T_InitNetClient));
	bRet = true;
	END_DEBUG_INFO
	return bRet;
}

bool CNetBase::e_ConnectServer()
{
	START_DEBUG_INFO
	/// ���巵��ֵ
	bool bRet = false;
	/// �������ӷ�����
	bRet = e_ReconnectServer();
	/// �������պͷ����߳�
	bRet &= e_StartTCPReceive();
	bRet &= e_StartTCPSend();
	END_DEBUG_INFO
	return bRet;
}

bool CNetBase::e_CreatUDPClient()
{
	START_DEBUG_INFO
	/// ���巵��ֵ
	bool bRet = false;
	/// ��ʼ��UDP�׽���
	bRet = i_InitUDPSocket();
	/// �������պͷ����߳�
	bRet &= e_StartUDPReceive();
	bRet &= e_StartUDPSend();
	END_DEBUG_INFO
	return bRet;
}

void CNetBase::e_SetReceiveDataCallBack(OnRecvDataCallBack pfnRecDataCallBack)
{
	START_DEBUG_INFO
	/// ���ûص�����
	m_pfnRecDataCallBack = pfnRecDataCallBack;
	END_DEBUG_INFO
}

int CNetBase::e_SendTCPData(char* pszSendBuf, UINT nSendSize)
{
	START_DEBUG_INFO
	int nRet = -1;
	if(NULL == pszSendBuf || 0 >= nSendSize)
	{
		END_DEBUG_INFO
		return nRet;
	}

	int nSendedlen = 0, nSendedSize = nSendSize;
	int nSeletRet = SELECT_STATE_ERROR;
	/// ֱ��������ɻ����˳�ѭ��
	while(0 < nSendSize)
	{
		nSeletRet = e_SelectProcess(m_sClientTCPSocket, SELECT_MODE_WRITE);

		if(SELECT_STATE_READY == nSeletRet)
		{
			nSendedlen = send(m_sClientTCPSocket, pszSendBuf, nSendSize, 0);
			/// ��ֵ�������ݴ�С
			nSendSize -= nSendedlen;
			pszSendBuf += nSendedlen;
		}
		else if(SELECT_STATE_ERROR == nSeletRet)
		{
			printf("e_SendTCPData Error = %d\r\n", GetLastError());
			END_DEBUG_INFO
			return nRet;
		}
		else
		{
			/// ��ʱ
			END_DEBUG_INFO
			continue;
		}
	}
	END_DEBUG_INFO
	return nSendedSize - nSendSize;
}

int CNetBase::e_SendUDPData(const char* pszRemoteIP, USHORT usRemotePort,
	char* pszSendBuf, UINT nSendSize)
{
	/// ���巵��ֵ
	int nRet = -1;
	/// ��֤�������ݺϷ���
	if(NULL == pszRemoteIP || 0 >= usRemotePort || NULL != pszSendBuf || 0 >= nSendSize)
	{
		return nRet;
	}

	int nSelectState = e_SelectProcess(m_sClientUDPSocket, SELECT_MODE_WRITE);
	/// ��ʱ����
	if(SELECT_STATE_TIMEOUT == nSelectState || SELECT_STATE_ABORTED == nSelectState)
	{
		return 0;
	}
	else if(SELECT_STATE_READY == nSelectState)
	{
		/// ��ʼ��������
		sockaddr_in ClientAddr;
		int nAddrSize = sizeof(ClientAddr);
		ClientAddr.sin_family = AF_INET;
		ClientAddr.sin_port = htons(usRemotePort);
		ClientAddr.sin_addr.S_un.S_addr = inet_addr(pszRemoteIP);
		nRet = sendto(m_sClientUDPSocket, pszSendBuf, nSendSize, 0,
			(sockaddr *)&ClientAddr, nAddrSize);
		nRet = (0 >= nRet) ? -1 : nRet;
	}
	return nRet;
}

void CNetBase::e_CloseSocket()
{
	/// �ͷ��׽���
	e_CloseTCPSocket();
	e_CloseUDPSocket();
}

void CNetBase::e_CloseTCPSocket()
{
	START_DEBUG_INFO
	/// ������Ӵ��ڹر�����
	if(0 < m_sClientTCPSocket)
	{
		closesocket(m_sClientTCPSocket);
	}
	/// �׽���ID��ʼ��Ϊ0
	m_sClientTCPSocket = 0;
	/// ����״̬����Ϊδ����
	m_bIsTCPConnect = false;
	END_DEBUG_INFO
}

void CNetBase::e_CloseUDPSocket()
{
	START_DEBUG_INFO
	/// ���UDP�׽��ִ�����ر��׽���
	if(0 < m_sClientUDPSocket)
	{
		closesocket(m_sClientUDPSocket);
	}
	/// �׽���ID��ʼ��Ϊ0
	m_sClientUDPSocket = 0;
	/// ��ֵUDPδ����
	m_bIsUDPCreated = false;
	END_DEBUG_INFO
}

bool CNetBase::e_ReconnectServer()
{
	START_DEBUG_INFO
	/// ���巵��ֵ
	bool bRet = false;
	/// �������״̬
	if(true == m_bIsTCPConnect)
	{
		/// �ر��׽���
		e_CloseTCPSocket();
	}
	/// ��շ��ͻ�����������
	i_DestroyTCPSendBufferArray();

	/// ���ӷ���IpΪ���򷵻�
	if(0 == strlen(m_sttInitNetClient.szTCPServerIP))
	{
		END_DEBUG_INFO
		return bRet;
	}

	struct sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = inet_addr(m_sttInitNetClient.szTCPServerIP);
	serverAddress.sin_port = htons(m_sttInitNetClient.usTCPServerPort);
	/// ���TCP�׽�����Ч
	if(INVALID_SOCKET == m_sClientTCPSocket)
	{
		if(false == i_InitTCPSocket())
		{
			END_DEBUG_INFO
			return bRet;
		}
	}
	/// ���ӷ���������������״̬(-1 = ���Ӳ��ɹ�)
	bRet = m_bIsTCPConnect = (-1 != connect(m_sClientTCPSocket, (struct sockaddr *)&serverAddress,
		sizeof(serverAddress)));
	if(false == m_bIsTCPConnect)
	{
		/// ����ʧ�ܣ��ر��׽���
		e_CloseTCPSocket();
	}
	END_DEBUG_INFO
	return bRet;
}

bool CNetBase::e_IsTCPConnect()
{
	START_DEBUG_INFO
	/// ��������״̬
	END_DEBUG_INFO
	return m_bIsTCPConnect;
}

bool CNetBase::e_IsUDPCreated()
{
	START_DEBUG_INFO
	/// ����UDP����״̬
	END_DEBUG_INFO
	return m_bIsUDPCreated;
}

bool CNetBase::e_StartTCPReceive()
{
	START_DEBUG_INFO
	/// ���巵��ֵ
	bool bRet = false;
	/// ����߳�δ�����������߳�
	if(NULL == m_HRecvTCPThreadHandle)
	{
		/// ��������߳�
		m_HRecvTCPThreadHandle = CreateThread(NULL, 0, &i_RecvTCPThread, this, 0, NULL);
	}
	/// �߳��Ƿ�������
	bRet = (NULL == m_HRecvTCPThreadHandle) ? false : true;
	END_DEBUG_INFO
	return bRet;
}
bool CNetBase::e_StartTCPSend()
{
	START_DEBUG_INFO
	/// ���巵��ֵ
	bool bRet = false;
	/// ����߳�δ�����������߳�
	if(NULL == m_HSendTCPThreadHandle)
	{
		/// ��������߳�
		m_HSendTCPThreadHandle = CreateThread(NULL, 0, &i_RecvTCPThread, this, 0, NULL);
	}
	/// �߳��Ƿ�������
	bRet = (NULL == m_HSendTCPThreadHandle) ? false : true;
	END_DEBUG_INFO
	return bRet;
}

void CNetBase::e_StopTCPReceive()
{
	START_DEBUG_INFO
	/// ���TCP�����߳�����
	if(NULL != m_HRecvTCPThreadHandle)
	{
		/// �����˳��¼�
		SetEvent(m_HRecvTCPStopEven);
		WaitForSingleObject(m_HRecvTCPThreadHandle, INFINITE);
		CloseHandle(m_HRecvTCPThreadHandle);
		m_HRecvTCPThreadHandle = NULL;
	}
	END_DEBUG_INFO
}

void CNetBase::e_StopTCPSend()
{
	START_DEBUG_INFO
	/// ���TCP�����߳�����
	if(NULL != m_HSendTCPThreadHandle)
	{
		/// �����˳��¼�
		SetEvent(m_HSendTCPStopEven);
		WaitForSingleObject(m_HSendTCPThreadHandle, INFINITE);
		CloseHandle(m_HSendTCPThreadHandle);
		m_HSendTCPThreadHandle = NULL;
	}
	END_DEBUG_INFO
}

bool CNetBase::e_StartUDPReceive()
{
	START_DEBUG_INFO
	/// ���巵��ֵ
	bool bRet = false;
	/// ����߳�δ�����������߳�
	if(NULL == m_HRecvUDPThreadHandle)
	{
		/// ��������߳�
		m_HRecvUDPThreadHandle = CreateThread(NULL, 0, &i_RecvUDPThread, this, 0, NULL);
	}
	/// �߳��Ƿ�������
	bRet = (NULL == m_HRecvUDPThreadHandle) ? false : true;
	END_DEBUG_INFO
	return bRet;
}

bool CNetBase::e_StartUDPSend()
{
	START_DEBUG_INFO
	/// ���巵��ֵ
	bool bRet = false;
	/// ����߳�δ�����������߳�
	if(NULL == m_HSendUDPThreadHandle)
	{
		/// ��������߳�
		m_HSendUDPThreadHandle = CreateThread(NULL, 0, &i_RecvUDPThread, this, 0, NULL);
	}
	/// �߳��Ƿ�������
	bRet = (NULL == m_HSendUDPThreadHandle) ? false : true;
	END_DEBUG_INFO
	return bRet;
}

void CNetBase::e_StopUDPReceive()
{
	START_DEBUG_INFO
	/// ���UDP�����߳�����
	if(NULL != m_HRecvUDPThreadHandle)
	{
		/// �����˳��¼�
		SetEvent(m_HRecvUDPStopEven);
		WaitForSingleObject(m_HRecvUDPThreadHandle, INFINITE);
		CloseHandle(m_HRecvUDPThreadHandle);
		m_HRecvUDPThreadHandle = NULL;
	}
	END_DEBUG_INFO
}

void CNetBase::e_StopUDPSend()
{
	START_DEBUG_INFO
	/// ���TCP�����߳�����
	if(NULL != m_HSendUDPThreadHandle)
	{
		/// �����˳��¼�
		SetEvent(m_HSendUDPStopEven);
		WaitForSingleObject(m_HSendUDPThreadHandle, INFINITE);
		CloseHandle(m_HSendUDPThreadHandle);
		m_HSendUDPThreadHandle = NULL;
	}
	END_DEBUG_INFO
}

DWORD WINAPI CNetBase::i_RecvTCPThread(void* pBaseSocket)
{
	START_DEBUG_INFO
	/// ����TCP�����߳�
	CNetBase* pThis = (CNetBase* )pBaseSocket;
	if(NULL != pThis)
	{
		pThis->e_ReceiveTCPLoop();
	}
	END_DEBUG_INFO
	return 0;
}

DWORD WINAPI CNetBase::i_SendTCPThread(void* pBaseSocket)
{
	START_DEBUG_INFO
	/// ����TCP�����߳�
	CNetBase* pThis = (CNetBase* )pBaseSocket;
	if(NULL != pThis)
	{
		pThis->e_SendTCPLoop();
	}	
	END_DEBUG_INFO
	return 0;
}

DWORD WINAPI CNetBase::i_RecvUDPThread(void* pBaseSocket)
{
	START_DEBUG_INFO
	/// ����UDP�����߳�
	CNetBase* pThis = (CNetBase* )pBaseSocket;
	if(NULL != pThis)
	{
		pThis->e_ReceiveUDPLoop();
	}
	END_DEBUG_INFO
	return 0;
}

DWORD WINAPI CNetBase::i_SendUDPThread(void* pBaseSocket)
{
	START_DEBUG_INFO
	/// ����UDP�����߳�
	CNetBase* pThis = (CNetBase* )pBaseSocket;
	if(NULL != pThis)
	{
		pThis->e_SendUDPLoop();
	}	
	END_DEBUG_INFO
	return 0;
}

int CNetBase::e_RecTCPData(char* pszRevBuffer, int nRevLength)
{
	START_DEBUG_INFO
	/// ���巵��ֵ
	int nRet = -1;
	/// ��֤���ݺϷ���
	if(NULL == pszRevBuffer || 0 >= nRevLength)
	{
		END_DEBUG_INFO
		return nRet;
	}
	int nSeletRet = SELECT_STATE_ERROR;
	/// ���ݳ���
	int nRecvedlen = 0, nCurrlen = 0;
	while(0 < nRevLength)
	{
		nSeletRet = e_SelectProcess(m_sClientTCPSocket, SELECT_MODE_READY);
		if(SELECT_STATE_READY == nSeletRet)
		{
			nCurrlen = recv(m_sClientTCPSocket, pszRevBuffer + nRecvedlen, nRevLength, 0);
			if(0 <= nCurrlen)
			{
				/// ��ֵ�������ݴ�С
				nRecvedlen += nCurrlen;
				nRevLength -= nCurrlen;
			}
			else
			{
				printf("e_RecTCPData Rev Error = %d\r\n", GetLastError());
				END_DEBUG_INFO
				return nRet;
			}
		}
		else if(SELECT_STATE_ERROR == nSeletRet)
		{
			printf("e_RecTCPData Select Error = %d\r\n", GetLastError());
			END_DEBUG_INFO
			return nRet;
		}
		else
		{
			/// ��ʱ
			END_DEBUG_INFO
			continue;
		}
	}
	END_DEBUG_INFO
	return nRecvedlen;
}

int CNetBase::e_RecUDPData(char* pszRevBuffer, int nRevLength,
	string& strRemoteIP, USHORT& usRemotePort)
{
	START_DEBUG_INFO
	int nRet = -1;
	/// ��֤���ݺϷ���
	if(INVALID_SOCKET == m_sClientUDPSocket || NULL == pszRevBuffer || 0 >= nRevLength)
	{
		return nRet;
	}

	int nSelectState = e_SelectProcess(m_sClientUDPSocket, SELECT_MODE_READY);
	/// ��ʱ����
	if(SELECT_STATE_TIMEOUT == nSelectState || SELECT_STATE_ABORTED == nSelectState)
	{
		nRet = 0;
		return nRet;
	}
	else if(SELECT_STATE_READY == nSelectState)
	{
		sockaddr_in ClientAddr;
		int nAddrSize = sizeof(ClientAddr);
		/// ��ʼ��������
		nRet = recvfrom(m_sClientUDPSocket, pszRevBuffer, nRevLength, 
			0, (sockaddr* )&ClientAddr, &nAddrSize);
		nRet = (0 > nRet) ? -1 : nRet;
		/// �����������
		if(0 < nRet)
		{
			strRemoteIP = inet_ntoa(ClientAddr.sin_addr);
			usRemotePort = ntohs(ClientAddr.sin_port);
		}
	}
	return nRet;
}

int CNetBase::e_SelectProcess(HSOCKET sClientSocket, USHORT usMode, USHORT usProcessTimeOut)
{
	if(INVALID_SOCKET == sClientSocket)
	{
		return SELECT_STATE_ERROR;
	}
	fd_set fdSet;
	fd_set *readSet, *writeSet;
	/// ��ʱʱ�����
	timeval selectTimeout;
	selectTimeout.tv_sec  = usProcessTimeOut;
	selectTimeout.tv_usec = 0;
	FD_ZERO(&fdSet);
	FD_SET(sClientSocket, &fdSet);
	readSet = (usMode & SELECT_MODE_READY) ? &fdSet : NULL;
	writeSet = (usMode & SELECT_MODE_WRITE) ? &fdSet : NULL;

	int nRet = select((int)sClientSocket + 1, readSet, writeSet, NULL, &selectTimeout);

	if(1 == nRet)
	{
		return SELECT_STATE_READY;
	}
	else if(SOCKET_ERROR == nRet)
	{
		if(errno == EINTR)
		{
			return SELECT_STATE_ABORTED;
		}
		return SELECT_STATE_ERROR;
	} 
	return SELECT_STATE_TIMEOUT;
}

bool CNetBase::i_InitTCPSocket()
{
	START_DEBUG_INFO
	/// �ر�TCP�׽���
	e_CloseTCPSocket();
	/// ��ȡ�µ��׽���
	m_sClientTCPSocket = socket(AF_INET,SOCK_STREAM, 0);
	/// �����ȡ�׽��ֳɹ�,����
	if(INVALID_SOCKET != m_sClientTCPSocket)
	{		
		///����������������
		int nRetLay = 1;
		setsockopt(m_sClientTCPSocket, IPPROTO_TCP, TCP_NODELAY, (char *)&nRetLay, sizeof(int));
		/// ��������С
		int nBufferSize = 1024 * 64;
		/// ���÷��ͺͽ����׽��ֵĻ����С
		setsockopt(m_sClientTCPSocket, SOL_SOCKET, SO_SNDBUF, (const char *)&nBufferSize, sizeof(int));
		setsockopt(m_sClientTCPSocket, SOL_SOCKET, SO_RCVBUF, (const char *)&nBufferSize, sizeof(int));
	}
	else
	{
		m_sClientTCPSocket = 0;
	}
	END_DEBUG_INFO
	return (0 < m_sClientTCPSocket);
}

bool CNetBase::i_InitUDPSocket()
{
	START_DEBUG_INFO
	bool bRet = false;
	/// ����׽�����Ч�ر�ԭ���׽���
	if(true == m_bIsUDPCreated)
	{
		e_CloseUDPSocket();
	}
	/// �����׽���
	m_sClientUDPSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
	if(INVALID_SOCKET == m_sClientUDPSocket)
	{
		END_DEBUG_INFO
		return bRet;
	}

	/// ����Ϊsocket����,��ֹ������������˿��ܹ������ٴ�ʹ�û������Ľ���ʹ��
	int nOpt = 1;
	int nError = setsockopt(m_sClientUDPSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&nOpt, sizeof(int));
	if(SOCKET_ERROR == nError)
	{
		e_CloseUDPSocket();
		END_DEBUG_INFO
		return bRet;
	}

	/// = ���ý������ݻ���
	/// = 2M Byte 1000Mbps��network��0.01������߿��Խ��յ�1.25MB����
	int nSendBufVal = (int)(1024 * 1024 * 1.25);
	nError = setsockopt(m_sClientUDPSocket, SOL_SOCKET, SO_RCVBUF, (char*)&nSendBufVal, sizeof(nSendBufVal));
	if(nError == SOCKET_ERROR)
	{
		e_CloseUDPSocket();
		END_DEBUG_INFO
		return bRet;
	}

	/// = ���÷������ݻ���
	nError = setsockopt(m_sClientUDPSocket, SOL_SOCKET, SO_SNDBUF, (char*)&nSendBufVal, sizeof(nSendBufVal));
	if(nError == SOCKET_ERROR)
	{
		e_CloseUDPSocket();
		END_DEBUG_INFO
		return bRet;
	}

	ULONG nonBlock = 1;
	nError = ioctlsocket(m_sClientUDPSocket, FIONBIO, &nonBlock);
	if(nError == SOCKET_ERROR)
	{
		e_CloseUDPSocket();
		END_DEBUG_INFO
		return bRet;
	}

	/// ���׽ӿ�
	SOCKADDR_IN AddrLocal;
	AddrLocal.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	AddrLocal.sin_family = AF_INET;
	AddrLocal.sin_port = htons(m_sttInitNetClient.usLocalUDPPort);
	nError = bind(m_sClientUDPSocket, (struct sockaddr *)&AddrLocal, sizeof(SOCKADDR_IN));
	if(SOCKET_ERROR == nError)
	{
		e_CloseUDPSocket();
		END_DEBUG_INFO
		return bRet;
	}

	/// �Ƿ�����鲥
	if(TRUE == m_sttInitNetClient.usUDPJoinGroup)
	{
		/// ���ø�UDP�鲥��ַ
		struct ip_mreq sIpMreqGroup;
		/// �൱����������
		sIpMreqGroup.imr_multiaddr.s_addr = inet_addr(m_sttInitNetClient.szUDPGroupIP);
		sIpMreqGroup.imr_interface.s_addr = htons(INADDR_ANY);
		nError = setsockopt(m_sClientUDPSocket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&sIpMreqGroup, sizeof(sIpMreqGroup));
		int nLoopBack = 0;
		/// ȡ�����ػػ�����
		nError = setsockopt(m_sClientUDPSocket, IPPROTO_IP, IP_MULTICAST_LOOP, (char* )&nLoopBack, sizeof(int));
		if(SOCKET_ERROR == nError)
		{
			e_CloseUDPSocket();
			END_DEBUG_INFO
			return bRet;
		}
	}
	/// ��ֵUDP����״̬
	bRet = m_bIsUDPCreated = true;
	return bRet;
}

void CNetBase::i_CancelEvens()
{
	START_DEBUG_INFO
	if(NULL != m_HRecvTCPThreadHandle)
	{
		CloseHandle(m_HRecvTCPThreadHandle);
		m_HRecvTCPThreadHandle = NULL;
	}
	if(NULL != m_HSendTCPThreadHandle)
	{
		CloseHandle(m_HSendTCPThreadHandle);
		m_HSendTCPThreadHandle = NULL;
	}
	if(NULL != m_HRecvUDPThreadHandle)
	{
		CloseHandle(m_HRecvUDPThreadHandle);
		m_HRecvUDPThreadHandle = NULL;
	}
	if(NULL != m_HSendUDPThreadHandle)
	{
		CloseHandle(m_HSendUDPThreadHandle);
		m_HSendUDPThreadHandle = NULL;
	}
	END_DEBUG_INFO
}

void CNetBase::i_DestroyTCPSendBufferArray()
{
	START_DEBUG_INFO
	/// �����������黺��
	CAutoLock lckSendBuffer(&m_SendTCPBufferLock);
	/// ��ջ�����������
	T_TCPSendBuffer* pSendBuffer = NULL;
	for(int i = 0; i < (int)m_SendTCPBufferArray.size(); i++)
	{
		pSendBuffer = (T_TCPSendBuffer* )m_SendTCPBufferArray.at(i);
		if(NULL != pSendBuffer)
		{
			if(NULL != pSendBuffer->pszBuffer)
			{
				delete pSendBuffer->pszBuffer;
				pSendBuffer->pszBuffer = NULL;
			}
			delete pSendBuffer;
			pSendBuffer = NULL;
		}
	}
	m_SendTCPBufferArray.clear();
	END_DEBUG_INFO
}

void CNetBase::i_DestroyUDPSendBufferArray()
{
	START_DEBUG_INFO
	/// �����������黺��
	CAutoLock lckSendBuffer(&m_SendUDPBufferLock);
	/// ��ջ�����������
	T_UDPSendBuffer* pSendBuffer = NULL;
	for(int i = 0; i < (int)m_SendUDPBufferArray.size(); i++)
	{
		pSendBuffer = (T_UDPSendBuffer* )m_SendUDPBufferArray.at(i);
		if(NULL != pSendBuffer)
		{
			if(NULL != pSendBuffer->pszBuffer)
			{
				delete pSendBuffer->pszBuffer;
				pSendBuffer->pszBuffer = NULL;
			}
			delete pSendBuffer;
			pSendBuffer = NULL;
		}
	}
	m_SendUDPBufferArray.clear();
	END_DEBUG_INFO
}