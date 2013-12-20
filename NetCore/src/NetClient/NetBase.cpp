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
	/// ��ʼ��������IP
	memset(m_szServerIP, 0x00, sizeof(m_szServerIP));
	/// ��ʼ���������˿ں�
	m_usServerTCPPort = 0;
	m_usClientUDPPort = 0;
	/// ��ʼ���߳̾��
	m_HRecvThreadHandle = NULL;
	m_HSendThreadHandle = NULL;
	/// ��ʼ���߳̽����¼�
	m_HRecvStopEven = CreateEvent(NULL, FALSE, FALSE, NULL);
	m_HSendStopEven = CreateEvent(NULL, FALSE, FALSE, NULL);
	/// ��ʼ����������
	m_SendBufferArray.clear();
	/// ��ʼ���ص�����
	m_pfnRecDateCallBack = NULL;
	/// ��ʼ�����ӷ�����״̬
	m_bConnect = false;
	END_DEBUG_INFO
}

CNetBase::~CNetBase()
{
	START_DEBUG_INFO
	/// ֹͣ�����߳�
	e_StopReceive();
	/// ֹͣ�����߳�
	e_StopSend();
	/// �ͷ��׽���
	e_CloseSocket();
	/// ��շ��͵Ļ�����������
	i_DestroySendBufferArray();
	END_DEBUG_INFO
}

bool CNetBase::e_ConnectServer(const char* pszServerIP, USHORT usServerPort)
{
	START_DEBUG_INFO
	/// ���巵��ֵ
	bool bRet = false;
	/// ��֤���ݺϷ���
	if(NULL == pszServerIP || 0 >= usServerPort)
	{
		return bRet;
	}
	/// ���÷�����IP
	strncpy_s(m_szServerIP, pszServerIP, sizeof(m_szServerIP) - 1);
	/// ���÷���˿ں�
	m_usServerTCPPort = usServerPort;
	END_DEBUG_INFO
	return e_ReconnectServer();
}

bool CNetBase::e_CreatUDPClient(USHORT usClientUDPPort)
{
	START_DEBUG_INFO
	/// ���巵��ֵ
	bool bRet = false;
	/// ��֤���ݺϷ���
	if(0 >= usClientUDPPort)
	{
		return bRet;
	}
	/// ��ֵUDP�˿ں�
	m_usClientUDPPort = usClientUDPPort;
	/// ��ʼ��UDP�׽���
	bRet = i_InitUDPSocket();
	END_DEBUG_INFO
	return bRet;
}

void CNetBase::e_SetReceiveDataCallBack(OnRecvDataCallBack pfnRecDataCallBack)
{
	START_DEBUG_INFO
	/// ���ûص�����
	m_pfnRecDateCallBack = pfnRecDataCallBack;
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
	m_bConnect = false;
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
	END_DEBUG_INFO
}

bool CNetBase::e_ReconnectServer()
{
	START_DEBUG_INFO
	/// ���巵��ֵ
	bool bRet = false;
	/// �������״̬
	if(true == m_bConnect)
	{
		/// �ر��׽���
		e_CloseTCPSocket();
	}
	/// ��շ��ͻ�����������
	i_DestroySendBufferArray();

	/// ���ӷ���IpΪ���򷵻�
	if(0 == strlen(m_szServerIP))
	{
		END_DEBUG_INFO
		return bRet;
	}

	struct sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = inet_addr(m_szServerIP);
	serverAddress.sin_port = htons(m_usServerTCPPort);
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
	bRet = m_bConnect = (-1 != connect(m_sClientTCPSocket, (struct sockaddr *)&serverAddress,
		sizeof(serverAddress)));
	if(false == m_bConnect)
	{
		/// ����ʧ�ܣ��ر��׽���
		e_CloseTCPSocket();
	}
	END_DEBUG_INFO
	return bRet;
}

bool CNetBase::e_IsConnect()
{
	START_DEBUG_INFO
	/// ��������״̬
	END_DEBUG_INFO
	return m_bConnect;
}

int CNetBase::e_StartReceive()
{
	START_DEBUG_INFO
	/// ��������
	int nResult = 0;
	/// ��������߳�
	m_HRecvThreadHandle = CreateThread(NULL, 0, &i_ReceiveThread, this, 0, NULL);
	END_DEBUG_INFO
	return nResult;
}

int CNetBase::e_StartSend()
{
	START_DEBUG_INFO
	int nResult = 0;
	m_HSendThreadHandle = CreateThread(NULL, 0, &i_SendThread, this, 0, NULL);

	END_DEBUG_INFO
	return nResult;
}

void CNetBase::e_StopReceive()
{
	START_DEBUG_INFO
	WaitForMultipleObjects(1, &m_HRecvThreadHandle, TRUE, INFINITE);
	CloseHandle(m_HRecvThreadHandle);
	END_DEBUG_INFO
}

void CNetBase::e_StopSend()
{
	START_DEBUG_INFO
	/// �رշ��������߳�
	WaitForMultipleObjects(1, &m_HSendThreadHandle, TRUE, INFINITE);
	CloseHandle(m_HSendThreadHandle);
	END_DEBUG_INFO
}

DWORD WINAPI CNetBase::i_ReceiveThread(void* pBaseSocket)
{
	START_DEBUG_INFO
	/// ���������߳�
	CNetBase* pThis = (CNetBase* )pBaseSocket;
	pThis->e_ReceiveLoop();
	END_DEBUG_INFO
	return 0;
}

DWORD WINAPI CNetBase::i_SendThread(void* pBaseSocket)
{
	START_DEBUG_INFO
	/// ���������߳�
	CNetBase* pThis = (CNetBase* )pBaseSocket;
	pThis->e_SendLoop();
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

void CNetBase::i_DestroySendBufferArray()
{
	START_DEBUG_INFO
	/// �����������黺��
	CAutoLock lckSendBuffer(&m_SendBufferLock);

	/// ��ջ�����������
	T_BufferStruct* pBuffer = NULL;
	for(int i = 0; i < (int)m_SendBufferArray.size(); i++)
	{
		pBuffer = (T_BufferStruct* )m_SendBufferArray.at(i);
		if(NULL != pBuffer)
		{
			if(pBuffer->pBuf)
			{
				delete pBuffer->pBuf;
			}
			delete pBuffer;
			pBuffer = NULL;
		}
	}
	m_SendBufferArray.clear();
	END_DEBUG_INFO
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
		END_DEBUG_INFO
		return bRet;
	}

	/// = ���ý������ݻ���
	/// = 2M Byte 1000Mbps��network��0.01������߿��Խ��յ�1.25MB����
	int nSendBufVal = (int)(1024 * 1024 * 1.25);
	nError = setsockopt(m_sClientUDPSocket, SOL_SOCKET, SO_RCVBUF, (char*)&nSendBufVal, sizeof(nSendBufVal));
	if(nError == SOCKET_ERROR)
	{
		END_DEBUG_INFO
		return bRet;
	}

	/// = ���÷������ݻ���
	nError = setsockopt(m_sClientUDPSocket, SOL_SOCKET, SO_SNDBUF, (char*)&nSendBufVal, sizeof(nSendBufVal));
	if(nError == SOCKET_ERROR)
	{
		END_DEBUG_INFO
		return bRet;
	}

	ULONG nonBlock = 1;
	if (ioctlsocket(m_sClientUDPSocket, FIONBIO, &nonBlock)) 
	{
		END_DEBUG_INFO
		return bRet;
	}

	/// ���׽ӿ�
	SOCKADDR_IN AddrLocal;
	AddrLocal.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	AddrLocal.sin_family = AF_INET;
	AddrLocal.sin_port = htons(m_usClientUDPPort);
	nError = bind(m_sClientUDPSocket, (struct sockaddr *)&AddrLocal, sizeof(SOCKADDR_IN));
	if(SOCKET_ERROR == nError)
	{
		END_DEBUG_INFO
		return bRet;
	}
	return true;
}