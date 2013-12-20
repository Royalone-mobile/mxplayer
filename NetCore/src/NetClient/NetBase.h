////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 
/// @par ģ����
/// <����ũ���ͻ���ͨѶ����>
/// @par ����ļ�
/// NetSocket.cpp 
/// @par ������ϸ����
/// <����ũ���ͻ���ͨѶ����>
/// @par ���̰߳�ȫ��
/// <��/��>[��˵��]
/// @par �쳣ʱ��ȫ��
/// <��/��>[��˵��]
/// @note         -
/// 
/// @file         NetSocket.h
/// @brief        <ģ�鹦�ܼ���>
/// @author       Li.xl
/// @version      1.0
/// @date         2011/05/25
/// @todo         <����Ҫ��������>
/// 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __FWAYNET_LOTTERYNETSOCKET_H__
#define __FWAYNET_LOTTERYNETSOCKET_H__

#include "common/AutoLock.h"
#include "NetBaseDef.h"

class CNetBase
{
public:
	CNetBase();
	~CNetBase();
public:

	/// 
	/// @par ���� 
	/// ���ӷ�����
	/// @param 
	///	[in,out]	<������>		<����˵��>
	/// [in]		pszServerIp		���ӷ���IP
	/// [in]		nServerPort		����˿ں�
	/// [in]		nClientType		�ͻ�������
	/// [in]		nConnectType	���ӷ�������(0 = ɨ������Ip�� 1 = �״�����ָ��Ip��2 = ֻ����ָ��Ip)
	/// @return 		-
	/// @note 
	/// <����˵��>
	/// @par ʾ����
	/// @code 
	/// <���ñ�������ʾ������>
	/// @endcode 
	/// @see		<�μ�����> 
	/// @deprecated	<��ʱ˵��> 
	/// 
	bool e_ConnectServer(char* pszServerIp, int nServerPort, int nClientType, int nConnectType);

	/// 
	/// @par ���� 
	/// ��ʼ��������
	/// @param 
	///	[in,out]	<������>		<����˵��>
	/// 
	/// @return 		-
	/// @note 
	/// <����˵��>
	/// @par ʾ����
	/// @code 
	/// <���ñ�������ʾ������>
	/// @endcode 
	/// @see		<�μ�����> 
	/// @deprecated	<��ʱ˵��> 
	/// 
	int e_StartReceive();

	/// 
	/// @par ���� 
	/// ��������
	/// @param 
	///	[in,out]	<������>		<����˵��>
	/// 
	/// @return 		-
	/// @note 
	/// <����˵��>
	/// @par ʾ����
	/// @code 
	/// <���ñ�������ʾ������>
	/// @endcode 
	/// @see		<�μ�����> 
	/// @deprecated	<��ʱ˵��> 
	/// 
	int e_StartSend();

	/// 
	/// @par ���� 
	/// ֹͣ�����߳�
	/// @param 
	///	[in,out]	<������>		<����˵��>
	/// 
	/// @return 		-
	/// @note 
	/// <����˵��>
	/// @par ʾ����
	/// @code 
	/// <���ñ�������ʾ������>
	/// @endcode 
	/// @see		<�μ�����> 
	/// @deprecated	<��ʱ˵��> 
	/// 
	void e_StopReceive();

	/// 
	/// @par ���� 
	/// ֹͣ�����߳�
	/// @param 
	///	[in,out]	<������>		<����˵��>
	/// 
	/// @return 		-
	/// @note 
	/// <����˵��>
	/// @par ʾ����
	/// @code 
	/// <���ñ�������ʾ������>
	/// @endcode 
	/// @see		<�μ�����> 
	/// @deprecated	<��ʱ˵��> 
	/// 
	void e_StopSend();

	/// 
	/// @par ���� 
	/// ���ý��սṹ��ص�
	/// @param 
	///	[in,out]	<������>		<����˵��>
	/// 
	/// @return 		-
	/// @note 
	/// <����˵��>
	/// @par ʾ����
	/// @code 
	/// <���ñ�������ʾ������>
	/// @endcode 
	/// @see		<�μ�����> 
	/// @deprecated	<��ʱ˵��> 
	/// 
	void e_SetReceiveDataCallBack(OnRecvDataCallBack pReceiveStructCallBack);

	/// 
	/// @par ���� 
	/// ���ͽṹ����
	/// @param 
	///	[in,out]	<������>		<����˵��>
	/// [in]		nJobType		��������
	/// [in]		nSST			�ṹ����
	/// @return 	bool			���سɹ�����ʧ��
	/// @note 
	/// <����˵��>
	/// @par ʾ����
	/// @code 
	/// <���ñ�������ʾ������>
	/// @endcode 
	/// @see		<�μ�����> 
	/// @deprecated	<��ʱ˵��> 
	/// 
	bool e_SendStructData(int nJobType, int nSST, long lStructLen, void* pStructData);

	/// 
	/// @par ���� 
	/// �������
	/// @param 
	///	[in,out]	<������>		<����˵��>
	/// [in]		nJobType		��������
	/// [in]		nSST			�ṹ������
	/// [in]		lStructLen		�ṹ���С
	/// [in]		pStructData		�ṹ������
	/// [in out]	pBuffer			��������
	/// [in]		plSendLen		�������ݴ�С
	/// @return 		-
	/// @note 
	/// <����˵��>
	/// @par ʾ����
	/// @code 
	/// <���ñ�������ʾ������>
	/// @endcode 
	/// @see		<�μ�����> 
	/// @deprecated	<��ʱ˵��> 
	/// 
	void e_BuildBuffer(UINT nJobType, UINT nSST, long lStructLen, 
		void* pStructData, char* pBuffer, long* plSendLen);

	/// 
	/// @par ���� 
	/// ���ͽṹ�����ݴ��
	/// @param 
	///	[in,out]	<������>		<����˵��>
	/// [in]		s				�׽���ID
	/// [in]		SST				�ṹ������
	/// [in]		StructLen		�ṹ�����ݴ�С
	/// [in]		pStructData		�ṹ������
	/// @return 		-
	/// @note 
	/// <����˵��>
	/// @par ʾ����
	/// @code 
	/// <���ñ�������ʾ������>
	/// @endcode 
	/// @see		<�μ�����> 
	/// @deprecated	<��ʱ˵��> 
	/// 
	bool e_SendStruct(HSOCKET s, UINT nJobType, UINT nSST, long lStructLen, void* pStructData);

	/// 
	/// @par ���� 
	/// 
	/// @param 
	///	[in,out]	<������>		<����˵��>
	/// [in]		s				�׽���ID
	/// [in]		pBuf			Ҫ���͵�����
	/// [in]		size			���ݴ�С
	/// @return 		-
	/// @note 
	/// <����˵��>
	/// @par ʾ����
	/// @code 
	/// <���ñ�������ʾ������>
	/// @endcode 
	/// @see		<�μ�����> 
	/// @deprecated	<��ʱ˵��> 
	/// 
	int e_SendData(HSOCKET s, char* pBuf, UINT nsize);

	/// 
	/// @par ���� 
	/// ���������߳�
	/// @param 
	///	[in,out]	<������>		<����˵��>
	/// 
	/// @return 		-
	/// @note 
	/// <����˵��>
	/// @par ʾ����
	/// @code 
	/// <���ñ�������ʾ������>
	/// @endcode 
	/// @see		<�μ�����> 
	/// @deprecated	<��ʱ˵��> 
	/// 
	virtual void e_ReceiveLoop() = 0;

	/// 
	/// @par ���� 
	/// ���������߳�
	/// @param 
	///	[in,out]	<������>		<����˵��>
	/// 
	/// @return 		-
	/// @note 
	/// <����˵��>
	/// @par ʾ����
	/// @code 
	/// <���ñ�������ʾ������>
	/// @endcode 
	/// @see		<�μ�����> 
	/// @deprecated	<��ʱ˵��> 
	/// 
	virtual void e_SendLoop() = 0;

	/// 
	/// @par ���� 
	/// ������յ���������
	/// @param 
	///	[in,out]	<������>		<����˵��>
	/// [in]		byDataType		�������ݰ�����
	/// [in]		pData			����ָ��
	/// @return 		-
	/// @note 
	/// <����˵��>
	/// @par ʾ����
	/// @code 
	/// <���ñ�������ʾ������>
	/// @endcode 
	/// @see		<�μ�����> 
	/// @deprecated	<��ʱ˵��> 
	/// 
	virtual void e_ProcessReceiveData(BYTE BtDataType, void* pData) = 0;
		
	/// 
	/// @par ���� 
	/// ��֤�Ƿ����ӵ��˷�����
	/// @param 
	///	[in,out]	<������>		<����˵��>
	/// 
	/// @return 		-
	/// @note 
	/// <����˵��>
	/// @par ʾ����
	/// @code 
	/// <���ñ�������ʾ������>
	/// @endcode 
	/// @see		<�μ�����> 
	/// @deprecated	<��ʱ˵��> 
	/// 
	bool e_IsConnect();

	/// 
	/// @par ���� 
	/// ��ʼ���ͻ�������
	/// @param 
	///	[in,out]	<������>		<����˵��>
	/// 
	/// @return 		-
	/// @note 
	/// <����˵��>
	/// @par ʾ����
	/// @code 
	/// <���ñ�������ʾ������>
	/// @endcode 
	/// @see		<�μ�����> 
	/// @deprecated	<��ʱ˵��> 
	/// 
	bool e_InitClient();

	/// 
	/// @par ���� 
	/// ��ȡ����Ip
	/// @param 
	///	[in,out]	<������>		<����˵��>
	/// [in/out]	pszHostIp		��ȡ����IP
	/// @return 		-
	/// @note 
	/// <����˵��>
	/// @par ʾ����
	/// @code 
	/// <���ñ�������ʾ������>
	/// @endcode 
	/// @see		<�μ�����> 
	/// @deprecated	<��ʱ˵��> 
	/// 
	void e_GetHostIP(char* pszHostIp);

private:
#ifdef WIN32
	/// 
	/// @par ���� 
	/// ���������߳�
	/// @param 
	///	[in,out]	<������>		<����˵��>
	/// [in]		pBaseSocket		�׽�������
	/// @return 		-
	/// @note 
	/// ������Windows
	/// @par ʾ����
	/// @code 
	/// <���ñ�������ʾ������>
	/// @endcode 
	/// @see		<�μ�����> 
	/// @deprecated	<��ʱ˵��> 
	/// 
	static DWORD WINAPI i_ReceiveThread(void* pBaseSocket);

	/// 
	/// @par ���� 
	/// ���������߳�
	/// @param 
	///	[in,out]	<������>		<����˵��>
	/// [in]		pBaseSocket		�׽�������
	/// @return 		-
	/// @note 
	/// ������Windows
	/// @par ʾ����
	/// @code 
	/// <���ñ�������ʾ������>
	/// @endcode 
	/// @see		<�μ�����> 
	/// @deprecated	<��ʱ˵��> 
	/// 
	static DWORD WINAPI i_SendThread(void* pBaseSocket);
#else
	/// 
	/// @par ���� 
	/// ���������߳�
	/// @param 
	///	[in,out]	<������>		<����˵��>
	/// [in]		pBaseSocket		�׽�������
	/// @return 		-
	/// @note 
	/// ������Linux
	/// @par ʾ����
	/// @code 
	/// <���ñ�������ʾ������>
	/// @endcode 
	/// @see		<�μ�����> 
	/// @deprecated	<��ʱ˵��> 
	/// 
	static void *i_ReceiveThread(void *pBaseSocket);

	/// 
	/// @par ���� 
	/// ���������߳�
	/// @param 
	///	[in,out]	<������>		<����˵��>
	/// [in]		pBaseSocket		�׽�������
	/// @return 		-
	/// @note 
	/// ������Linux
	/// @par ʾ����
	/// @code 
	/// <���ñ�������ʾ������>
	/// @endcode 
	/// @see		<�μ�����> 
	/// @deprecated	<��ʱ˵��> 
	/// 
	static void *i_SendThread(void *pBaseSocket);
#endif

protected:
	/// 
	/// @par ���� 
	/// ��ʼ���׽���
	/// @param 
	///	[in,out]	<������>		<����˵��>
	/// 
	/// @return 		-
	/// @note 
	/// ��Ҫ�û���������
	/// @par ʾ����
	/// @code 
	/// <���ñ�������ʾ������>
	/// @endcode 
	/// @see		<�μ�����> 
	/// @deprecated	<��ʱ˵��> 
	/// 
	bool e_IniSocket();

	/// 
	/// @par ���� 
	/// ���õ�ǰʹ�õ��׽�������
	/// @param 
	///	[in,out]	<������>		<����˵��>
	/// [in]		nSocketId		�׽���ID
	/// @return 		-
	/// @note 
	/// <����˵��>
	/// @par ʾ����
	/// @code 
	/// <���ñ�������ʾ������>
	/// @endcode 
	/// @see		<�μ�����> 
	/// @deprecated	<��ʱ˵��> 
	/// 
	void e_SetSocket(int nSocketId);

	/// 
	/// @par ���� 
	/// �ر��׽���
	/// @param 
	///	[in,out]	<������>		<����˵��>
	/// 
	/// @return 		-
	/// @note 
	/// <����˵��>
	/// @par ʾ����
	/// @code 
	/// <���ñ�������ʾ������>
	/// @endcode 
	/// @see		<�μ�����> 
	/// @deprecated	<��ʱ˵��> 
	/// 
	void e_CloseSocket();

	/// 
	/// @par ���� 
	/// �������ӷ�����
	/// @param 
	///	[in,out]	<������>		<����˵��>
	/// 
	/// @return 		-
	/// @note 
	/// <����˵��>
	/// @par ʾ����
	/// @code 
	/// <���ñ�������ʾ������>
	/// @endcode 
	/// @see		<�μ�����> 
	/// @deprecated	<��ʱ˵��> 
	/// 
	bool e_ReconnectServer();

	/// 
	/// @par ���� 
	/// ɨ�����ӷ�����
	/// @param 
	///	[in,out]	<������>		<����˵��>
	/// 
	/// @return 	bool			���ӳɹ�����ʧ��
	/// @note 
	/// <����˵��>
	/// @par ʾ����
	/// @code 
	/// <���ñ�������ʾ������>
	/// @endcode 
	/// @see		<�μ�����> 
	/// @deprecated	<��ʱ˵��> 
	/// 
	bool e_ScanConnectServer();

	/// 
	/// @par ���� 
	/// ��������ɨ��ķ�����Ip
	/// @param 
	///	[in,out]	<������>		<����˵��>
	/// [in]		pServerIp		���ӷ�������Ip
	/// [in]		nConnectTimeOut	�������ӳ�ʱʱ��
	/// @return 		-
	/// @note 
	/// <����˵��>
	/// @par ʾ����
	/// @code 
	/// <���ñ�������ʾ������>
	/// @endcode 
	/// @see		<�μ�����> 
	/// @deprecated	<��ʱ˵��> 
	/// 
	bool e_ConnectTest(char* pServerIp, int nConnectTimeOut);

	/// 
	/// @par ���� 
	/// �����������
	/// @param 
	///	[in,out]	<������>		<����˵��>
	/// [in]		nSocketId		�׽���ID
	/// [in]		pszRevBuffer	�������ݻ����ַ
	/// [in]		lngRevLength	�������ݳ���
	/// @return 		-
	/// @note 
	/// <����˵��>
	/// @par ʾ����
	/// @code 
	/// <���ñ�������ʾ������>
	/// @endcode 
	/// @see		<�μ�����> 
	/// @deprecated	<��ʱ˵��> 
	/// 
	long e_ReceiveData(int nSocketId, char* pszRevBuffer, long lRevLength);

	/// 
	/// @par ���� 
	/// ���������������
	/// @param 
	///	[in,out]	<������>		<����˵��>
	/// 
	/// @return 		-
	/// @note 
	/// <����˵��>
	/// @par ʾ����
	/// @code 
	/// <���ñ�������ʾ������>
	/// @endcode 
	/// @see		<�μ�����> 
	/// @deprecated	<��ʱ˵��> 
	/// 
	void e_DestroySendBufferArray();

protected:
#ifdef WIN32

	/// ��ʼ���׽��ַ���ֵ
	int						m_nWSAInitResult;
	/// ���������߳�ID
	DWORD					m_DRecvThreadId;
	/// ���������߳̾��
	HANDLE					m_HRecvThreadHandle;
	/// ���������߳�ID
	DWORD					m_DSendThreadId;
	/// ���������߳̾��
	HANDLE					m_HSendThreadHandle;
#else
	/// ���������߳�ID 
	pthread_t				m_RecvThreadTaskId;
	/// ���������߳̾��
	pthread_mutex_t			m_RecvThreadMutex;
	/// ���������߳�ID
	pthread_t				m_SendThreadTaskId;
	/// ���������߳̾��
	pthread_mutex_t			m_SendThreadMutex;
#endif
	/// �׽���ID
	int						m_nSocketId;
	/// ֹͣ���ձ�־
	bool					m_bStopRecvTag;
	/// ֹͣ���ͱ�־
	bool					m_bStopSendTag;
	/// ���̷߳���������
	CLightCritSec			m_SendLock;
	/// ���ͻ�����
	CLightCritSec			m_SendBufferLock;
	/// ���ͻ�������
	CPublicPtrArray			m_SendBufferArray;
	/// �������ݻص���������
	OnRecvDataCallBack		m_pReceiveStructCallBack;
	/// �Ƿ����ӳɹ�״̬
	bool					m_bConnect;
	/// ������IP��ַ
	char					m_szServerIp[15];
	/// �������˿ں�
	int						m_nServerPort;
	/// ���ӷ�������(0 = ɨ������Ip�� 1 = �״�����ָ��Ip��2 = ֻ����ָ��Ip)
	int						m_nConnectType;
public:
	/// �ͻ�������
	int						m_nClientType;
};

#endif	/// __FWAYNET_LOTTERYNETSOCKET_H__