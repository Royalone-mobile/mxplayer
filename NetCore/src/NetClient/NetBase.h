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
	/// ��ʼ���ͻ���������Ϣ
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	[in]		psttNetClient	�ͻ���������Ϣ�ṹ��
	/// @return 	-
	/// @note 		Creat By li.xl 2013/12/21 
	/// ==============================================
	bool e_InitNetClient(T_InitNetClient* psttNetClient);

	/// ==============================================
	/// @par ���� 
	/// ���ӷ�����
	/// @param 
	/// [in,out]	<������>		<����˵��>
	/// @return 	bool			�����Ƿ�ɹ�
	/// @note 		Creat By li.xl 2013/12/20 
	/// ==============================================
	bool e_ConnectServer();

	/// ==============================================
	/// @par ���� 
	/// ����UDP�ͻ���
	/// @param 
	/// [in,out]	<������>		<����˵��>
	/// @return 	-
	/// @note 		Creat By li.xl 2013/12/20 
	/// ==============================================
	bool e_CreatUDPClient();

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
	/// ��֤�Ƿ����ӵ��˷�����
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	-
	/// @return 	bool			�����Ƿ����ӳɹ�
	/// @note 		Creat By li.xl 2013/12/20 
	/// ==============================================
	bool e_IsTCPConnect();

	/// ==============================================
	/// @par ���� 
	/// ��֤UDP�ͻ����Ƿ��Ѿ�����
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	
	/// @return 	-
	/// @note 		Creat By li.xl 2013/12/21 
	/// ==============================================
	bool e_IsUDPCreated();

	/// ==============================================
	/// @par ���� 
	/// ��ʼ��������
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	-
	/// @return 	-
	/// @note 		Creat By li.xl 2013/12/20 
	/// ==============================================
	bool e_StartTCPReceive();

	/// ==============================================
	/// @par ���� 
	/// ��ʼ��������
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	-
	/// @return 	-
	/// @note 		Creat By li.xl 2013/12/20 
	/// ==============================================
	bool e_StartTCPSend();

	/// ==============================================
	/// @par ���� 
	/// ֹͣ��������
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	-
	/// @return 	-
	/// @note 		Creat By li.xl 2013/12/20 
	/// ==============================================
	void e_StopTCPReceive();

	/// ==============================================
	/// @par ���� 
	/// ֹͣ�����߳�
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	-
	/// @return 	-
	/// @note 		Creat By li.xl 2013/12/20 
	/// ==============================================
	void e_StopTCPSend();

	/// ==============================================
	/// @par ���� 
	/// ���������̣߳��麯����
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	-
	/// @return 	-
	/// @note 		Creat By li.xl 2013/12/20 
	/// ==============================================
	virtual void e_ReceiveTCPLoop() = 0;

	/// ==============================================
	/// @par ���� 
	/// ���������߳�
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	-
	/// @return 	-
	/// @note 		Creat By li.xl 2013/12/20 
	/// ==============================================
	virtual void e_SendTCPLoop() = 0;

	/// ==============================================
	/// @par ���� 
	/// ��ʼ��������
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	-
	/// @return 	-
	/// @note 		Creat By li.xl 2013/12/20 
	/// ==============================================
	bool e_StartUDPReceive();

	/// ==============================================
	/// @par ���� 
	/// ��ʼ��������
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	-
	/// @return 	-
	/// @note 		Creat By li.xl 2013/12/20 
	/// ==============================================
	bool e_StartUDPSend();

	/// ==============================================
	/// @par ���� 
	/// ֹͣ��������
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	-
	/// @return 	-
	/// @note 		Creat By li.xl 2013/12/20 
	/// ==============================================
	void e_StopUDPReceive();

	/// ==============================================
	/// @par ���� 
	/// ֹͣ�����߳�
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	-
	/// @return 	-
	/// @note 		Creat By li.xl 2013/12/20 
	/// ==============================================
	void e_StopUDPSend();

	/// ==============================================
	/// @par ���� 
	/// ���������̣߳��麯����
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	-
	/// @return 	-
	/// @note 		Creat By li.xl 2013/12/20 
	/// ==============================================
	virtual void e_ReceiveUDPLoop() = 0;

	/// ==============================================
	/// @par ���� 
	/// ���������߳�
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	-
	/// @return 	-
	/// @note 		Creat By li.xl 2013/12/20 
	/// ==============================================
	virtual void e_SendUDPLoop() = 0;

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
	/// ����TCP�����߳�
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	[in]		pNetBase		�������
	/// @return 	-
	/// @note 		Creat By li.xl 2013/12/20 
	/// ==============================================
	static DWORD WINAPI i_RecvTCPThread(void* pNetBase);

	/// ==============================================
	/// @par ���� 
	/// ����TCP�����߳�
	/// @param 
	/// [in,out]	<������>		<����˵��>
	/// [in]		pNetBase		�������
	/// @return 	-
	/// @note 		Creat By li.xl 2013/12/20 
	/// ==============================================
	static DWORD WINAPI i_SendTCPThread(void* pNetBase);

	/// ==============================================
	/// @par ���� 
	/// ����UDP�����߳�
	/// @param 
	/// [in,out]	<������>		<����˵��>
	/// [in]		pNetBase		�������
	/// @return 	-
	/// @note 		Creat By li.xl 2013/12/21 
	/// ==============================================
	static DWORD WINAPI i_RecvUDPThread(void* pNetBase);

	/// ==============================================
	/// @par ���� 
	/// ����UDP�����߳�
	/// @param 
	/// [in,out]	<������>		<����˵��>
	/// [in]		pNetBase		�������
	/// @return 	-
	/// @note 		Creat By li.xl 2013/12/21 
	/// ==============================================
	static DWORD WINAPI i_SendUDPThread(void* pNetBase);

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
	/// �ͷ��¼�����
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	
	/// @return 	-
	/// @note 		Creat By li.xl 2013/12/21 
	/// ==============================================
	void i_CancelEvens();

	/// ==============================================
	/// @par ���� 
	/// �ͷ�TCP���ͻ�������
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	-
	/// @return 	-
	/// @note 		Creat By li.xl 2013/12/20 
	/// ==============================================
	void i_DestroyTCPSendBufferArray();

	/// ==============================================
	/// @par ���� 
	/// �ͷ�UDP���ͻ���
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	-
	/// @return 	-
	/// @note 		Creat By li.xl 2013/12/21 
	/// ==============================================
	void i_DestroyUDPSendBufferArray();

private:
	/// TCP�׽���ID
	HSOCKET					m_sClientTCPSocket;
	/// UDP�׽���
	HSOCKET					m_sClientUDPSocket;
	/// ����TCP������
	CLightCritSec			m_CLCSConnSeverLock;
	/// TCP�Ƿ����ӳɹ�״̬
	bool					m_bIsTCPConnect;
	/// UDP�Ƿ񴴽��ɹ�
	bool					m_bIsUDPCreated;
protected:
	/// ��ʼ������ͻ�����Ϣ
	T_InitNetClient			m_sttInitNetClient;
	/// ����TCP�����߳̾��
	HANDLE					m_HRecvTCPThreadHandle;
	/// ����TCP�����߳̾��
	HANDLE					m_HSendTCPThreadHandle;
	/// ����TCPֹͣ�¼�
	HANDLE					m_HRecvTCPStopEven;
	/// ����TCPֹͣ�¼�
	HANDLE					m_HSendTCPStopEven;

	/// ����UDP�����߳̾��
	HANDLE					m_HRecvUDPThreadHandle;
	/// ����UDP�����߳̾��
	HANDLE					m_HSendUDPThreadHandle;
	/// ����UDPֹͣ�¼�
	HANDLE					m_HRecvUDPStopEven;
	/// ����UDPֹͣ�¼�
	HANDLE					m_HSendUDPStopEven;

	/// ����TCP������
	CLightCritSec			m_SendTCPBufferLock;
	/// ���ͻ�������
	CPublicPtrArray			m_SendTCPBufferArray;
	/// ����UDP������
	CLightCritSec			m_SendUDPBufferLock;
	/// �������ݻ�������
	CPublicPtrArray			m_SendUDPBufferArray;
	/// �������ݻص���������
	OnRecvDataCallBack		m_pfnRecDataCallBack;
};

#endif	/// __FWAYNET_NETCLIENT_NETBASE_H__