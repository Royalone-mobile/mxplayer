/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 
/// @par ģ����
/// IOCP����ģ��
/// @par ����ļ�
/// NetIOCP.h
/// @par ������ϸ����
/// @par ���̰߳�ȫ��
/// [��˵��]
/// @par �쳣ʱ��ȫ��
/// [��˵��]
/// @note         -
/// @file         NetIOCP.cpp
/// @brief        -
/// @author       Li.xl
/// @version      1.0
/// @date         2013/06/08
/// @todo         -
/// 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "NetIOCP.h"

CNetIOCP::CNetIOCP()
{
	START_DEBUG_INFO
	/// �������״̬
	m_bNetRunStatus = false;
	/// ��ʼ������˼����߳̾��
	m_hListenThread = NULL;
	/// ��ʼ������˼����׽���
	m_sListenTCPSocket = NULL;
	/// ��ʼ��UDP�����׽���
	m_sUDPServerSocket = NULL;
	/// ��ʼ��UDP�����������Ϣ
	m_pUDPSeverContext = NULL;
	/// ��ʼ������˼����¼����
	m_hPostAcceptEvent = NULL;
	/// ��ʼ����ɶ˿ھ��
	m_hCompletionPort = NULL;
	/// ��ʼ�������߳̾��
	for(int i = 0;  i < MAX_PROCESSER_NUMBERS; i++)
	{
		m_hWorkThread[i] = NULL;
	}
	/// ��ʼ���ص�����ָ�����
	m_pFunRecvData = NULL;
	m_pFunSendData = NULL;
	m_pFunConection = NULL;
	m_pFunDisConection = NULL;
	m_pFunPrintLog = NULL;
	/// ��ʼ�������б�������
	m_ContextManager.e_InitContextList(&m_CBufferManager);
	END_DEBUG_INFO
}

CNetIOCP::~CNetIOCP()
{
	START_DEBUG_INFO
	/// ֹͣ����
	e_StopdNetService();
	END_DEBUG_INFO
}

/// ��ʼ���������ģ��
bool CNetIOCP::e_InitNetModel(T_NetInitStruct& sttNetInitStruct,
	OnRecvDataCallBack pFunRecvData,
	OnSendDataCallBack pFunSendData, 
	OnConectionCallBack pFunConection, 
	OnDisConectionCallBack pFunDisConection,
	OnPrintLog pFunPrintLog)
{
	START_DEBUG_INFO
	/// ��ֵ������������ݻص�����
	memcpy(&m_sttNetInitStruct, &sttNetInitStruct, sizeof(T_NetInitStruct));
	/// �����������Ϊ0, ����Ĭ�Ͽ�����������
	if(0 == m_sttNetInitStruct.usFreeConnMaxNum)
	{
		m_sttNetInitStruct.usFreeConnMaxNum = NETCORE_TCP_CONPOOL_NUM;
	}
	/// �����������Ϊ0, ����Ĭ�Ͽ��л�������
	if(0 == m_sttNetInitStruct.usFreeBufMaxNum)
	{
		m_sttNetInitStruct.usFreeBufMaxNum = NETCORE_BUFFER_POOL_NUM;
	}
	/// ���Ͷ���¼���������Ϊ 0
	if(0 == m_sttNetInitStruct.usPendReadsNum)
	{
		/// ��ֵΪ�����߳�����
		m_sttNetInitStruct.usPendReadsNum = m_sttNetInitStruct.usMaxIOWorkers;
	}
	m_pFunRecvData = pFunRecvData;
	m_pFunSendData = pFunSendData;
	m_pFunConection = pFunConection;
	m_pFunDisConection = pFunDisConection;
	m_pFunPrintLog = pFunPrintLog;
	END_DEBUG_INFO
	return true;
}

/// �����������
bool CNetIOCP::e_StartNetService()
{
	START_DEBUG_INFO
	/// ��֤�����Ƿ���������
	if(true == m_bNetRunStatus)
	{
		END_DEBUG_INFO;
		return false;
	}
	/// ��ֵ������
	m_bNetRunStatus = true;
	/// ������ɶ˿�
	m_hCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if(NULL == m_hCompletionPort)
	{
		m_bNetRunStatus = false;
		END_DEBUG_INFO
		return false;
	}
	bool bRet = true;
	/// ����������ΪTCP����
	if(NCNT_TCP == m_sttNetInitStruct.usServerNetType ||
		NCNT_Both == m_sttNetInitStruct.usServerNetType)
	{
		bRet &= i_RunTCPService();
	}
	/// ����������ΪUDP����
	if(NCNT_UDP == m_sttNetInitStruct.usServerNetType ||
		NCNT_Both == m_sttNetInitStruct.usServerNetType)
	{
		bRet &= i_RunUDPService();
	}
	/// ������������ɹ�
	if(true == bRet)
	{
		/// ���ÿ�����������
		m_ContextManager.e_SetFreeConNum(m_sttNetInitStruct.usFreeConnMaxNum);
		/// ���ÿ��л�������
		m_CBufferManager.e_SetBuffFreeCount(m_sttNetInitStruct.usFreeBufMaxNum);
		/// ��������߳���Ϊ0����ȡ��ѹ����߳���
		if(0 == m_sttNetInitStruct.usMaxIOWorkers)
		{
			/// ����߳�������CPU������2��
			SYSTEM_INFO	sysinfo;
			GetSystemInfo(&sysinfo);
			m_sttNetInitStruct.usMaxIOWorkers = (USHORT)sysinfo.dwNumberOfProcessors * 2 + 2;
			/// ��ֵͶ�ݽ�������
			m_sttNetInitStruct.usPendReadsNum = m_sttNetInitStruct.usMaxIOWorkers;
		}

		/// ��֤�Ƿ񳬹���������߳���������
		if(MAX_PROCESSER_NUMBERS < m_sttNetInitStruct.usMaxIOWorkers)
		{
			/// ��ֵ�����߳���
			m_sttNetInitStruct.usMaxIOWorkers = MAX_PROCESSER_NUMBERS;
		}

		/// ����߳�������1������Ͷ�ݿɶ����ݴ���1�����ñ��밴˳���д����
		if(1 < m_sttNetInitStruct.usMaxIOWorkers && 1 < m_sttNetInitStruct.usPendReadsNum)
		{
			m_sttNetInitStruct.bOrderSend = true;
			m_sttNetInitStruct.bOrderRead = true;
		}

		/// ���������߳�
		USHORT usIOWorkers = 0;
		while(usIOWorkers != m_sttNetInitStruct.usMaxIOWorkers)
		{
			/// ���������߳�
			m_hWorkThread[usIOWorkers] = (HANDLE)_beginthreadex(NULL, 0, i_WorkThread, this, 0, NULL);
			if(NULL != m_hWorkThread[usIOWorkers])
			{
				usIOWorkers++;
			}
		}
		/// ��������ɹ�
		e_PrintLog("�����������ɹ�...\r\n");
	}
	else
	{
		/// ��ֵδ�����ɹ�
		m_bNetRunStatus = false;
	}
	END_DEBUG_INFO
	return bRet;
}

bool CNetIOCP::e_StopdNetService()
{
	START_DEBUG_INFO
	/// ֹͣ�����߳��Լ������߳�
	if(true == m_bNetRunStatus)
	{
		m_bNetRunStatus = false;
		/// �����߳��˳�
		if(NULL != m_hListenThread)
		{
			WaitForSingleObject(m_hListenThread, INFINITE);
			CloseHandle(m_hListenThread);
			m_hListenThread = NULL;
		}
		/// �����߳��˳�
		int nIONums = m_sttNetInitStruct.usMaxIOWorkers;
		while(0 < nIONums)
		{
			if(TRUE == PostQueuedCompletionStatus(m_hCompletionPort, 0, (DWORD)NULL, NULL))
			{
				nIONums--;
			}
		}
		/// �ȴ����ź��¼�
		WaitForMultipleObjects(m_sttNetInitStruct.usMaxIOWorkers,
			m_hWorkThread, TRUE, INFINITE);
		for(int i = 0;  i < m_sttNetInitStruct.usMaxIOWorkers; i++)
		{
			if(NULL != m_hWorkThread[i])
			{
				CloseHandle(m_hWorkThread[i]);
				m_hWorkThread[i] = NULL;
			}
		}
		/// �ͷ�����֪ͨ�¼�
		if(NULL != m_hPostAcceptEvent)
		{
			CloseHandle(m_hPostAcceptEvent);
			m_hPostAcceptEvent = NULL;
		}
		/// ֹͣ��ɶ˿ڲ���
		if(NULL != m_hCompletionPort)
		{
			CloseHandle(m_hCompletionPort);
			m_hCompletionPort = NULL;
		}
		/// �ͷ�TCP�����׽��֣���UDP�����׽���
		e_CloseSocket(m_sListenTCPSocket);
		e_CloseSocket(m_sUDPServerSocket);
		/// �ͷ�UDP�������Ӷ�����Ϣ
		if(NULL != m_pUDPSeverContext)
		{
			m_ContextManager.e_ReleaseConntext(m_pUDPSeverContext);
			m_pUDPSeverContext = NULL;
		}
		/// ���÷���˼����׽���
		m_sListenTCPSocket = NULL;
		/// ����UDP�����׽���
		m_sUDPServerSocket = NULL;
		/// ���÷�������ṹ
		m_sttNetInitStruct.e_InitStruct();
		/// ֹͣ����ɹ�
		e_PrintLog("ֹͣ�������ɹ�...\r\n");
		/// ���ûص�����ָ�����
		m_pFunRecvData = NULL;
		m_pFunSendData = NULL;
		m_pFunConection = NULL;
		m_pFunDisConection = NULL;
		m_pFunPrintLog = NULL;
	}
	END_DEBUG_INFO
	return true;
}

/// ����TCP�������
bool CNetIOCP::i_RunTCPService()
{
	START_DEBUG_INFO
	/// ����TCP����˼����׽���
	m_sListenTCPSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if(INVALID_SOCKET == m_sListenTCPSocket)
	{
		END_DEBUG_INFO
		return false;
	}

	///// ��receiveform��sendto����Ϊ�첽������ģʽ,��Ϊ��WSAStartup��ʼ��Ĭ����ͬ��
	//ULONG ul = 1;
	//int nErrorCode = ioctlsocket(m_sListenTCPSocket, FIONBIO, &ul);
	//if(SOCKET_ERROR == nErrorCode)
	//{
	//	END_DEBUG_INFO
	//	return false;
	//}

	/// ����Ϊ��ַ���ã��ŵ����ڷ������رպ������������		
	char szOptRet = 1;
	int nErrorCode = setsockopt(m_sListenTCPSocket, SOL_SOCKET, SO_REUSEADDR, &szOptRet, sizeof(szOptRet));
	if(SOCKET_ERROR == nErrorCode)
	{
		END_DEBUG_INFO
		return false;
	}

	/// ���ü����˿�
	SOCKADDR_IN	sttTcpServerAdd;
	sttTcpServerAdd.sin_family = AF_INET;
	sttTcpServerAdd.sin_addr.s_addr = INADDR_ANY;
	sttTcpServerAdd.sin_port = htons(m_sttNetInitStruct.usServerPort);
	/// ��׽��ֺͷ�������ַ
	nErrorCode = bind(m_sListenTCPSocket, (struct sockaddr* )&sttTcpServerAdd, sizeof(sttTcpServerAdd));
	if(SOCKET_ERROR == nErrorCode)
	{
		END_DEBUG_INFO
		return false;
	}

	/// ����IO�¼�
	m_hPostAcceptEvent = WSACreateEvent();
	/// ����ʧ���򷵻�
	if(WSA_INVALID_EVENT == m_hPostAcceptEvent)
	{
		/// �ر��׽���
		e_CloseSocket(m_sListenTCPSocket);
		END_DEBUG_INFO
		return false;
	}

	/// = ���¼�����ϵͳ����
	WSAEventSelect(m_sListenTCPSocket, m_hPostAcceptEvent, FD_ACCEPT);
	/// = ���ü����ȴ����
	/// = �ڶ����������������Ӷ�������ĵȴ������Ŀ.���������server��Ҫ�����ֵ
	nErrorCode = listen(m_sListenTCPSocket, SOMAXCONN);
	if(SOCKET_ERROR == nErrorCode)
	{
		END_DEBUG_INFO
		return false;
	}

	/// ���������߳�
	m_hListenThread = (HANDLE)_beginthreadex(NULL, 0, i_ListenThread, this, 0, NULL);
	if(NULL == m_hListenThread)
	{
		END_DEBUG_INFO
		return false;
	}
	END_DEBUG_INFO
	return true;
}

/// ����UDP�������
bool CNetIOCP::i_RunUDPService()
{
	START_DEBUG_INFO
	/// ���������׽���
	m_sUDPServerSocket = WSASocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if(INVALID_SOCKET == m_sUDPServerSocket)
	{
		END_DEBUG_INFO
		return false;
	}

	/// ��receiveform��sendto����Ϊ�첽������ģʽ,��Ϊ��WSAStartup��ʼ��Ĭ����ͬ��
	//ULONG ul = 1;
	//int nErrorCode = ioctlsocket(m_sUDPServerSocket, FIONBIO, &ul);
	//if(SOCKET_ERROR == nErrorCode)
	//{
	//	END_DEBUG_INFO
	//	return false;
	//}
	
	/// ����Ϊ��ַ���ã��ŵ����ڷ������رպ������������		
	char szOptRet = 1;
	int nErrorCode = setsockopt(m_sUDPServerSocket, SOL_SOCKET, SO_REUSEADDR, &szOptRet, sizeof(szOptRet));
	if(SOCKET_ERROR == nErrorCode)
	{
		END_DEBUG_INFO
		return false;
	}

	/// ����UDP���ݱ���������
	int nCurTTL = 255;
	/// �����µ�TTLֵ
	setsockopt(m_sUDPServerSocket, IPPROTO_IP, IP_TTL, (char *)&nCurTTL, sizeof(nCurTTL));

	DWORD dwBytesReturned = 0;
	int bNewBehavior = FALSE;
	/// ����ĺ������ڽ��Զ��ͻȻ�رջᵼ��WSARecvFrom����10054�����·�������ɶ�����û��Receive����������
	nErrorCode  = WSAIoctl(m_sUDPServerSocket, SIO_UDP_CONNRESET, &bNewBehavior,
		sizeof(bNewBehavior), NULL, 0, &dwBytesReturned, NULL, NULL);
	if(SOCKET_ERROR == nErrorCode)
	{
		END_DEBUG_INFO
		return false;
	}
	/// ����������ַ��Ϣ
	SOCKADDR_IN UdpServerAdd;
	UdpServerAdd.sin_family = AF_INET;
	UdpServerAdd.sin_addr.s_addr = INADDR_ANY;
	UdpServerAdd.sin_port = htons(m_sttNetInitStruct.usServerPort);

	/// ��׽��ֺͷ�������ַ
	nErrorCode = bind(m_sUDPServerSocket, (struct sockaddr*)&UdpServerAdd, sizeof(UdpServerAdd));
	if(SOCKET_ERROR == nErrorCode)
	{
		END_DEBUG_INFO
		return false;
	}

	/// �Ƿ�����鲥
	if(true == m_sttNetInitStruct.bUDPJoinGroup)
	{
		/// ���ø�UDP�鲥��ַ
		struct ip_mreq sIpMreqGroup;
		/// �൱����������
		sIpMreqGroup.imr_multiaddr.s_addr = inet_addr(m_sttNetInitStruct.szUDPGroupIP);
		sIpMreqGroup.imr_interface.s_addr = htons(INADDR_ANY);
		nErrorCode = setsockopt(m_sUDPServerSocket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&sIpMreqGroup, sizeof(sIpMreqGroup));
		/// int nLoopBack = 0;
		/// ȡ�����ػػ�����
		/// setsockopt(m_sUDPServerSocket, IPPROTO_IP, IP_MULTICAST_LOOP, (char* )&nLoopBack, sizeof(int));
		if(SOCKET_ERROR == nErrorCode)
		{
			END_DEBUG_INFO
			return false;
		}
	}

	if(NULL == m_pUDPSeverContext)
	{
		if(true == m_ContextManager.e_AllocateConntext(m_pUDPSeverContext))
		{
			m_pUDPSeverContext->sSocketID = m_sUDPServerSocket;
			/// �Ѽ����̺߳���ɶ˿ڰ
			HANDLE hHandle = CreateIoCompletionPort((HANDLE)m_pUDPSeverContext->sSocketID,
				m_hCompletionPort, (ULONG)m_pUDPSeverContext, 0);
			if(hHandle != m_hCompletionPort)
			{
				/// �Ƴ�������Ϣ
				m_ContextManager.e_ReleaseConntext(m_pUDPSeverContext);
				/// ��ֵ����Ϊ��
				m_pUDPSeverContext = NULL;
				/// �ͷ��׽�������
				e_CloseSocket(m_sUDPServerSocket);
				END_DEBUG_INFO
				return false;
			}
			/// ��ȡͶ�ݸ���
			i_PostUDPRecvIssue(m_sttNetInitStruct.usUDPRevcNum);
		}
	}
	END_DEBUG_INFO
	return true;
}

/// TCP�����������߳�
UINT WINAPI CNetIOCP::i_ListenThread(void* pParama)
{
	START_DEBUG_INFO
	CNetIOCP* pThis = (CNetIOCP* )pParama;
	/// �������ָ��Ϊ�շ��أ����߷����TCP�����׽��ֲ����ڣ��˳��߳�
	if(NULL == pThis || NULL == pThis->m_sListenTCPSocket)
	{
		END_DEBUG_INFO
		return 0; 
	}

	ULONG	ulRet = 0;
	USHORT	usPostAcceptNum = 1;
	int nAddrSize = sizeof(SOCKADDR_IN);
	/// ��֤����״̬
	while(true == pThis->m_bNetRunStatus)
	{
		ulRet = WSAWaitForMultipleEvents(1, &pThis->m_hPostAcceptEvent, FALSE, 100, FALSE);
		/// ��֤����״̬
		if(false == pThis->m_bNetRunStatus)
		{
			break;
		}

		if(WSA_WAIT_FAILED == ulRet || WSA_WAIT_TIMEOUT == ulRet)
		{
			continue;
		}
		else  
		{
			WSANETWORKEVENTS NetWorkEvents;
			/// �����ָ�����׽ӿ��������¼��ķ�����
			int nRet = WSAEnumNetworkEvents(pThis->m_sListenTCPSocket,
				pThis->m_hPostAcceptEvent, &NetWorkEvents);
			/// ������ִ���,����ѭ��
			if(SOCKET_ERROR == nRet)
			{
				break;
			}

			/// λ����Ϊ��, ��Ϊ�����¼�
			if(NetWorkEvents.lNetworkEvents & FD_ACCEPT)
			{
				/// �����ȡ���¼��޴���
				if(0 == NetWorkEvents.iErrorCode[FD_ACCEPT_BIT] && true == pThis->m_bNetRunStatus)
				{
					SOCKET sClientSocket = INVALID_SOCKET;
					/// ��ȡ���׽�������
					sClientSocket = WSAAccept(pThis->m_sListenTCPSocket, NULL, &nAddrSize, 0, 0);
					/// �׽��ִ���
					if(SOCKET_ERROR == sClientSocket)
					{
						/// ��ȡ������
						nRet = WSAGetLastError();
						/// ��������û�������
						if(WSAEWOULDBLOCK != nRet)
						{
							pThis->e_PrintLog("i_ListenThread WASAccept() Error.\r\n");
						}
					}
					else
					{
						/// �µ����ӵ�����Ϊ�����߳���׼��
						pThis->i_AssIncomClientWithContext(sClientSocket);
					}
				}
			}
		}
	}
	END_DEBUG_INFO
	return 0;
}

bool CNetIOCP::i_AssIncomClientWithContext(SOCKET sClientSocket)
{
	START_DEBUG_INFO
	bool bRet = false;
	/// ��֤�Ϸ���
	if(INVALID_SOCKET == sClientSocket)
	{
		return bRet;
	}
	/// ����������Ϣָ��
	T_ClientContext* pContextStruct = NULL;
	/// ��ȡһ���µ�������Ϣ
	if(true == m_ContextManager.e_AllocateConntext(pContextStruct))
	{
		/// �����׽��ֳ�ʼ������
		pContextStruct->sSocketID = sClientSocket;
		/// ����ѹ���㷨
		const char chOpt = 1;
		int nErrCode = setsockopt(pContextStruct->sSocketID, IPPROTO_TCP, TCP_NODELAY, &chOpt, sizeof(char));
		/// ���������׽���ʧ��
		if(-1 == nErrCode)
		{
			/// �Ƴ�������Ϣ
			m_ContextManager.e_ReleaseConntext(pContextStruct);
			/// �ͷ��׽�������
			e_CloseSocket(sClientSocket);
			END_DEBUG_INFO;
			return bRet;
		}

		/// �����׽��ִ�����Ӧ��ɶ˿�
		HANDLE hHandle = CreateIoCompletionPort((HANDLE)pContextStruct->sSocketID, m_hCompletionPort, (ULONG)pContextStruct, 0);
		if(hHandle != m_hCompletionPort)
		{
			/// �Ƴ�������Ϣ
			m_ContextManager.e_ReleaseConntext(pContextStruct);
			/// �ͷ��׽�������
			e_CloseSocket(sClientSocket);
			END_DEBUG_INFO;
			return bRet;
		}

		/// �������Ϸ�
		CNetBuffer* pNetBuffer = NULL;
		if(true == m_CBufferManager.e_AllocateBuffer(pNetBuffer, IOPT_TACCEPT))
		{
			/// ָ����ɶ˿�m_hCompletionPort������ֹ�߳�
			BOOL bSuccess = PostQueuedCompletionStatus(m_hCompletionPort, 0, (DWORD)pContextStruct, &pNetBuffer->m_sttIOOverLapped);
			/// ����ʧ�ܷ�����ֹ�߳�
			if(FALSE == bSuccess && ERROR_IO_PENDING != GetLastError())
			{
				/// �ͷ�������Ϣ
				m_ContextManager.e_ReleaseConntext(pContextStruct);
				/// �ͷŻ�������
				m_CBufferManager.e_ReleaseBuffer(pNetBuffer);
				END_DEBUG_INFO;
				return FALSE;
			}
		}
		else
		{
			/// �Ƴ�������Ϣ
			m_ContextManager.e_ReleaseConntext(pContextStruct);
			/// �ͷ��׽�������
			e_CloseSocket(sClientSocket);
		}
	}
	return TRUE;
}

/// ����������߳�
UINT WINAPI CNetIOCP::i_WorkThread(void* pParama)
{
	START_DEBUG_INFO
	CNetIOCP* pThis = (CNetIOCP* )pParama;
	/// �������ָ��Ϊ�շ��أ��˳��߳�
	if(NULL != pThis)
	{
		pThis->i_IoWorkerLoop();
	}
	END_DEBUG_INFO
	return 0;
}

/// IOCP����������
void CNetIOCP::i_IoWorkerLoop()
{
	START_DEBUG_INFO
	try
	{
		/// ����������ʱ����
		DWORD dwIoSize;
		bool bError = false;
		BOOL bIORet = FALSE;
		DWORD dwIOError = 0;
		LPOVERLAPPED pOverlapped = NULL;
		T_ClientContext* pContextStruct = NULL;
		CNetBuffer* pNetBuffer = NULL;
		/// ��ȡIO����
		while(false == bError && true == m_bNetRunStatus)
		{
			/// ��ʼ����ʱ����
			pOverlapped = NULL;
			pContextStruct = NULL;
			/// ��ȡһ����ɶ˿ڶ���
			bIORet = GetQueuedCompletionStatus(m_hCompletionPort, &dwIoSize,
				(LPDWORD)&pContextStruct, &pOverlapped, INFINITE);

			/// �����ȡIOCP���ж���ʧ��
			if(FALSE == bIORet)
			{
				/// �����Ϊ���ӳ�ʱ
				if(WAIT_TIMEOUT != WSAGetLastError())
				{
					/// ������Ӵ���
					if(NULL != pContextStruct)
					{
						/// OVERLAPPED�Ϸ�
						if(NULL != pOverlapped)
						{
							/// ��ȡIOCP���������õ�OVERLAPPED
							if(NULL != (pNetBuffer = CONTAINING_RECORD(pOverlapped, CNetBuffer, m_sttIOOverLapped)))
							{
								/// ���ΪUDP���մ����ύ��Ϣ�Ľ��յ���ɶ˿�
								if(IOPT_UDPRECV == pNetBuffer->e_GetOperation())
								{
									///// �ͷ����ݻ���
									//m_CBufferManager.e_ReleaseBuffer(pNetBuffer);
									///// Ͷ���º��������ݵ���ɶ˿�
									//i_PostUDPRecvIssue();
									/// �ͷ����ݻ���
									pNetBuffer->e_ReBuffValue();
									/// Ͷ�ݽ������ݵ��ͻ���
									i_PostUDPRecvIssue(pNetBuffer);
									continue;
								}
								/// �ͷ����
								m_CBufferManager.e_ReleaseBuffer(pNetBuffer);
							}
							/// ��������ָ��
							pOverlapped = NULL;
						}
						/// �ͷ�����
						e_DisConectionConntext(pContextStruct);
					}
				}

				/// OVERLAPPED�Ϸ�
				if(NULL != pOverlapped)
				{
					/// ��ȡIOCP���������õ�OVERLAPPED
					if(NULL != (pNetBuffer = CONTAINING_RECORD(pOverlapped, CNetBuffer, m_sttIOOverLapped)))
					{
						m_CBufferManager.e_ReleaseBuffer(pNetBuffer);
					}
				}
				/// ��������ѭ�������´�ѭ��
				continue;
			}

			/// ���Ӷ�����
			if(NULL != pContextStruct && NULL != pOverlapped)
			{
				/// ���ݰ�����Ϸ�
				if(NULL != (pNetBuffer = CONTAINING_RECORD(pOverlapped, CNetBuffer, m_sttIOOverLapped)))
				{
					/// ����IO��Ϣ
					i_ProcessIOMessage(pContextStruct, pNetBuffer, dwIoSize);
				}
			}

			/// ������Ӳ�������ȡIOCP��ֹͣ
			if((NULL == pContextStruct && NULL == pNetBuffer) || false == m_bNetRunStatus)
			{
				/// ��ֵֹͣѭ����־
				bError = true;
			}
		}
	}
	catch(...)
	{
		e_PrintLog("�˴����쳣...\r\n");
	}
	END_DEBUG_INFO
}

/// �����������׽���
bool CNetIOCP::i_SetNewSocketOption(SOCKET sSocket)
{
	START_DEBUG_INFO
	bool bRet = false;
	/// ��֤���ݺϷ���
	if(INVALID_SOCKET == sSocket)
	{
		END_DEBUG_INFO
		return bRet;
	}
	END_DEBUG_INFO
	return bRet;
}

bool CNetIOCP::i_PostUDPRecvIssue(USHORT usNum)
{
	START_DEBUG_INFO
	bool bRet = false;
	/// ��֤
	if(0 == usNum)
	{
		END_DEBUG_INFO
		return bRet;
	}

	/// ��֤���ݺϷ���
	if(INVALID_SOCKET != m_sUDPServerSocket && NULL != m_pUDPSeverContext)
	{
		do 
		{
			/// Ͷ�ݽ��յ�IO��ɶ˿�
			if(true == i_PostUDPRecvIssue())
			{
				/// ����
				usNum--;
			}
		}while(0 < usNum);
		bRet = true;
	}

	END_DEBUG_INFO
	return bRet;
}

bool CNetIOCP::i_PostUDPRecvIssue(CNetBuffer* pNetBuffer)
{
	START_DEBUG_INFO
	bool bRet = false;
	/// ��֤���ݺϷ���
	if(NULL == pNetBuffer)
	{
		m_CBufferManager.e_AllocateBuffer(pNetBuffer, IOPT_UDPRECV);
	}

	if(NULL != pNetBuffer)
	{
		/// ��֤���ݺϷ���
		if(INVALID_SOCKET != m_sUDPServerSocket && NULL != m_pUDPSeverContext)
		{
			ULONG ulBytes = 0;
			ULONG ulFlags = 0;
			memset(&pNetBuffer->m_sttUDPAddrInfo, 0x00, sizeof(struct sockaddr_in));
			memset(&pNetBuffer->m_sttIOOverLapped, 0x00, sizeof(WSAOVERLAPPED));
			/// = ���ͽ���operation����ɶ˿ڶ�����,�˴�һ�����첽������
			/// = �˴��� lpFromLen ���� ����pNetBuffer->m_nAddressLen �����������ڶ������IO������ͷţ�
			/// = ���Դ˲�������Ϊ NetBuffer �ĳ�Ա����
			int nErrorCode = WSARecvFrom(
				m_sUDPServerSocket, 
				pNetBuffer->e_GetWSABuffer(),
				1,
				&ulBytes,
				&ulFlags,
				(struct sockaddr*)&pNetBuffer->m_sttUDPAddrInfo,
				&pNetBuffer->m_nAddressLen,
				&pNetBuffer->m_sttIOOverLapped,
				NULL);
			/// ��ֵ����ֵ
			bRet = true;
			/// ���Ͷ�ݴ���
			if(SOCKET_ERROR == nErrorCode && WSA_IO_PENDING != WSAGetLastError())
			{
				m_CBufferManager.e_ReleaseBuffer(pNetBuffer);
				bRet = false;
			}
		}
		else
		{
			m_CBufferManager.e_ReleaseBuffer(pNetBuffer);
		}
	}
	END_DEBUG_INFO
	return bRet;
}

bool CNetIOCP::i_PostUDPSendIssue(const char* pszClientIP, USHORT usClientPort, CNetBuffer* pSendBuffer)
{
	START_DEBUG_INFO
	bool bRet = false;
	/// ��֤���ݺϷ���
	if(INVALID_SOCKET != m_sUDPServerSocket && NULL != m_pUDPSeverContext)
	{
		ULONG ulBytes = 0;
		ULONG ulFlags = 0;
		pSendBuffer->m_sttUDPAddrInfo.sin_family = AF_INET;
		pSendBuffer->m_sttUDPAddrInfo.sin_addr.s_addr = inet_addr(pszClientIP);
		pSendBuffer->m_sttUDPAddrInfo.sin_port = htons(usClientPort);
		memset(&pSendBuffer->m_sttIOOverLapped, 0x00, sizeof(WSAOVERLAPPED));
		/// ���ͽ���operation����ɶ˿ڶ�����,�˴�һ�����첽����
		int nErrorCode = WSASendTo(
			m_sUDPServerSocket, 
			pSendBuffer->e_GetWSABuffer(),
			1,
			&ulBytes,
			ulFlags,
			(struct sockaddr*)&pSendBuffer->m_sttUDPAddrInfo,
			pSendBuffer->m_nAddressLen,
			&pSendBuffer->m_sttIOOverLapped,
			NULL);
		/// ��ֵ����ֵ
		bRet = true;
		/// ���Ͷ�ݴ���
		if(SOCKET_ERROR == nErrorCode && WSA_IO_PENDING != WSAGetLastError())
		{
			m_CBufferManager.e_ReleaseBuffer(pSendBuffer);
			bRet = false;
		}
	}
	else
	{
		m_CBufferManager.e_ReleaseBuffer(pSendBuffer);
	}
	END_DEBUG_INFO
	return bRet;
}

bool CNetIOCP::i_ProcessIOMessage(T_ClientContext* pClientContext, CNetBuffer* pNetBuffer, int nDataLen)
{
	START_DEBUG_INFO
	bool bRet = false;
	/// ��ȡIO��Ϣ����
	USHORT usIOType = pNetBuffer->e_GetOperation();
	/// ��Ϣ���Ͷ�Ӧ����
	switch(usIOType)
	{
	case IOPT_TACCEPT:
		{
			/// �����ʼ������
			i_OnTCPAccept(pClientContext, pNetBuffer, nDataLen);
			break;
		}
	case IOPT_TCPRECV:
		{
			/// ����TCP����׼��
			i_OnTCPRecv(pClientContext, pNetBuffer);
			break;
		}
	case IOPT_TRECVED:
		{
			/// ����TCP�������
			i_OnTCPRecved(pClientContext, pNetBuffer, nDataLen);
			break;
		}
	case IOPT_TCPSEND:
		{
			/// ����TCP����׼��
			i_OnTCPSend(pClientContext, pNetBuffer, nDataLen);
			break;
		}
	case IOPT_TSENDED:
		{
			/// ����TCP�������			
			i_OnTCPSended(pClientContext, pNetBuffer, nDataLen);
			break;
		}
	case IOPT_UDPRECV:
		{
			/// ����UDP����׼��
			i_OnUDPRecv(pClientContext, pNetBuffer, nDataLen);
			break;
		}
	case IOPT_UDPSEND:
		{
			/// ����UDP����׼��
			i_OnUDPSend(pClientContext, pNetBuffer, nDataLen);
			break;
		}
	default:
		{
			/// �ͷŻ������ݶ���
			m_CBufferManager.e_ReleaseBuffer(pNetBuffer);
			/// �ͷ�������Ϣ����
			m_ContextManager.e_ReleaseConntext(pClientContext);
			break;
		}
	}
	END_DEBUG_INFO
	return bRet;
}

void CNetIOCP::i_OnTCPAccept(T_ClientContext* pClientContext, CNetBuffer* pNetBuffer, int nDataLen)
{
	START_DEBUG_INFO
	/// ����������
	e_NotifyClientConntext(pClientContext);
	/// Ͷ�ݽ�������IO��������ɶ˿�
	for(int i = 0; i < m_sttNetInitStruct.usPendReadsNum; i++)
	{
		/// ��ȡ���Ӽ�����Ͷ�ݽ�����Ϣ����ɶ˿�
		i_AIOProcess(m_ContextManager.e_GetConntext(pClientContext->ulContextID), IOPT_TCPRECV);
	}
	/// �ͷ�һ�����Ӽ���
	m_ContextManager.e_ReleaseConntext(pClientContext);
	/// �ͷ����ӻ������
	m_CBufferManager.e_ReleaseBuffer(pNetBuffer);
	END_DEBUG_INFO
}

void CNetIOCP::i_OnTCPRecv(T_ClientContext* pClientContext, CNetBuffer* pNetBuffer)
{
	START_DEBUG_INFO
	/// ���û�����������
	pNetBuffer->e_SetOperation(IOPT_TRECVED);
	/// �����˳���ȡ״̬Ϊ��
	if(TRUE == m_sttNetInitStruct.bOrderRead)
	{
		i_MakeOrderdRead(pClientContext, pNetBuffer);
	}
	else
	{
		/// ��Ҫ����
		DWORD dwIoSize = 0;
		ULONG ulFlags = MSG_PARTIAL;
		/// ���ý�������
		UINT nRetVal = WSARecv(pClientContext->sSocketID, 
			pNetBuffer->e_GetWSABuffer(), 1,
			&dwIoSize, &ulFlags,
			&pNetBuffer->m_sttIOOverLapped, NULL);
		/// ���ý�������ʧ��
		if(SOCKET_ERROR == nRetVal && WSA_IO_PENDING != WSAGetLastError()) 
		{
			/// �ͷŻ���
			m_CBufferManager.e_ReleaseBuffer(pNetBuffer);
			/// �˿�����
			e_DisConectionConntext(pClientContext);
		}
	}
	END_DEBUG_INFO
}

void CNetIOCP::i_OnTCPRecved(T_ClientContext* pClientContext, CNetBuffer* pNetBuffer, int nDataLen)
{
	START_DEBUG_INFO
	/// ������յ�����Ϊ0, �Ͽ�����
	if(0 == nDataLen)
	{
		m_CBufferManager.e_ReleaseBuffer(pNetBuffer);
		/// �Ͽ����Ӷ���
		e_DisConectionConntext(pClientContext);
		return;
	}
	/// ���ӿ�ʹ�õĻ�������
	pNetBuffer->e_AddUsed(nDataLen);
	/// ��������״̬
	if(true == m_sttNetInitStruct.bOrderRead)
	{
		/// ��ȡ���ݻ������
		pNetBuffer = i_GetNextReadBuffer(pClientContext, pNetBuffer);
	}

	/// ���ݶ������
	while(NULL != pNetBuffer)
	{
		/// ����������ݰ�
		i_ProcessRecvPackage(pClientContext, pNetBuffer);
		/// ���ý���ҵ��ǰ���е���
		pClientContext->e_InCreaseCurReadSeqNum();
		/// ���û�������
		pNetBuffer = NULL;
		/// ��������״̬
		if(true == m_sttNetInitStruct.bOrderRead)
		{
			/// ��ȡ���ݻ������
			pNetBuffer = i_GetNextReadBuffer(pClientContext);
		}
	}

	if(NULL != pClientContext)
	{
		/// ����ɶ˿���һ���첽��Ϣ��ȡ
		i_AIOProcess(pClientContext, IOPT_TCPRECV);
	}
	END_DEBUG_INFO
}

CNetBuffer* CNetIOCP::i_GetNextReadBuffer(T_ClientContext* pContextStruct, CNetBuffer* pNetBuffer)
{
	START_DEBUG_INFO
	/// ��֤���ӺϷ���
	if(NULL == pContextStruct)
	{
		END_DEBUG_INFO
		return NULL;
	}
	/// ����Hash����
	CBufferMapToPtr::iterator ItemPos;
	/// ������Ϣ����
	CAutoLock AutoLock(&pContextStruct->ContextCritSec);
	/// ���巵����ʱ�������
	CNetBuffer* pRetBuffer = NULL;
	/// �������ݴ���
	if(NULL != pNetBuffer)
	{
		/// ��ȡ�������к�
		ULONG ulBufferSeqNum = pNetBuffer->e_GetBufferSeqNum();
		/// �������к�Ϊ��ǰ���ӻ������к�
		if(ulBufferSeqNum == pContextStruct->usCurReadSeqNum)
		{
			/// ���ص�ǰ����
			END_DEBUG_INFO
			return pNetBuffer;
		}
		/// ������յ��Ļ����б��в����ڸ����ж���Ļ������
		ItemPos = pContextStruct->CReadBufferMap.find(ulBufferSeqNum);
		if(ItemPos == pContextStruct->CReadBufferMap.end())
		{
			/// ������ڽ��յ��Ļ����б��м���ö����б�
			pContextStruct->CReadBufferMap[ulBufferSeqNum] = pNetBuffer;
		}
	}

	/// ������յ��Ļ����б��в����ڸ����ж���Ļ������
	ItemPos = pContextStruct->CReadBufferMap.find(pContextStruct->usCurReadSeqNum);
	if(ItemPos != pContextStruct->CReadBufferMap.end())
	{
		pRetBuffer = ItemPos->second;
		pContextStruct->CReadBufferMap.erase(ItemPos);
	}
	/// ���ص�ǰ��������
	END_DEBUG_INFO
	return pRetBuffer;
}

CNetBuffer* CNetIOCP::i_GetNextSendBuffer(T_ClientContext* pContextStruct, CNetBuffer* pNetBuffer)
{
	START_DEBUG_INFO;
	/// ��֤���ӺϷ���
	if(NULL == pContextStruct)
	{
		END_DEBUG_INFO
		return NULL;
	}
	/// ����Hash����
	CBufferMapToPtr::iterator ItemPos;
	/// ����������
	CAutoLock AutoLock(&pContextStruct->ContextCritSec);
	/// ������ʱ����
	CNetBuffer* pRetBuffer = NULL;
	/// ������岻Ϊ��
	if(NULL != pNetBuffer)
	{ 
		/// ��ȡ�����������к�
		ULONG ulBufferSeqNum = pNetBuffer->e_GetBufferSeqNum();
		/// ��֤�����к�
		if(ulBufferSeqNum == pContextStruct->usCurSendSeqNum)
		{
			/// ���ػ���
			END_DEBUG_INFO
			return pNetBuffer;
		}
		/// ������յ��Ļ����б��в����ڸ����ж���Ļ������
		ItemPos = pContextStruct->CSendBufferMap.find(ulBufferSeqNum);
		if(ItemPos == pContextStruct->CSendBufferMap.end())
		{
			/// ������ڷ��͵Ļ����б��м���ö����б�
			pContextStruct->CSendBufferMap[ulBufferSeqNum] = pNetBuffer;
		}
	}

	/// ������յ��Ļ����б��в����ڸ����ж���Ļ������
	ItemPos = pContextStruct->CSendBufferMap.find(pContextStruct->usCurSendSeqNum);
	if(ItemPos != pContextStruct->CSendBufferMap.end())
	{
		pRetBuffer = ItemPos->second;
		pContextStruct->CSendBufferMap.erase(ItemPos);
	}
	/// ���ص�ǰ��������
	END_DEBUG_INFO
	return pRetBuffer;
}

void CNetIOCP::i_ProcessRecvPackage(T_ClientContext*& pContextStruct, CNetBuffer*& pNetBuffer)
{
	START_DEBUG_INFO
	/// �����ϴ�δ������ɵ����ݽ���
	if(false == i_ProcessPreviouRecv(pContextStruct, pNetBuffer))
	{
		/// ��֤���Ӷ���ͻ������Ϸ���
		if(NULL != pContextStruct && NULL != pNetBuffer)
		{
			/// ����˴����ݽ���
			i_ProcessCurrentRecv(pContextStruct, pNetBuffer);
		}
	}
	END_DEBUG_INFO
}

bool CNetIOCP::i_ProcessPreviouRecv(T_ClientContext*& pContextStruct, CNetBuffer*& pNetBuffer)
{
	START_DEBUG_INFO
	/// ���巵��ֵ
	bool bRet = false;
	/// ������ʱ����������ݻ������
	CNetBuffer* pTempBuffer = pContextStruct->pCurrentBuffer;
	/// ������Ҫ��Buffer��С
	UINT unHowMuchIsNeeded = 0;
	UINT unUsedBuffer = 0;
	/// �����ȡ��С����
	UINT unDataSize = 0;
	/// ��֤���ݰ��Ϸ���
	if(NULL != pTempBuffer)
	{
		/// ��ȡ��������ʹ�ô�С
		unUsedBuffer = pTempBuffer->e_GetUsed();
		/// ��֤ʹ�����ݴ�С��С����С�����ݣ�
		if(MIN_NET_BUFFER_SIZE > unUsedBuffer)
		{
			/// ��ȡ��Ҫ�����ݴ�С
			unHowMuchIsNeeded = MIN_NET_BUFFER_SIZE - unUsedBuffer;
			/// ����ʹ�õĴ�С������������
			if(unHowMuchIsNeeded > pNetBuffer->e_GetUsed())
			{ 
				/// �������̫С, �ƶ����嵽���Ӷ��󻺳���ͷŶ���
				pTempBuffer->e_AddMoveOtherBuffer(pNetBuffer, pNetBuffer->e_GetUsed());
				/// ���������ڣ��ͷ�û�б�ʹ�õĻ���
				m_CBufferManager.e_ReleaseBuffer(pNetBuffer);
				/// ���صȴ����������
				bRet = true;
				END_DEBUG_INFO
				return bRet;
			}
			else
			{
				/// �ƶ��������ݵ����Ӷ��󻺳�
				pTempBuffer->e_AddMoveOtherBuffer(pNetBuffer, unHowMuchIsNeeded);
			}
		}

		/// ���������Ѿ�������ͷ���ȴ�С�������ˣ����ǻ�ȡ����
		unUsedBuffer = pTempBuffer->e_GetUsed();
		/// �ƶ�����
		memmove(&unDataSize, pTempBuffer->e_GetBuffer(), MIN_NET_BUFFER_SIZE);
		/// ���ݴ�С���С�ڵ��������Ч������
		if(MAX_PAL_BUFFER_SIZE >= unDataSize)
		{
			/// ��ȡ��Ҫ�����ݴ�С
			unHowMuchIsNeeded = unDataSize - (unUsedBuffer - MIN_NET_BUFFER_SIZE);
			/// �������ֻ��Ҫһ�����ݣ�������������Ҫ��С����
			if(unHowMuchIsNeeded <= pNetBuffer->e_GetUsed())
			{
				/// ����һ����С�����ݵ����ӻ���
				pTempBuffer->e_AddMoveOtherBuffer(pNetBuffer, unHowMuchIsNeeded);
				/// ������հ�����
				e_NotifyReceivedPackage(NDT_TCPData, pContextStruct, pTempBuffer);
				/// �ͷ����ݻ���
				m_CBufferManager.e_ReleaseBuffer(pContextStruct->pCurrentBuffer);
				/// ���õ�ǰ����Ļ���
				pContextStruct->pCurrentBuffer = NULL;
				/// ������ݻ��������ݰ��ոյ����������ݴ�С
				if(0 == pNetBuffer->e_GetUsed())
				{
					/// �ͷ����ݻ���
					m_CBufferManager.e_ReleaseBuffer(pNetBuffer);
					/// �Ѵ������
					bRet = true;
				}
			}
			else
			{
				/// ����һ����С������
				pTempBuffer->e_AddMoveOtherBuffer(pNetBuffer, pNetBuffer->e_GetUsed());
				/// ���������ڣ��ͷ�û�б�ʹ�õĻ���
				m_CBufferManager.e_ReleaseBuffer(pNetBuffer);
				/// ���صȴ����������
				bRet = true;
				END_DEBUG_INFO
				return bRet;
			}
		}
		else
		{
			/// �ͷ����ݻ���
			m_CBufferManager.e_ReleaseBuffer(pNetBuffer);
			m_CBufferManager.e_ReleaseBuffer(pContextStruct->pCurrentBuffer);
			pContextStruct->pCurrentBuffer = NULL;
			/// �Ͽ�����
			e_DisConectionConntext(pContextStruct);
			/// ֱ�ӷ���
			bRet = true;
			END_DEBUG_INFO
			return bRet;
		}
	}
	END_DEBUG_INFO
	return bRet;
}

void CNetIOCP::i_ProcessCurrentRecv(T_ClientContext*& pContextStruct, CNetBuffer*& pNetBuffer)
{
	START_DEBUG_INFO
	/// ������ʱ����������ݻ������
	CNetBuffer* pTempBuffer = NULL;
	/// ������Ҫ��Buffer��С
	UINT unHowMuchIsNeeded = 0;
	UINT unUsedBuffer = 0;
	/// �����ȡ��С����
	UINT unDataSize = 0;
	/// ����ѭ������
	bool bDone = false;
	/// ��ʼѭ����ȡ����
	do 
	{
		/// ��ֵѭ������
		bDone = true;
		/// ��ȡ����ʹ�ô�С
		unUsedBuffer = pNetBuffer->e_GetUsed();
		/// �����С
		if(MIN_NET_BUFFER_SIZE <= unUsedBuffer)
		{
			/// ��ȡ���ݰ���С
			memmove(&unDataSize, pNetBuffer->e_GetBuffer(), MIN_NET_BUFFER_SIZE);
			/// ����������İ�
			if(unDataSize == unUsedBuffer - MIN_NET_BUFFER_SIZE)
			{
				/// ����������ݰ�
				e_NotifyReceivedPackage(NDT_TCPData, pContextStruct, pNetBuffer);
				/// �ͷ����ݶ���
				m_CBufferManager.e_ReleaseBuffer(pNetBuffer);
				/// ��ֵѭ������, �˳�ѭ��
				bDone = true;
			}
			else if(unDataSize < unUsedBuffer - MIN_NET_BUFFER_SIZE)
			{
				/// ��ʹ�õ����ݴ���Ҫ���õ����ݴ�С ��������
				if(true == m_CBufferManager.e_SplitNetBuffer(pNetBuffer, pTempBuffer, unDataSize + MIN_NET_BUFFER_SIZE))
				{
					/// �������ݰ�
					e_NotifyReceivedPackage(NDT_TCPData, pContextStruct, pTempBuffer);
					/// �ͷ�����
					m_CBufferManager.e_ReleaseBuffer(pTempBuffer);
					/// ����ѭ����ȡ���ݣ���Ϊ����δ����ֳ���������û�н���
					bDone = false;
				}
			}
			else if(unDataSize > unUsedBuffer - MIN_NET_BUFFER_SIZE && MAX_PAL_BUFFER_SIZE > unDataSize)
			{
				/// ���ݻ����С�� ʹ�����ݺ�������ݰ�֮��
				/// ��ֵ���ݰ������������ݻ������
				pContextStruct->pCurrentBuffer = pNetBuffer;
				/// ���û������ݰ�
				pNetBuffer = NULL;
				/// ��ֵѭ������, �˳�ѭ��
				bDone = true;
			}
			else
			{
				/// = �˴�ֻ��һ����������ݴ�С���ڻ�����Ԥ��Ĵ�С��
				/// = ��Ϊ���Ϸ� (unDataSize > MAX_PAL_BUFFER_SIZE)
				/// �ͷŻ������ݶ���
				m_CBufferManager.e_ReleaseBuffer(pNetBuffer);
				/// �Ͽ�����
				e_DisConectionConntext(pContextStruct);
				/// ��ֵѭ������, �˳�ѭ��
				bDone = true;
			}
		}
		else if(0 < unUsedBuffer)
		{
			/// ����̫С��ֵ�������ݵ����ӻ������
			pContextStruct->pCurrentBuffer = pNetBuffer;
			/// ���ô���Ļ������
			pNetBuffer = NULL;
			/// ��ֵѭ����־��ֹͣѭ��
			bDone = true;
		}
	}while(false == bDone);
	END_DEBUG_INFO
}

void CNetIOCP::i_OnTCPSend(T_ClientContext* pClientContext, CNetBuffer* pNetBuffer, int nDataLen)
{
	START_DEBUG_INFO
	/// ��֤���ӺϷ���
	if(NULL != pClientContext && INVALID_SOCKET != pClientContext->sSocketID)
	{
		/// ���÷���״̬Ϊ��
		if(true == m_sttNetInitStruct.bOrderSend)
		{
			/// ��ȡ���ͻ������
			pNetBuffer = i_GetNextSendBuffer(pClientContext, pNetBuffer);
		}

		/// ��������������Ϣ
		pClientContext->ContextCritSec.e_Lock();
		/// ����������
		while(NULL != pNetBuffer)
		{
			/// ���ò���״̬
			pNetBuffer->e_SetOperation(IOPT_TSENDED);
			/// ����������
			ULONG ulFlags = MSG_PARTIAL;
			DWORD dwSendNumBytes = 0;

			/// ���÷���״̬
			int nRetVal = WSASend(pClientContext->sSocketID,
				pNetBuffer->e_GetWSABuffer(),
				1,
				&dwSendNumBytes, 
				ulFlags,
				&pNetBuffer->m_sttIOOverLapped,
				NULL);
			/// ���ӷ��͵�ǰ��������к�
			pClientContext->e_InCreaseCurSendSeqNum();
			/// ����״̬ʧ��
			if(SOCKET_ERROR == nRetVal && WSA_IO_PENDING != WSAGetLastError())
			{
				/// �ͷŻ�������
				m_CBufferManager.e_ReleaseBuffer(pNetBuffer);
				/// �������ݻ���
				pNetBuffer = NULL;
				/// �Ͽ�����
				e_DisConectionConntext(pClientContext);
			}
			else
			{
				/// �������ݻ���
				pNetBuffer = NULL;
				/// ��֤����״̬
				if(TRUE == m_sttNetInitStruct.bOrderSend)
				{
					/// ��ȡ���͵Ļ������
					pNetBuffer = i_GetNextSendBuffer(pClientContext);
				}
			}
		}
		/// ������Ϣ���ڣ���������
		if(NULL != pClientContext)
		{
			pClientContext->ContextCritSec.e_Unlock();
		}
	}
	else
	{
		/// �ͷŻ������
		m_CBufferManager.e_ReleaseBuffer(pNetBuffer);
		/// �ͷ����Ӷ���
		m_ContextManager.e_ReleaseConntext(pClientContext);
	}
	END_DEBUG_INFO
}

void CNetIOCP::i_OnTCPSended(T_ClientContext* pClientContext, CNetBuffer* pNetBuffer, int nDataLen)
{
	START_DEBUG_INFO
	/// ������Ϣ�Ϸ�
	if(NULL != pClientContext)
	{
		/// �������ݶ���Ϸ�
		if(NULL != pNetBuffer)
		{
			/// ��������ʹ�ô�С���ڶ˿ڻ�ȡ�Ĵ�С
			if(nDataLen == pNetBuffer->e_GetUsed())
			{
				/// ���������
				e_NotifyWriteCompleted(NDT_TCPData, pClientContext->ulContextID,
					pClientContext->szClientIP, pClientContext->usClientPort, 
					pNetBuffer->e_GetBuffer(), nDataLen);
			}
			/// �ͷŻ�������
			m_CBufferManager.e_ReleaseBuffer(pNetBuffer);
		}
		/// �ͷ�������Ϣ
		m_ContextManager.e_ReleaseConntext(pClientContext);
	}
	END_DEBUG_INFO
}

void CNetIOCP::i_OnUDPRecv(T_ClientContext* pClientContext, CNetBuffer* pNetBuffer, int nDataLen)
{
	START_DEBUG_INFO
	/// ��֤���ݺϷ���
	if(0 >= nDataLen)
	{
		END_DEBUG_INFO
		return;
	}
	/// ����������ݴ���
	if(NULL != pNetBuffer)
	{
		/// ����ʹ�����ݴ�С
		pNetBuffer->e_AddUsed(nDataLen);
		/// ִ�лص�����
		if(NULL != m_pFunRecvData)
		{
			m_pFunRecvData(NDT_UDPData, 0/*pClientContext->ulContextID*/,
				inet_ntoa(pNetBuffer->m_sttUDPAddrInfo.sin_addr),
				ntohs(pNetBuffer->m_sttUDPAddrInfo.sin_port),
				pNetBuffer->e_GetBuffer(), nDataLen);
		}
		/// �ͷ����ݻ���
		pNetBuffer->e_ReBuffValue();
		/// Ͷ�ݽ������ݵ��ͻ���
		i_PostUDPRecvIssue(pNetBuffer);
		/// �ͷ����ݻ���
		/// m_CBufferManager.e_ReleaseBuffer(pNetBuffer);
	}
	/// Ͷ�ݽ��յ���ɶ˿�
	/// i_PostUDPRecvIssue();
	END_DEBUG_INFO
}

void CNetIOCP::i_OnUDPSend(T_ClientContext* pClientContext, CNetBuffer* pNetBuffer, int nDataLen)
{
	START_DEBUG_INFO
	/// ����������ݴ���
	if(NULL != pNetBuffer)
	{
		/// �����������֪ͨ
		e_NotifyWriteCompleted(NDT_UDPData, pClientContext->ulContextID,
			inet_ntoa(pNetBuffer->m_sttUDPAddrInfo.sin_addr),
			ntohs(pNetBuffer->m_sttUDPAddrInfo.sin_port),
			pNetBuffer->e_GetBuffer(), nDataLen);
		
		/// �ͷŽ�������
		m_CBufferManager.e_ReleaseBuffer(pNetBuffer);
	}
	END_DEBUG_INFO
}

bool CNetIOCP::i_AIOProcess(T_ClientContext* pClientContext, USHORT usIOType, CNetBuffer* pNetBuffer)
{
	START_DEBUG_INFO
	/// ���巵��ֵ
	bool bRet = false;
	/// ��֤�����Ƿ�Ϸ�
	if(NULL == pNetBuffer)
	{
		m_CBufferManager.e_AllocateBuffer(pNetBuffer, usIOType);
	}
	if(NULL != pNetBuffer)
	{
		bRet = true;
		/// Ͷ��IO�¼�����ɶ˿�
		BOOL bSuccess = PostQueuedCompletionStatus(m_hCompletionPort, 0,
			(DWORD)pClientContext, &pNetBuffer->m_sttIOOverLapped);
		/// ��֤��ȷ��
		if(FALSE == bSuccess && WSA_IO_PENDING != WSAGetLastError())
		{
			bRet = false;
			/// �Ƴ����ݻ���
			m_CBufferManager.e_ReleaseBuffer(pNetBuffer);
			/// �Ƴ��ͻ���������Ϣ
			e_DisConectionConntext(pClientContext);
		}
	}
	else
	{
		/// �Ƴ��ͻ���������Ϣ
		m_ContextManager.e_ReleaseConntext(pClientContext);
	}
	END_DEBUG_INFO
	return bRet;
}

void CNetIOCP::i_MakeOrderdRead(T_ClientContext* pClientContext, CNetBuffer*& pNetBuffer)
{
	START_DEBUG_INFO
	/// �����б���ΪҪ��֤���ɵ�˳�����ȷ
	pClientContext->ContextCritSec.e_Lock();
	/// ��Ҫ����
	DWORD dwIoSize = 0;
	ULONG ulFlags = MSG_PARTIAL;
	/// ���ý���ҵ�����кŵ��������
	pNetBuffer->e_SetBufferSeqNum(pClientContext->usReadSeqNum);
	/// ���ý�������
	UINT nRetVal = WSARecv(pClientContext->sSocketID, 
		pNetBuffer->e_GetWSABuffer(), 1,
		&dwIoSize, &ulFlags,
		&pNetBuffer->m_sttIOOverLapped, NULL);
	/// ���ý�������ʧ��
	if(SOCKET_ERROR == nRetVal && WSA_IO_PENDING != WSAGetLastError()) 
	{
		/// �ͷŻ���
		m_CBufferManager.e_ReleaseBuffer(pNetBuffer);
		/// �˿�����
		e_DisConectionConntext(pClientContext);
	}
	else
	{
		/// ���ӽ���ҵ�����
		pClientContext->e_AddReadSeqNum();
	}

	/// ��֤���ݺϷ���
	if(NULL != pClientContext)
	{
		/// �����б�
		pClientContext->ContextCritSec.e_Unlock();
	}
	END_DEBUG_INFO
}

bool CNetIOCP::e_NotifyClientConntext(T_ClientContext* pClientContext)
{
	START_DEBUG_INFO
	bool bRet = false;
	/// ��֤���ݺϷ���
	if(NULL == pClientContext)
	{
		END_DEBUG_INFO
		return bRet;
	}

	/// �ص�֪ͨ�ͻ��˳ɹ�����
	if(NULL != m_pFunConection)
	{
		/// ��ȡ�ͻ���IP�Ͷ˿���Ϣ
		if(true == e_GetIPAddrBySocket(pClientContext->sSocketID, pClientContext->szClientIP, 
			sizeof(pClientContext->szClientIP), pClientContext->usClientPort))
		{
			m_pFunConection(pClientContext->ulContextID, pClientContext->szClientIP,
				pClientContext->usClientPort);
		}
	}
	END_DEBUG_INFO
	return true;
}

bool CNetIOCP::e_DisConectionConntext(T_ClientContext*& pClientContext)
{
	START_DEBUG_INFO
	bool bRet = false;
	/// ��֤���ݺϷ���
	if(NULL == pClientContext)
	{
		END_DEBUG_INFO
		return bRet;
	}

	/// �б����
	pClientContext->ContextCritSec.e_Lock();
	/// �ر��׽�������
	if(INVALID_SOCKET != pClientContext->sSocketID && NULL != pClientContext->sSocketID)
	{
		/// �ص�֪ͨ�ͻ��˶Ͽ�����
		if(NULL != m_pFunDisConection)
		{
			m_pFunDisConection(pClientContext->ulContextID, pClientContext->szClientIP,
				pClientContext->usClientPort);
		}
		/// �ͷ��׽���
		e_CloseSocket(pClientContext->sSocketID);
	}
	pClientContext->ContextCritSec.e_Unlock();
	/// �ͷ���������
	m_ContextManager.e_ReleaseConntext(pClientContext);
	END_DEBUG_INFO
	return true;
}

void CNetIOCP::e_NotifyReceivedPackage(USHORT usNetType, T_ClientContext* pContextStruct, CNetBuffer* pNetBuffer)
{
	START_DEBUG_INFO
	/// ��֤���ݺϷ���
	if(NULL == pContextStruct || NULL == pNetBuffer)
	{
		END_DEBUG_INFO;
		return;
	}

	if(NULL != m_pFunRecvData)
	{
		/// �������ݵ��ͻ���
		m_pFunRecvData(usNetType, pContextStruct->ulContextID,
			pContextStruct->szClientIP, pContextStruct->usClientPort,
			pNetBuffer->e_GetBuffer(), pNetBuffer->e_GetUsed());
	}
	END_DEBUG_INFO
}

void CNetIOCP::e_NotifyWriteCompleted(USHORT usNetType, ULONG ulContextID, const char* pszClientIP,
	USHORT usClientPort, PBYTE pSendBuffer, UINT ulDataLen)
{
	START_DEBUG_INFO
	/// ��֤���ݺϷ���
	if(0 >= usNetType || 0 >= ulContextID || NULL == pszClientIP || 
		0 >= usClientPort || NULL == pSendBuffer || 0 >= ulDataLen)
	{
		END_DEBUG_INFO;
		return;
	}

	if(NULL != m_pFunSendData)
	{
		/// �������֪ͨ�ͻ���
		m_pFunSendData(usNetType, ulContextID, pszClientIP,
			usClientPort, pSendBuffer, ulDataLen);
	}
	END_DEBUG_INFO
}

bool CNetIOCP::e_GetIPAddrBySocket(SOCKET sClientSocket, char* pszClientIP, USHORT usStrSize, USHORT& usClientPort)
{
	START_DEBUG_INFO
	bool bRet = false;
	/// ��֤���ݺϷ���
	if(INVALID_SOCKET == sClientSocket || NULL == pszClientIP || 0 >= usStrSize)
	{
		END_DEBUG_INFO
		return bRet; 
	}
	/// ��ȡָ���ͻ���IP��ַ
	sockaddr_in sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));
	int nSockAddrLen = sizeof(sockAddr);
	/// ��ȡ����ֵ
	int nResult = getpeername(sClientSocket, (struct sockaddr *)&sockAddr, &nSockAddrLen);
	/// �����ȡ�ɹ�
	if(0 == nResult)
	{
		/// ��ֵIP
		strncpy_s(pszClientIP, usStrSize, inet_ntoa(sockAddr.sin_addr), 16);
		/// ��ֵ�˿ں�
		usClientPort = ntohs(sockAddr.sin_port);
		/// ��ֵ����
		bRet = true;
	}
	END_DEBUG_INFO
	return bRet;
}

bool CNetIOCP::e_SendDataToTCPClient(ULONG ulContextID, BYTE* pSendData, int nDataLen)
{
	START_DEBUG_INFO
	bool bRet = false;
	/// ��֤�������ݺϷ���
	if(NULL == pSendData || 0 >= nDataLen || MAX_NET_BUFFER_SIZE < nDataLen)
	{
		END_DEBUG_INFO
		return bRet;
	}
	/// ��ȡ�ͻ���������Ϣ
	T_ClientContext* pClientContext = m_ContextManager.e_GetConntext(ulContextID);
	if(NULL != pClientContext)
	{
		if(INVALID_SOCKET != pClientContext->sSocketID && NULL != pClientContext->sSocketID)
		{
			CNetBuffer* pNetBuffer = NULL;
			/// ��ȡ
			if(true == m_CBufferManager.e_AllocateBuffer(pNetBuffer, IOPT_TCPSEND))
			{
				/// �������ݵ�����
				pNetBuffer->e_AddData(pSendData, nDataLen);
				/// �������ݶ���
				pClientContext->ContextCritSec.e_Lock();
				/// ���÷��ͻ����������к�
				pNetBuffer->e_SetBufferSeqNum(pClientContext->usSendSeqNum);
				/// ���ӷ���ҵ�����к�
				pClientContext->e_AddSendSeqNum();
				/// �������ݶ���
				pClientContext->ContextCritSec.e_Unlock();
				/// Ͷ��һ�����Ͳ�����IO
				bRet = i_AIOProcess(pClientContext, IOPT_TCPSEND, pNetBuffer);
			}
			else
			{
				/// �����ȡ��������ͷ�����
				m_ContextManager.e_ReleaseConntext(pClientContext);
			}
		}
		else
		{
			m_ContextManager.e_ReleaseConntext(pClientContext);
		}
	}
	END_DEBUG_INFO
	return bRet;
}

bool CNetIOCP::e_SendDataToUDPClient(const char* pszClientIP, USHORT usClientPort, BYTE* pSendData, int nDataLen)
{
	START_DEBUG_INFO
	bool bRet = false;
	/// ��֤���ݺϷ���
	if(NULL == pszClientIP || 0 >= usClientPort || NULL == pSendData 
		|| 0 >= nDataLen || MAX_NET_BUFFER_SIZE < nDataLen)
	{
		END_DEBUG_INFO
		return bRet;
	}

	CNetBuffer* pNetBuffer = NULL;
	if(true == m_CBufferManager.e_AllocateBuffer(pNetBuffer, IOPT_UDPSEND))
	{
		/// �������ݵ��������
		pNetBuffer->e_AddData(pSendData, nDataLen);
		/// ����UDP��Ϣ���ͻ���
		bRet = i_PostUDPSendIssue(pszClientIP, usClientPort, pNetBuffer);
	}
	END_DEBUG_INFO
	return bRet;
}

bool CNetIOCP::e_CloseTCPContext(ULONG ulContextID)
{
	START_DEBUG_INFO
	/// ���巵��ֵ
	bool bRet = false;
	/// ��ȡ������Ϣ
	T_ClientContext* pClientContext = m_ContextManager.e_GetConntext(ulContextID);
	if(NULL != pClientContext)
	{
		/// �Ͽ�����
		bRet = e_DisConectionConntext(pClientContext);
	}
	END_DEBUG_INFO
	return bRet;
}

void CNetIOCP::e_CloseSocket(SOCKET& sSocket)
{
	START_DEBUG_INFO
	/// ��֤���ݵĺϷ���
	if(INVALID_SOCKET == sSocket)
	{
		return;
	}
	/// ȡ��IO���׽��ֲ���, �����׽������÷�������
	CancelIo((HANDLE)sSocket);
	/// ֹͣ����
	shutdown(sSocket, SD_BOTH);
	/// �ͷ��׽�������
	closesocket(sSocket);
	/// �����׽���
	sSocket = SOCKET_ERROR;
	END_DEBUG_INFO
}

void CNetIOCP::e_PrintLog(const char* pszLog, ...)
{
	/// ��ʱ�ַ���
	char szTmp[1024] = {0};
	va_list argptr;
	va_start(argptr, pszLog);
	memset(szTmp, 0x00, sizeof(szTmp));
	vsnprintf_s(szTmp, sizeof(szTmp) - 1, pszLog, argptr);
	va_end(argptr);
	/// �ص���ӡ��־
	if(NULL != m_pFunPrintLog)
	{
		m_pFunPrintLog("%s", szTmp);
	}
	else
	{
		printf("%s", szTmp);
	}	
}