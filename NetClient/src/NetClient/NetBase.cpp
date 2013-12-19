////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 
/// @par ģ����
/// <����ũ���ͻ���ͨѶ����>
/// @par ����ļ�
/// NetSocket.h 
/// @par ������ϸ����
/// <����ũ���ͻ���ͨѶ����>
/// @par ���̰߳�ȫ��
/// <��/��>[��˵��]
/// @par �쳣ʱ��ȫ��
/// <��/��>[��˵��]
/// @note         -
/// 
/// @file         NetSocket.cpp
/// @brief        <ģ�鹦�ܼ���>
/// @author       Li.xl
/// @version      1.0
/// @date         2011/05/25
/// @todo         <����Ҫ��������>
/// 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "NetSocket.h"

namespace FWAYNET
{
	/// ������̬���õ�ȫ�������
	CNetBase* m_pCCNetSocket = NULL;
	CNetBase::CNetBase()
	{
		START_DEBUG_INFO
#ifdef WIN32
		/// ����Windows Sockets ��ʼ������
		WSADATA wsaData;
		/// ��ʼ���׽���Sockets��ȡ��ʼ��״̬
		m_nWSAInitResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
		/// ��ʼ�������߳�ID
		m_DRecvThreadId = NULL;
		/// ��ʼ�������߳�ID
		m_DSendThreadId = NULL;
#endif
		/// ��ʼ���׽���ID
		m_nSocketId = 0;
		/// ��ʼ��ֹͣ���ձ�־
		m_bStopRecvTag = false;
		/// ��ʼ�����ͽ��ձ�־
		m_bStopSendTag = false;
		/// ��ʼ��������IP
		memset(m_szServerIp, 0x00, sizeof(m_szServerIp));
		/// ��ʼ���������˿ں�
		m_nServerPort = 0;
		/// ��ʼ�����ӷ�����״̬
		m_bConnect = false;
		/// ��ʼ���ص�����
		m_pReceiveStructCallBack = NULL;
		/// ��ʼ����������
		m_SendBufferArray.clear();
		/// ��ʼ���ͻ�������
		m_nClientType = CT_NoneError;
		/// ��ֵȫ�������
		m_pCCNetSocket = this;
		/// ��ʼ����������
		m_nConnectType = 0;
		END_DEBUG_INFO
	}

	CNetBase::~CNetBase()
	{
		START_DEBUG_INFO
	#ifdef WIN32
		/// �׽���״̬Ϊ������Ͽ��׽���
		if(NO_ERROR == m_nWSAInitResult)
		{
			WSACleanup();
		}
	#endif
		/// ֹͣ�׽������ӣ�ֹͣ���ͺͽ����߳�
		e_StopReceive();
		e_StopSend();
		/// ��շ��͵Ļ�����������
		e_DestroySendBufferArray();
		END_DEBUG_INFO
	}

	bool CNetBase::e_IniSocket()
	{
		START_DEBUG_INFO
		/// �ر��׽���
		e_CloseSocket();
		/// ��ȡ�µ��׽���
		m_nSocketId = socket(AF_INET,SOCK_STREAM, 0);
		/// �����ȡ�׽��ֳɹ�,����
		if(-1 != m_nSocketId)
		{
			/// �׽���������ò����ڴ˴���
			e_SetSocket(m_nSocketId);
		}
		else
		{
			m_nSocketId = 0;
		}
		END_DEBUG_INFO
		return (0 < m_nSocketId);
	}

	void CNetBase::e_SetSocket(int nSocketId)
	{
		START_DEBUG_INFO
		/// ����׽��ֲ�����ֱ�ӷ���
		if(0 >= nSocketId)
		{
			END_DEBUG_INFO
			return;
		}

		BOOL bSopt = TRUE;
		int ret = 0;

		int nBufferSize = 1024 * 64;
		/// ������������
		setsockopt(nSocketId, IPPROTO_TCP, TCP_NODELAY, (char *)&bSopt, sizeof(BOOL));
#ifdef WIN32
		/// ����������ʱ����[60*1000] = 60s
		int timeout = 60000;
		ret = setsockopt(nSocketId, SOL_SOCKET, SO_SNDTIMEO, (const char *)&timeout, sizeof(timeout));
		/// ���÷��ͺͽ����׽��ֵĻ����С
		ret = setsockopt(nSocketId, SOL_SOCKET, SO_SNDBUF, (const char *)&nBufferSize, sizeof(int));
		ret = setsockopt(nSocketId, SOL_SOCKET, SO_RCVBUF, (const char *)&nBufferSize, sizeof(int));
#else
		/// ����������ʱ����60s
		struct timeval timeout = {60, 0};
		ret = setsockopt(nSocketId, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));
		nBufferSize = 1024 * 64;
		/// ���÷��ͺͽ����׽��ֵĻ����С
		ret = setsockopt(nSocketId, SOL_SOCKET, SO_SNDBUF, &nBufferSize, sizeof(int));
		ret = setsockopt(nSocketId, SOL_SOCKET, SO_RCVBUF, &nBufferSize, sizeof(int));
#endif
		END_DEBUG_INFO
	}

	void CNetBase::e_CloseSocket()
	{
		START_DEBUG_INFO
		/// ������Ӵ��ڹر�����
#ifdef WIN32
		if(0 < m_nSocketId)
		{
			closesocket(m_nSocketId);
		}
#else
		if(0 < m_nSocketId)
		{
			close(m_nSocketId);
		}
#endif
		/// �׽���ID��ʼ��Ϊ0
		m_nSocketId = 0;
		/// ����״̬����Ϊδ����
		m_bConnect = false;
		END_DEBUG_INFO
	}

	bool CNetBase::e_ConnectServer(char* pszServerIp, int nServerPort, int nClientType, int nConnectType)
	{
		START_DEBUG_INFO
		/// ���÷�����IP
		snprintf(m_szServerIp, sizeof(m_szServerIp), "%s", pszServerIp);
		/// ���÷���˿ں�
		m_nServerPort = nServerPort;
		/// ���ÿͻ�����������
		m_nClientType = nClientType;
		/// ������������
		m_nConnectType = nConnectType;
		/// ���ӷ��񣬷�������״̬
		END_DEBUG_INFO
		return e_ReconnectServer();
	}

	bool CNetBase::e_ReconnectServer()
	{
		START_DEBUG_INFO
		/// �˿ں�δ���÷���
		if(0 == m_nServerPort)
		{
			END_DEBUG_INFO
			return false;
		}
		/// �������״̬
		if(true == m_bConnect)
		{
			/// �ر��׽���
			e_CloseSocket();
		}
		/// ��շ��ͻ�����������
		e_DestroySendBufferArray();

		/// ɨ���������ط���
		if(0 == m_nConnectType)
		{
			/// ɨ�����ӷ�����
			if(false == e_ScanConnectServer())
			{
				/// ɨ��ʧ�ܷ���
				printf("ɨ�����ӷ�����ʧ��\r\n");
				END_DEBUG_INFO
				return false;
			}
		}

		/// ���ӷ���IpΪ���򷵻�
		if(0 == strlen(m_szServerIp))
		{
			return false;
		}

		struct sockaddr_in serverAddress;
		serverAddress.sin_family = AF_INET;
		serverAddress.sin_addr.s_addr = inet_addr(m_szServerIp);
		serverAddress.sin_port = htons(m_nServerPort);
		if(0 == m_nSocketId)
		{
			if(false == e_IniSocket())
			{
				END_DEBUG_INFO
				return false;
			}
		}
		/// ˯��20����
#ifdef WIN32
		Sleep(20);
#else
		usleep(20000);
#endif
		/// ���ӷ���������������״̬(-1 = ���Ӳ��ɹ�)
		m_bConnect = (connect(m_nSocketId, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) != -1);
		if(false == m_bConnect)
		{
			static int ntick = 0;
			ntick++;
			if(10 < ntick || 2 != m_nConnectType)
			{
				ntick = 0;
				printf("���ӷ�����IP = %s Port = %d ʧ��\r\n", m_szServerIp, m_nServerPort);
			}
			/// �״�����ָ��Ipʧ�ܣ��Զ���ֵΪɨ�����ӷ���
			if(1 == m_nConnectType)
			{
				m_nConnectType = 0;
			}
			/// ����ʧ�ܣ��ر��׽���
			e_CloseSocket();
		}
		else
		{
			if(true == e_InitClient())
			{
				printf("���ӷ�����IP = %s Port = %d �ɹ�\r\n", m_szServerIp, m_nServerPort);
			}
			else
			{
				/// �״�����ָ��Ipʧ�ܣ��Զ���ֵΪɨ�����ӷ���
				if(1 == m_nConnectType)
				{
					m_nConnectType = 0;
				}
				/// ��������״̬
				m_bConnect = false;
				/// ����ʧ�ܣ��ر��׽���
				e_CloseSocket();
			}
		}
		END_DEBUG_INFO
		return m_bConnect;
	}

	bool CNetBase::e_ScanConnectServer()
	{
		START_DEBUG_INFO
		/// ��ȡ���ε�ַ
		char szLocalIp[16];
		memset(szLocalIp, 0x00, sizeof(szLocalIp));
		/// ��ȡ����Ip
		e_GetHostIP(szLocalIp);
		/// ����Ipʧ��
		if(0 == strlen(szLocalIp))
		{
			END_DEBUG_INFO
			return false;
		}

		/// ��ȡIp��ַͷ
		char* ptr = NULL;
		ptr = strrchr(szLocalIp, '.');
		if(NULL == ptr)
		{
			/// Ip��ַ���󣬷��ؼ�
			END_DEBUG_INFO
			return false;
		}

		/// ������ȡIpͷ
		char szBeginPartIp[16];

		/// ����ɨ���Ip��ַ��
		memset(szBeginPartIp, 0x00, sizeof(szBeginPartIp));
		/// ��ֵIP��ַͷ
		memcpy(szBeginPartIp, szLocalIp, ptr - szLocalIp + 1);

		/// �����ȡɨ�赽��Ip
		char szServerIp[16];

		/// ��ȡ������Ipɨ���
		int nScanBegIp = NETSERVERSTARIP;
		int nScanEndIp = NETSERVERSTOPIP;

		/// ѭ��Ip����
		for(int i = nScanBegIp; i <= nScanEndIp; i++)
		{
			memset(szServerIp, 0x00, sizeof(szServerIp));
			/// ��ֵɨ�赽��Ip
			snprintf(szServerIp, sizeof(szServerIp), "%s%d", szBeginPartIp, i);

			if(true == e_ConnectTest(szServerIp, 100000))
			{
				/// ��ֵ������Ip
				snprintf(m_szServerIp, sizeof(m_szServerIp), "%s", szServerIp);
				return true;
			}
		}
		/// ��������״̬
		return false;
	}

	bool CNetBase::e_ConnectTest(char * pServerIp, int nConnectTimeOut)
	{
		START_DEBUG_INFO
		/// ���巵����Ϣ
		bool	bReturn = false;
		/// ����������ӵ��׽���Id
		UINT	nTestSocketID = 0;
		/// �����ȡ�Ĵ������
		int		nError = -1;

		/// �������ӽṹ��
		struct sockaddr_in saddr; 
		saddr.sin_family = AF_INET; 
		saddr.sin_addr.s_addr = inet_addr(pServerIp);
		saddr.sin_port = htons(m_nServerPort); 
		/// ��ȡ�����׽���
		nTestSocketID = socket(AF_INET, SOCK_STREAM, 0);
		/// �����׽��ִ���
		if(0 >= nTestSocketID)
		{
			END_DEBUG_INFO
			return false; 
		}
		//���÷�������ʽ����
#ifdef WIN32
		/// �������ӷ�ʽ
		unsigned long lSetType = 1;
		nError = ioctlsocket(nTestSocketID, FIONBIO, (unsigned long* )&lSetType);
		/// ������������ʧ�ܷ���
		if(SOCKET_ERROR == nError)
		{
			END_DEBUG_INFO
			return false;
		}
#else
		int flags = fcntl(nTestSocketID, F_GETFL, 0);
		fcntl(nTestSocketID, F_SETFL, flags|O_NONBLOCK); 
#endif
		/// ����ɨ�����ӷ�����
		nError = connect(nTestSocketID, (struct sockaddr*)&saddr, sizeof(saddr));
		if(-1 == nError)
		{
#ifndef WIN32
			/// Linux���������ΪEINPROGRESS ����û�д���ֻ�����ӻ�δ���
			if(errno == EINPROGRESS)
#endif
			{
				/// ���ô������ӳ�ʱ��ʱ��
				struct timeval tv; 
				fd_set writefds; 
				tv.tv_sec = 0;
				tv.tv_usec = nConnectTimeOut; 
				FD_ZERO(&writefds); 
				FD_SET(nTestSocketID, &writefds); 
				if(0 < select(nTestSocketID + 1, NULL, &writefds, NULL, &tv))
				{
#ifndef WIN32
					/// �����һ��һ��Ҫ����Ҫ���Linux����ǽ
					int nlen = sizeof(int);
					getsockopt(nTestSocketID, SOL_SOCKET, SO_ERROR, &nError, (socklen_t *)&nlen);
					if(0 == nError)
					{
						/// ���ӳɹ�
						bReturn = true;
					}
					else
					{
						/// ����ʧ��
						bReturn = false;
					}
#else
					/// ���óɹ�������
					bReturn = true;
#endif
				}
				else
				{
					bReturn = false;
				}
			}
#ifndef WIN32
			else
			{
				/// �������EINPROGRESS ���󣬷��ؼ�
				bReturn = false; 
			}
#endif
		} 
		else
		{
			/// ���ӳɹ�
			bReturn = true;
		}
		/// �رղ��������׽���
#ifdef WIN32
		closesocket(nTestSocketID); 
#else
		close(nTestSocketID); 
#endif
		return bReturn; 
	}

	bool CNetBase::e_IsConnect()
	{
		START_DEBUG_INFO
		/// ��������״̬
		END_DEBUG_INFO
		return m_bConnect;
	}

	bool CNetBase::e_InitClient()
	{
		START_DEBUG_INFO
		/// ���巵�ر���
		bool bReturn = false;
		/// ����ͻ������ݷ��͸����ط���
		T_ClientPack sttClientPack;
		/// ��ʼ���ṹ������
		memset(&sttClientPack, 0x00, sizeof(T_ClientPack));
		/// ��ֵ�ͻ�������
		sttClientPack.nClientType = m_nClientType;
		/// ��ֵ�׽���ID
		sttClientPack.nClientID = 0;
		/// ��ֵ�ͻ���IP
		e_GetHostIP(sttClientPack.szClientIp);
		/// �������ͽ�����ʱ����
		T_RSStructData* pRecvClient = NULL;

		/// ��ȡ�ṹ���С
		UINT nStructSize = sizeof(T_ClientPack);
		/// ��ȡ���ݰ���С
		UINT nAllSize = sizeof(T_RSStructData) + nStructSize - 1;
		/// ��ȡ��������
		pRecvClient = (T_RSStructData*)new char[nAllSize];
		/// ��ֵ���ݰ�
		pRecvClient->nClientType = m_nClientType;
		pRecvClient->nJobDataType = JDT_StructData;
		pRecvClient->nSRStructType = RST_ClientPack;
		pRecvClient->nDataSize = nStructSize;
		pRecvClient->nDataCount = 1;
		pRecvClient->nErrorCode = ECT_None;
		memcpy(pRecvClient->szData, &sttClientPack, nStructSize);
		/// ��������
		bReturn = e_SendStructData(JDT_StructData, RST_ClientPack, nAllSize, pRecvClient);
		if(NULL != pRecvClient)
		{
			delete pRecvClient;
		}
		pRecvClient = NULL;

		END_DEBUG_INFO
		return bReturn;
	}

	int CNetBase::e_StartReceive()
	{
		START_DEBUG_INFO
		/// ��������
		int nResult = 0;
		/// ��������߳�
#ifdef WIN32
		m_HRecvThreadHandle = CreateThread(NULL, 0, &i_ReceiveThread, this, 0, &m_DRecvThreadId);
#else
		pthread_mutex_init(&m_pCCNetSocket->m_RecvThreadMutex, 0);
		nResult = pthread_create(&m_pCCNetSocket->m_RecvThreadTaskId, 0, i_ReceiveThread, this);
#endif
		END_DEBUG_INFO
		return nResult;
	}

	int CNetBase::e_StartSend()
	{
		int nResult = 0;
#ifdef WIN32
		m_HSendThreadHandle = CreateThread(NULL, 0, &i_SendThread, this, 0, &m_DSendThreadId);
#else
		pthread_mutex_init(&m_pCCNetSocket->m_SendThreadMutex, 0);
		nResult = pthread_create(&m_pCCNetSocket->m_SendThreadTaskId, 0, i_SendThread, this);
#endif
		END_DEBUG_INFO
		return nResult;
	}

	void CNetBase::e_StopReceive()
	{
		START_DEBUG_INFO
		/// ��ֵ�������ձ�־
		m_bStopRecvTag = true;
		/// �ر��׽�������
		e_CloseSocket();

		/// �رս��������߳�
#ifdef WIN32
		WaitForMultipleObjects(1, &m_HRecvThreadHandle, TRUE, INFINITE);
		CloseHandle(m_HRecvThreadHandle);
#else
		int	nTemp = 0;
		usleep(200);
		while(true)
		{
			if(EBUSY == pthread_mutex_trylock(&m_pCCNetSocket->m_RecvThreadMutex))
			{
				nTemp++;
				if(10 == nTemp)
				{
					pthread_cancel(m_RecvThreadTaskId);
					break;
				}
				usleep(200);
			}
			else
			{
				break;
			}
		}
		pthread_join(m_RecvThreadTaskId, 0);
		pthread_mutex_destroy(&m_pCCNetSocket->m_RecvThreadMutex);
#endif
		END_DEBUG_INFO
	}

	void CNetBase::e_StopSend()
	{
		START_DEBUG_INFO
		/// ��ֵ�������ͱ�־
		m_bStopSendTag = true;
		/// �ر��׽�������
		e_CloseSocket();
		/// �رշ��������߳�
#ifdef WIN32
		WaitForMultipleObjects(1, &m_HSendThreadHandle, TRUE, INFINITE);
		CloseHandle(m_HSendThreadHandle);
#else
		int	nTemp = 0;
		usleep(200);
		while(true)
		{
			if(EBUSY == pthread_mutex_trylock(&m_pCCNetSocket->m_SendThreadMutex))
			{
				nTemp++;
				if(10 == nTemp)
				{
					pthread_cancel(m_SendThreadTaskId);
					break;
				}
				usleep(200);
			}
			else
			{
				break;
			}
		}
		pthread_join(m_SendThreadTaskId, 0);
		pthread_mutex_destroy(&m_pCCNetSocket->m_SendThreadMutex);
#endif
		END_DEBUG_INFO
	}

#ifdef WIN32
	DWORD WINAPI CNetBase::i_ReceiveThread(void* pBaseSocket)
	{
#else
	void *CNetBase::i_ReceiveThread(void* pBaseSocket)
	{
#endif
		START_DEBUG_INFO
		/// ���������߳�
		CNetBase* pSocket = (CNetBase* )pBaseSocket;
#ifndef WIN32
		pthread_mutex_lock(&m_pCCNetSocket->m_RecvThreadMutex);
#endif
		pSocket->e_ReceiveLoop();
#ifndef WIN32
		pthread_mutex_unlock(&m_pCCNetSocket->m_RecvThreadMutex);
#endif
		END_DEBUG_INFO
		return 0;
	}

#ifdef WIN32
	DWORD WINAPI CNetBase::i_SendThread(void* pBaseSocket)
	{
#else
	void *CNetBase::i_SendThread(void* pBaseSocket)
	{
#endif
		START_DEBUG_INFO
		/// ���������߳�
		CNetBase* pSocket = (CNetBase* )pBaseSocket;
#ifndef WIN32
		pthread_mutex_lock(&m_pCCNetSocket->m_SendThreadMutex);
#endif
		pSocket->e_SendLoop();
#ifndef WIN32
		pthread_mutex_unlock(&m_pCCNetSocket->m_SendThreadMutex);
#endif
		END_DEBUG_INFO
		return 0;
	}

	bool CNetBase::e_SendStructData(int nJobType, int nSST, long lStructLen, void* pStructData)
	{
		START_DEBUG_INFO
		/// �������
		if(false == m_bConnect)
		{
			printf("�������Ͽ�����\n");
			END_DEBUG_INFO
			return false;
		}

		/// �������ݼ���
		CLightAutoLock lckSendBufferLock(&m_SendBufferLock);

		if(MAXSENDBUFARR < (int)m_SendBufferArray.size())
		{
			printf("���ͻ���������̫��\n");
			END_DEBUG_INFO
			return false;
		}

		/// ����һ����������
		T_BufferStruct* pBuffer = new T_BufferStruct();
		if(NULL == pBuffer)
		{
			END_DEBUG_INFO
			return false;
		}
		/// ��ʼ����������
		memset(pBuffer, 0x00, sizeof(T_BufferStruct));

		/// ������������
		pBuffer->nJobType = nJobType;
		pBuffer->nSST = nSST;
		pBuffer->nSize = lStructLen;
		pBuffer->pBuf = new char[lStructLen + 1024];
		if(NULL == pBuffer->pBuf)
		{
			delete pBuffer;
			END_DEBUG_INFO
			return false;
		}
		/// ��ֵ��������
		memcpy(pBuffer->pBuf, pStructData, lStructLen);
		m_SendBufferArray.push_back(pBuffer);
		END_DEBUG_INFO
		return true;
	}

	void CNetBase::e_BuildBuffer(UINT nJobType, UINT nSST, long lStructLen,
		void* pStructData, char* pBuffer, long* plSendLen)
	{
		START_DEBUG_INFO
		long DataLen = 0;
		long lngTmp = 0;
		long lngSizeOfDataLen = 0;
		DataLen = sizeof(char) + sizeof(lStructLen) + lStructLen;
		/// �������ݳ���
		memcpy(pBuffer, &DataLen, sizeof(DataLen));
		lngSizeOfDataLen = sizeof(DataLen);
		lngTmp = nJobType;
		/// ����ҵ������
		pBuffer[lngSizeOfDataLen] = (char)lngTmp;
		lngTmp = nSST;
		/// ����ṹ����
		memcpy(&pBuffer[lngSizeOfDataLen + sizeof(char)], &lngTmp, sizeof(lngTmp));
		if(NULL != pStructData)
		{
			/// ����ṹ����
			memcpy(&pBuffer[lngSizeOfDataLen + sizeof(char) + sizeof(lngTmp)], pStructData, lStructLen);
		}
		*plSendLen = lngSizeOfDataLen + sizeof(char) + sizeof(lngTmp) + lStructLen;
		END_DEBUG_INFO
	}

	bool CNetBase::e_SendStruct(HSOCKET s, UINT nJobType, UINT nSST, long lStructLen, void* pStructData)
	{
		START_DEBUG_INFO
		long lSendLen = 0;
		/// ���͵�BYTE��С
		int nNumBytes;
		char* pBuffer = NULL;
		if(0 < s)
		{
			pBuffer = new char[lStructLen + 1024];
			if(NULL != pBuffer)
			{
				memset(pBuffer, 0x00, lStructLen + 1024);
				/// ���ݴ��
				e_BuildBuffer(nJobType, nSST, lStructLen, pStructData, pBuffer, &lSendLen);
				if(-1 == (nNumBytes = e_SendData(s, pBuffer, lSendLen)))
				{
					if(NULL != pBuffer)
					{
						delete[] pBuffer;
						pBuffer = NULL;
					}
					END_DEBUG_INFO
					return false;
				}
				if(NULL != pBuffer)
				{
					delete[] pBuffer;
					pBuffer = NULL;
				}
				END_DEBUG_INFO
				return true;
			}
		}
		END_DEBUG_INFO
		return false;
	}

	int CNetBase::e_SendData(HSOCKET s, char* pszBuf, UINT nsize)
	{
		START_DEBUG_INFO
		if(NULL == pszBuf)
		{
			END_DEBUG_INFO
			return -1;
		}
		/// ���Դ���
		int nTryCount = 0;
		/// ʧ�����Դ���
		int nTryDoCount = 0;
		int nsize1, nlen;
		nsize1 = nsize;
		while(0 < nsize) 
		{
			nlen = send(s, pszBuf, nsize, 0);
			if(0 > nlen) 
			{
#ifndef WIN32
				int err = errno;
#else
				int err = GetLastError();
#endif
				if(10055 == err)
				{
#ifndef WIN32
					usleep(1000);
#else
					Sleep(10);
#endif
					printf("��������\n");
					nTryDoCount++;
					if(10 < nTryDoCount)
					{
						END_DEBUG_INFO
						return -1;
					}
					continue;
				}
				else if(10060 == err)
				{
					/// ���ͳ�ʱ����������
					e_CloseSocket();
					END_DEBUG_INFO
					return -1;
				}
				else if(EINTR == err)
				{
#ifndef WIN32
					usleep(1000);
#else
					Sleep(10);
#endif
					printf("Socket EINTR\n");
					nTryDoCount++;
					if(10 < nTryDoCount)
					{
						END_DEBUG_INFO
						return -1;
					}
					continue;
				}
				else if(EAGAIN == err)
				{
					nTryCount++;
					printf("��������ʧ�����Դ��� = %i\n", nTryCount);
					if(2 <= nTryCount)
					{
						END_DEBUG_INFO
						return -1;
					}
					else
					{
						continue;
					}
				}
				else 
				{
					printf("�����׽������ݴ��� = %i\n", err);
					END_DEBUG_INFO
					return -1;
				}
			}
			nsize -= nlen;
			pszBuf += nlen;
			nTryDoCount = 0;
		}
		END_DEBUG_INFO
		return nsize1-nsize;
	}
	
	long CNetBase::e_ReceiveData(int nSocketId, char* pReceiveBuffer, long lRevLength)
	{
		START_DEBUG_INFO
		if(NULL == pReceiveBuffer)
		{
			END_DEBUG_INFO
			return -1;
		}
		/// ���ݳ���
		int nRecvLen = lRevLength;
		/// ���Դ���
		int nTryCount = 0;
		/// ʧ�����Դ���
		int nTryDoCount = 0;
		while(0 < lRevLength)
		{
			int nlen = recv(nSocketId, pReceiveBuffer, lRevLength, 0);
			if(0 >= nlen)
			{
#ifndef WIN32
				int nErr = errno;
#else
				int nErr = GetLastError();
#endif
				if(10055 == nErr)
				{
#ifndef WIN32
					usleep(1000);
#else
					Sleep(5);
#endif
					printf("��������\n");
					if(nTryDoCount++ > 10)
					{
						END_DEBUG_INFO
						return -1;
					}
					continue;
				}
				else if(EINTR == nErr)
				{
#ifndef WIN32
					usleep(1000);
#else
					Sleep(5);
#endif
					printf("Socket EINTR\n");
					nTryDoCount++;
					if(10 < nTryDoCount)
					{
						END_DEBUG_INFO
						return -1;
					}
					continue;
				}
				else if(EAGAIN == nErr)
				{
					nTryCount++;
					printf("��������ʧ�����Դ���=%i\n", nTryCount);
					if(2 <= nTryCount)
					{
						END_DEBUG_INFO
						return -1;
					}
					continue;
				}
				else 
				{
					printf("�������ݴ���[%i]\n", nErr);
					END_DEBUG_INFO
					return -1;
				}
			}
			else 
			{
				lRevLength -= nlen;
				pReceiveBuffer += nlen;
				nTryDoCount = 0;
			}
		}
		END_DEBUG_INFO
		return nRecvLen;
	}

	void CNetBase::e_DestroySendBufferArray()
	{
		START_DEBUG_INFO
		/// �����������黺��
		CLightAutoLock lckSendBuffer(&m_SendBufferLock);

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

	void CNetBase::e_GetHostIP(char* pszHostIp)
	{
		START_DEBUG_INFO
#ifdef _NC_WINDOWS_PLATFORM_
		char  szName[256];
		memset(szName, 0x00, sizeof(szName));
		/// ��ȡ��������
		if(0 == gethostname(szName, 256))
		{
			PHOSTENT  ptHostInfo;
			/// ������������ȡIp��ַ
			if(NULL != (ptHostInfo = gethostbyname(szName)))
			{
				LPCSTR pcIP = inet_ntoa(*(struct in_addr *)*ptHostInfo->h_addr_list);
				snprintf(pszHostIp, 16, "%s", pcIP);
			}
		}
#else
		int nFd, nIntrface, nRetn = 0;
		struct ifreq  tBuf[16];
		struct ifconf tIfc;
		if((nFd = socket(AF_INET, SOCK_DGRAM, 0)) >= 0) 
		{
			tIfc.ifc_len = sizeof tBuf;
			tIfc.ifc_buf = (caddr_t)tBuf;
			if(0 == ioctl(nFd, SIOCGIFCONF, (char* )&tIfc)) 
			{
				nIntrface = tIfc.ifc_len / sizeof(struct ifreq);
				while(nIntrface-- > 0)
				{
					/// ������ȡ���������豸
					if(!(ioctl(nFd, SIOCGIFFLAGS, (char *) &tBuf[nIntrface]))) 
					{
						if(tBuf[nIntrface].ifr_flags & IFF_PROMISC) 
						{
							nRetn++;
						}
					} 
					else 
					{
						char szTemp[256];
						sprintf(szTemp, "Ioctl device %s����", tBuf[nIntrface].ifr_name);
						//D_ERROR(szTemp);
					}
					/// �Ƿ�ȵ�������״̬
					if(tBuf[nIntrface].ifr_flags & IFF_UP) 
					{
					}
					/// �������л�ȡIP��ַ
					if(0 == (ioctl(nFd, SIOCGIFADDR, (char *)&tBuf[nIntrface])))
					{
						/// ��ȡIp��ַ
						char* pcIpData = inet_ntoa(((struct sockaddr_in*)
							(&tBuf[nIntrface].ifr_addr))->sin_addr);
						if(0 != strcmp(pcIpData, "127.0.0.1"))
						{
							/// ���˱��ص�ַip��Ϣ
							strcpy(pszHostIp, pcIpData);
							printf("pszHostIp = %s\r\n", pszHostIp);
						}
					}
					else 
					{
						char szTemp[256];
						sprintf(szTemp, "Ioctl device %s����", tBuf[nIntrface].ifr_name);
						//D_ERROR(szTemp);
					}
				}
			} 
			else
			{
				//D_ERROR("Ioctl����");
			}
		}
		else
		{
			//D_ERROR("Socket����");
		}
		close (nFd);
#endif
		END_DEBUG_INFO
	}

	void CNetBase::e_SetReceiveDataCallBack(OnRecvDataCallBack pReceiveStructCallBack)
	{
		START_DEBUG_INFO
		/// ���ûص�����
		m_pReceiveStructCallBack = pReceiveStructCallBack;
		END_DEBUG_INFO
	}
}