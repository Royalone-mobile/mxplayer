////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 
/// @par ģ����
/// FWAY�ͻ���ͨѶ����
/// @par ����ļ�
/// NetBase.cpp 
/// @par ������ϸ����
/// @par ���̰߳�ȫ��
/// [��˵��]
/// @par �쳣ʱ��ȫ��
/// [��˵��]
/// @note         -
/// @file         NetBase.h
/// @brief        -
/// @author       Li.xl
/// @version      1.0
/// @date         2013/12/20
/// @todo         -
/// 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __FWAYNET_NETCLIENT_NETBASE_H__
#define __FWAYNET_NETCLIENT_NETBASE_H__

#include "common/AutoLock.h"
#include "NetBaseDef.h"
#include "NetClient/NetClientDef.h"

class CNetBase
{
public:
	CNetBase();
	~CNetBase();
public:
	/// ==============================================
	/// @par ���� 
	/// ���ӷ�����
	/// @param 
	/// [in,out]	<������>		<����˵��>
	/// [in]		pszServerIp		���ӷ���IP
	/// [in]		usServerPort	����˿ں�
	/// @return 	bool			�����Ƿ�ɹ�
	/// @note 		Creat By li.xl 2013/12/20 
	/// ==============================================
	bool e_ConnectServer(const char* pszServerIP, USHORT usServerPort);

	/// ==============================================
	/// @par ���� 
	/// ����UDP�ͻ���
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	[in]		usClientUDPPort	UDP�˿ں�
	/// @return 	-
	/// @note 		Creat By li.xl 2013/12/20 
	/// ==============================================
	bool e_CreatUDPClient(USHORT usClientUDPPort);

	/// ==============================================
	/// @par ���� 
	/// ���ý��սṹ��ص�
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	
	/// @return 	-
	/// @note 		Creat By li.xl 2013/12/20 
	/// ==============================================
	void e_SetReceiveDataCallBack(OnRecvDataCallBack pfnRecDataCallBack);

	/// ==============================================
	/// @par ���� 
	/// �������ݵ��ͻ���
	/// @param 
	/// [in,out]	<������>		<����˵��>
	/// [in]		pszSendBuf		Ҫ���͵�����
	/// [in]		nSendSize		���ݴ�С
	/// @return 	-
	/// @note 		Creat By li.xl 2013/12/20 
	/// ==============================================
	int e_SendTCPData(char* pszSendBuf, UINT nSendSize);

	/// ==============================================
	/// @par ���� 
	/// ��������
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	[in]		pszRemoteIP		Զ������IP
	/// [in]		usRemotePort	Զ�̶˿�
	/// [in]		pszSendBuf		Ҫ���͵�����
	/// [in]		nSendSize		���ݴ�С		
	/// @return 	-
	/// @note 		Creat By li.xl 2013/12/20 
	/// ==============================================
	int e_SendUDPData(const char* pszRemoteIP, USHORT usRemotePort, char* pszSendBuf, UINT nSendSize);


protected:

	/// ==============================================
	/// @par ���� 
	/// �������ӷ�����
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	-
	/// @return 	-
	/// @note 		Creat By li.xl 2013/12/20 
	/// ==============================================
	bool e_ReconnectServer();

	/// ==============================================
	/// @par ���� 
	/// ��ʼ��������
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	-
	/// @return 	-
	/// @note 		Creat By li.xl 2013/12/20 
	/// ==============================================
	int e_StartReceive();

	/// ==============================================
	/// @par ���� 
	/// ֹͣ��������
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	-
	/// @return 	-
	/// @note 		Creat By li.xl 2013/12/20 
	/// ==============================================
	void e_StopReceive();

	/// ==============================================
	/// @par ���� 
	/// ��ʼ��������
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	-
	/// @return 	-
	/// @note 		Creat By li.xl 2013/12/20 
	/// ==============================================
	int e_StartSend();

	/// ==============================================
	/// @par ���� 
	/// ֹͣ�����߳�
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	-
	/// @return 	-
	/// @note 		Creat By li.xl 2013/12/20 
	/// ==============================================
	void e_StopSend();

	/// ==============================================
	/// @par ���� 
	/// ���������̣߳��麯����
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	-
	/// @return 	-
	/// @note 		Creat By li.xl 2013/12/20 
	/// ==============================================
	virtual void e_ReceiveLoop() = 0;

	/// ==============================================
	/// @par ���� 
	/// ���������߳�
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	-
	/// @return 	-
	/// @note 		Creat By li.xl 2013/12/20 
	/// ==============================================
	virtual void e_SendLoop() = 0;
		
	/// ==============================================
	/// @par ���� 
	/// ��֤�Ƿ����ӵ��˷�����
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	-
	/// @return 	bool			�����Ƿ����ӳɹ�
	/// @note 		Creat By li.xl 2013/12/20 
	/// ==============================================
	bool e_IsConnect();

	/// ==============================================
	/// @par ���� 
	/// �ر��׽���
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	-
	/// @return 	-
	/// @note 		Creat By li.xl 2013/12/20 
	/// ==============================================
	void e_CloseSocket();

	/// ==============================================
	/// @par ���� 
	/// �ر�TCP�����׽���
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	-
	/// @return 	-
	/// @note 		Creat By li.xl 2013/12/20 
	/// ==============================================
	void e_CloseTCPSocket();

	/// ==============================================
	/// @par ���� 
	/// �ر�UDP�����׽���
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	-
	/// @return 	-
	/// @note 		Creat By li.xl 2013/12/20 
	/// ==============================================
	void e_CloseUDPSocket();

	/// ==============================================
	/// @par ���� 
	/// �������TCP����
	/// @param 
	/// [in,out]	<������>		<����˵��>
	/// [in]		pszRevBuffer	���յ�������Buffer
	/// [in]		nRevLength		�յ������ݳ���
	/// @return 	-
	/// @note 		Creat By li.xl 2013/12/20 
	/// ==============================================
	int e_RecTCPData(char* pszRevBuffer, int nRevLength);

	/// ==============================================
	/// @par ���� 
	/// �������UDP����
	/// @param 
	/// [in,out]	<������>		<����˵��>
	/// [in]		pszRevBuffer	�������ݻ���
	/// [in]		nRevLength		�յ������ݳ���
	///	[in]		strRemoteIP		Զ��IP
	/// [in]		usRemotePort	Զ�̶˿�
	/// @return 	-
	/// @note 		Creat By li.xl 2013/12/21 
	/// ==============================================
	int e_RecUDPData(char* pszRevBuffer, int nRevLength, string& strRemoteIP, USHORT& usRemotePort);

	/// ==============================================
	/// @par ���� 
	/// ��֤�׽����Ƿ�ɶ�д
	/// @param 
	/// [in,out]	<������>		<����˵��>
	/// [in]		sClientSocket	�ͻ����׽���
	///	[in]		usMode			��дģʽ��֤
	/// [in]		usProcessTimeOut��ʱʱ��
	/// @return 	-
	/// @note 		Creat By li.xl 2013/12/20 
	/// ==============================================
	int e_SelectProcess(HSOCKET sClientSocket, USHORT usMode, USHORT usProcessTimeOut = NET_REV_SEND_TIME_OUT);

private:
	/// ==============================================
	/// @par ���� 
	/// ���������߳�
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	[in]		pNetBase		�������
	/// @return 	-
	/// @note 		Creat By li.xl 2013/12/20 
	/// ==============================================
	static DWORD WINAPI i_ReceiveThread(void* pNetBase);

	/// ==============================================
	/// @par ���� 
	/// ���������߳�
	/// @param 
	/// [in,out]	<������>		<����˵��>
	////	[in]		pNetBase		�������
	/// @return 	-
	/// @note 		Creat By li.xl 2013/12/20 
	/// ==============================================
	static DWORD WINAPI i_SendThread(void* pNetBase);

	/// ==============================================
	/// @par ���� 
	/// ��ʼ���׽���
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	-
	/// @return 	-
	/// @note 		Creat By li.xl 2013/12/20 
	/// ==============================================
	bool i_InitTCPSocket();

	/// ==============================================
	/// @par ���� 
	/// ��ʼ��UDP�׽���
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	
	/// @return 	-
	/// @note 		Creat By li.xl 2013/12/21 
	/// ==============================================
	bool i_InitUDPSocket();

	/// ==============================================
	/// @par ���� 
	/// �ͷŷ��ͻ�������
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	-
	/// @return 	-
	/// @note 		Creat By li.xl 2013/12/20 
	/// ==============================================
	void i_DestroySendBufferArray();

protected:
	/// TCP�׽���ID
	HSOCKET					m_sClientTCPSocket;
	/// UDP�׽���
	HSOCKET					m_sClientUDPSocket;
	/// ������IP��ַ
	char					m_szServerIP[16];
	/// �������˿ں�
	USHORT					m_usServerTCPPort;
	/// UDP�˿ں�
	USHORT					m_usClientUDPPort;

protected:
	/// ���������߳̾��
	HANDLE					m_HRecvThreadHandle;
	/// ���������߳̾��
	HANDLE					m_HSendThreadHandle;
	/// ����ֹͣ�¼�
	HANDLE					m_HRecvStopEven;
	/// ����ֹͣ�¼�
	HANDLE					m_HSendStopEven;
	/// ���ͻ�����
	CLightCritSec			m_SendBufferLock;
	/// ���ͻ�������
	CPublicPtrArray			m_SendBufferArray;
	/// �������ݻص���������
	OnRecvDataCallBack		m_pfnRecDateCallBack;
	/// �Ƿ����ӳɹ�״̬
	bool					m_bConnect;
};

#endif	/// __FWAYNET_NETCLIENT_NETBASE_H__