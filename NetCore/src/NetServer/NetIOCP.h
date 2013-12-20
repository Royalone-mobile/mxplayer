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
#ifndef __THE_LEE_NET_IOCP_H_H__
#define __THE_LEE_NET_IOCP_H_H__

#include "ContextList.h"
#include "BufferPool.h"
#include "NetServerDef.h"

class CNetIOCP
{
public:
	CNetIOCP();
	virtual ~CNetIOCP();

public:
	/// ==============================================
	/// @par ���� 
	/// ��ʼ��IOCP������Ϣ
	/// @param 
	/// [in,out]	<������>			<����˵��>
	///	[in]		sttNetInitStruct	�����ʼ������
	/// [in]		pFunRecvData		�������ݻص�����
	/// [in]		pFunSendData		�������ݻص�����
	/// [in]		pFunConection		�ͻ���TCP�ɹ����ӻص�����
	/// [in]		pFunDisConection	�ͻ���TCP�Ͽ����ӻص�����
	/// @return 	-
	/// @note 		by li.xl 2013/06/14 
	/// ==============================================
	bool e_InitNetModel(T_NetInitStruct& sttNetInitStruct,
		OnRecvDataCallBack pFunRecvData = NULL,
		OnSendDataCallBack pFunSendData = NULL, 
		OnConectionCallBack pFunConection = NULL, 
		OnDisConectionCallBack pFunDisConection = NULL,
		OnPrintLog pFunPrintLog = NULL);

	/// ==============================================
	/// @par ���� 
	/// �����������
	/// @param 
	/// [in,out]	<������>		<����˵��>	
	/// @return 	-
	/// @note 		by li.xl 2013/06/14 
	/// ==============================================
	bool e_StartNetService();

	/// ==============================================
	/// @par ���� 
	/// �����������
	/// @param 
	/// [in,out]	<������>		<����˵��>
	/// @return 	-
	/// @note 		by li.xl 2013/06/22 
	/// ==============================================
	bool e_StopdNetService();
private:
	/// ==============================================
	/// @par ���� 
	/// ��������TCP�������
	/// @param 
	/// [in,out]	<������>		<����˵��>
	/// @return 	-
	/// @note 		by li.xl 2013/06/14 
	/// ==============================================
	bool i_RunTCPService();

	/// ==============================================
	/// @par ���� 
	/// ��������UDP�������
	/// @param 
	/// [in,out]	<������>		<����˵��>
	/// @return 	-
	/// @note 		by li.xl 2013/06/14 
	/// ==============================================
	bool i_RunUDPService();

	/// ==============================================
	/// @par ���� 
	/// TCP���Ӽ����߳�
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	[in]		pParama			�̲߳���
	/// @return 	-
	/// @note 		by li.xl 2013/06/14 
	/// ==============================================
	static UINT WINAPI i_ListenThread(void* pParama);

	/// ==============================================
	/// @par ���� 
	/// �����߳�
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	[in]		pParama			�̲߳���
	/// @return 	-
	/// @note 		by li.xl 2013/06/14 
	/// ==============================================
	static UINT WINAPI i_WorkThread(void* pParama);

	/// ==============================================
	/// @par ���� 
	/// IOCP����������
	/// @param 
	/// @return 	-
	/// @note 		by li.xl 2013/06/19 
	/// ==============================================
	void i_IoWorkerLoop();

	/// ==============================================
	/// @par ���� 
	/// ���������Ӳ�����Ͷ�ݵ�IO�����߳�
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	[in]		sClientSocket	�ͻ��������׽���
	/// @return 	-
	/// @note 		by li.xl 2013/06/18 
	/// ==============================================
	bool i_AssIncomClientWithContext(SOCKET sClientSocket);

	/// ==============================================
	/// @par ���� 
	/// ���������ӵ��׽���
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	[in]		sSocket			Ҫ���õ��׽���
	/// @return 	-
	/// @note 		by li.xl 2013/06/17 
	/// ==============================================
	bool i_SetNewSocketOption(SOCKET sSocket);

	/// ==============================================
	/// @par ���� 
	/// Ͷ��UDP����������ɶ˿�
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	[in]		usNum			Ͷ�ݽ��յ�IO����
	/// @return 	-
	/// @note 		by li.xl 2013/06/17 
	/// ==============================================
	bool i_PostUDPRecvIssue(USHORT usNum);
	
	/// ==============================================
	/// @par ���� 
	/// Ͷ��UDP����������ɶ˿�
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	[in]		pNetBuffer		�������
	/// @return 	-
	/// @note 		by li.xl 2013/06/21 
	/// ==============================================
	bool i_PostUDPRecvIssue(CNetBuffer* pNetBuffer = NULL);

	/// ==============================================
	/// @par ���� 
	/// Ͷ��UDP����������ɶ˿�
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	[in]		pszClientIP		�ͻ��˶Ͽ�
	/// [in]		usClientPort	�ͻ��˶˿ںŴ���
	/// [in]		pSendBuffer		�������ݻ���
	/// @return 	-
	/// @note 		by li.xl 2013/06/17 
	/// ==============================================
	bool i_PostUDPSendIssue(const char* pszClientIP, USHORT usClientPort, CNetBuffer* pSendBuffer);

	/// ==============================================
	/// @par ���� 
	/// ����IO��Ϣ����
	/// @param 
	/// [in,out]	<������>		<����˵��>
	/// [in]		pClientContext	������Ϣ
	///	[in]		psttIOHand		IO�ص������Ϣ
	/// [in]		nDataLen		���ݳ���
	/// @return 	-
	/// @note 		by li.xl 2013/06/17 
	/// ==============================================
	bool i_ProcessIOMessage(T_ClientContext* pClientContext, CNetBuffer* pNetBuffer, int nDataLen);

	/// ==============================================
	/// @par ���� 
	/// ����TCP�������¼�����
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	[in]		pClientContext	������Ϣ
	/// [in]		pNetBuffer		�������
	/// [in]		nDataLen		���ݴ�С
	/// @return 	-
	/// @note 		by li.xl 2013/06/20 
	/// ==============================================
	void i_OnTCPAccept(T_ClientContext* pClientContext, CNetBuffer* pNetBuffer, int nDataLen);

	/// ==============================================
	/// @par ���� 
	/// ����TCP׼�������¼�
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	[in]		pClientContext	������Ϣ
	/// [in]		pNetBuffer		�������
	/// [in]		nDataLen		���ݴ�С
	/// @return 	-
	/// @note 		by li.xl 2013/06/20 
	/// ==============================================
	void i_OnTCPRecv(T_ClientContext* pClientContext, CNetBuffer* pNetBuffer);

	/// ==============================================
	/// @par ���� 
	/// ����TCP��������¼�
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	[in]		pClientContext	������Ϣ
	/// [in]		pNetBuffer		�������
	/// [in]		nDataLen		���ݴ�С
	/// @return 	-
	/// @note 		by li.xl 2013/06/20 
	/// ==============================================
	void i_OnTCPRecved(T_ClientContext* pClientContext, CNetBuffer* pNetBuffer, int nDataLen);

	/// ==============================================
	/// @par ���� 
	/// ����TCP׼�������¼�
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	[in]		pClientContext	������Ϣ
	/// [in]		pNetBuffer		�������
	/// [in]		nDataLen		���ݴ�С
	/// @return 	-
	/// @note 		by li.xl 2013/06/20 
	/// ==============================================
	void i_OnTCPSend(T_ClientContext* pClientContext, CNetBuffer* pNetBuffer, int nDataLen);

	/// ==============================================
	/// @par ���� 
	/// ����TCP��������¼�
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	[in]		pClientContext	������Ϣ
	/// [in]		pNetBuffer		�������
	/// [in]		nDataLen		���ݴ�С
	/// @return 	-
	/// @note 		by li.xl 2013/06/20 
	/// ==============================================
	void i_OnTCPSended(T_ClientContext* pClientContext, CNetBuffer* pNetBuffer, int nDataLen);

	/// ==============================================
	/// @par ���� 
	/// ����UDP׼�������¼�
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	[in]		pClientContext	������Ϣ
	/// [in]		pNetBuffer		�������
	/// [in]		nDataLen		���ݴ�С
	/// @return 	-
	/// @note 		by li.xl 2013/06/20 
	/// ==============================================
	void i_OnUDPRecv(T_ClientContext* pClientContext, CNetBuffer* pNetBuffer, int nDataLen);

	/// ==============================================
	/// @par ���� 
	/// ����UDP׼�������¼�
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	[in]		pClientContext	������Ϣ
	/// [in]		pNetBuffer		�������
	/// [in]		nDataLen		���ݴ�С
	/// @return 	-
	/// @note 		by li.xl 2013/06/20 
	/// ==============================================
	void i_OnUDPSend(T_ClientContext* pClientContext, CNetBuffer* pNetBuffer, int nDataLen);

	/// ==============================================
	/// @par ���� 
	/// ����һ��������Ϣ����ɶ˿�
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	[in]		pClientContext	������Ϣ
	/// [in]		usIOType		IO��������
	/// [in]		pNetBuffer		�������
	/// @return 	-
	/// @note 		by li.xl 2013/06/20 
	/// ==============================================
	bool i_AIOProcess(T_ClientContext* pClientContext, USHORT usIOType, CNetBuffer* pNetBuffer = NULL);

	/// ==============================================
	/// @par ���� 
	/// ʹIO˳���ȡ����
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	[in]		pClientContext	������Ϣ
	/// [in]		pNetBuffer		�������
	/// @return 	-
	/// @note 		by li.xl 2013/06/20 
	/// ==============================================
	void i_MakeOrderdRead(T_ClientContext* pClientContext, CNetBuffer*& pNetBuffer);

	/// ==============================================
	/// @par ���� 
	/// ��˳���ȡ������
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	[in]		pClientContext	������Ϣ
	/// [in]		pNetBuffer		�������
	/// @return 	CNetBuffer		����Ҫ����Ļ������
	/// @note 		by li.xl 2013/06/20 
	/// ==============================================
	CNetBuffer* i_GetNextReadBuffer(T_ClientContext* pContextStruct, CNetBuffer* pNetBuffer = NULL);

	/// ==============================================
	/// @par ���� 
	/// ��˳���ȡд����
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	[in]		pClientContext	������Ϣ
	/// [in]		pNetBuffer		�������
	/// @return 	CNetBuffer		����Ҫ����Ļ������
	/// @return 	-
	/// @note 		by li.xl 2013/06/21 
	/// ==============================================
	CNetBuffer* i_GetNextSendBuffer(T_ClientContext* pContextStruct, CNetBuffer* pNetBuffer = NULL);

	/// ==============================================
	/// @par ���� 
	/// ������հ����
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	[in]		pClientContext	������Ϣ
	/// [in]		pNetBuffer		�������
	/// @return 	-
	/// @note 		by li.xl 2013/06/20 
	/// ==============================================
	void i_ProcessRecvPackage(T_ClientContext*& pContextStruct, CNetBuffer*& pNetBuffer);

	/// ==============================================
	/// @par ���� 
	/// ����ǰһ��δ�����������
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	[in]		pClientContext	������Ϣ
	/// [in]		pNetBuffer		�������
	/// @return 	bool			false == ����δ������ɵ�����, true == �Ѿ��������
	/// @note 		by li.xl 2013/06/21 
	/// ==============================================
	bool i_ProcessPreviouRecv(T_ClientContext*& pContextStruct, CNetBuffer*& pNetBuffer);

	/// ==============================================
	/// @par ���� 
	/// ����ǰ���յ�����
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	[in]		pClientContext	������Ϣ
	/// [in]		pNetBuffer		�������
	/// @return 	bool			false == ����δ������ɵ�����, true == �Ѿ��������
	/// @note 		by li.xl 2013/06/21 
	/// ==============================================
	void i_ProcessCurrentRecv(T_ClientContext*& pContextStruct, CNetBuffer*& pNetBuffer);

protected:

	/// ==============================================
	/// @par ���� 
	/// ֪ͨ�ͻ��������ӵ���
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	[in]		pClientContext	�ͻ���������Ϣ
	/// @return 	-
	/// @note 		by li.xl 2013/06/19 
	/// ==============================================
	bool e_NotifyClientConntext(T_ClientContext* pClientContext);

	/// ==============================================
	/// @par ���� 
	/// ֪ͨ�ͻ��˶Ͽ�����
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	[in]		pClientContext	�ͻ���������Ϣ
	/// @return 	-
	/// @note 		by li.xl 2013/06/19 
	/// ==============================================
	bool e_DisConectionConntext(T_ClientContext*& pClientContext);

	/// ==============================================
	/// @par ���� 
	/// ֪ͨ���յ����ݰ�
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	[in]		pContextStruct	���յĵ�����
	/// [in]		pNetBuffer		�������
	/// [in]		unDataSize		���ݴ�С
	/// @return 	-
	/// @note 		by li.xl 2013/06/21 
	/// ==============================================
	void e_NotifyReceivedPackage(USHORT usNetType, T_ClientContext* pContextStruct, CNetBuffer* pNetBuffer);

	/// ==============================================
	/// @par ���� 
	/// ֪ͨ�������ݰ����
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	[in]		usNetType		���ͺͽ������ݵ��������� �μ�_eNetTransfType
	/// [in]		ulContextID		����TCP���Ӵ˲�������0��UDP���Ӵ˲���Ϊ0
	/// [in]		pszClientIP		�ͻ���IP
	/// [in]		usClientPort	�ͻ��˶˿ں�
	/// [in]		sDataType		��������
	/// [in]		nOBJType		����/�ṹ����
	/// [in]		sOBJCount		����/�ṹ����
	/// [in]		sSNum			����/�ṹ���鿪ʼ����(�����б���ʱ��sOBJCount���ʹ��)
	/// [in]		sENum			����/�ṹ�����������(�����б�������sOBJCount���ʹ��)
	/// [in]		nDatalen		���ݳ���
	/// [in]		pData			��������
	/// @return 	-
	/// @note 		by li.xl 2013/06/21 
	/// ==============================================
	void e_NotifyWriteCompleted(USHORT usNetType, ULONG ulContextID, const char* pszClientIP,
		USHORT usClientPort, SHORT sDataType, int nOBJType, SHORT sOBJCount,
		SHORT sSNum, SHORT sENum, int nDatalen, void* pData);

	/// ==============================================
	/// @par ���� 
	/// ��ȡIP�Ͷ˿���Ϣ
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	[in]		sClientSocket	�ͻ����׽���
	/// [in]		pszClientIP		�ͻ���IP
	/// [in]		usStrSize		�ַ�������
	/// [in]		usClientPort	�ͻ��˶˿�
	/// @return 	-
	/// @note 		by li.xl 2013/06/20 
	/// ==============================================
	bool e_GetIPAddrBySocket(SOCKET sClientSocket, char* pszClientIP, USHORT usStrSize, USHORT& usClientPort);

	/// ==============================================
	/// @par ���� 
	/// �ͷ��׽���
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	[in]		sSocket			Ҫ�ͷŵ��׽���
	/// @return 	-
	/// @note 		by li.xl 2013/06/18 
	/// ==============================================
	void e_CloseSocket(SOCKET& sSocket);
public:

	/// ==============================================
	/// @par ���� 
	/// �������ݵ�TCP�ͻ���
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	[in]		ulContextID		������ϢID
	/// [in]		sttBufferHead	TCP����ͷ
	/// [in]		pSendData		���͵�����
	/// [in]		nDataLen		���ݳ���
	/// @return 	-
	/// @note 		by li.xl 2013/06/21 
	/// ==============================================
	bool e_SendDataToTCPClient(ULONG ulContextID, T_TCPBufferHead& sttBufferHead,
		BYTE* pSendData, int nDataLen);

	/// ==============================================
	/// @par ���� 
	/// �������ݵ�UDP�ͻ���
	/// @param 
	/// [in,out]	<������>		<����˵��>
	/// [in]		pszClientIP		�ͻ���IP
	///	[in]		usClientPort	�ͻ��˶˿ں�
	/// [in]		sttBufferHead	UDP����ͷ
	/// [in]		pSendData		���͵�����
	/// [in]		nDataLen		���ݳ���
	/// @return 	-
	/// @note 		by li.xl 2013/06/22 
	/// ==============================================
	bool e_SendDataToUDPClient(const char* pszClientIP, USHORT usClientPort,
		T_UDPBufferHead& sttBufferHead, BYTE* pSendData, int nDataLen);

	/// ==============================================
	/// @par ���� 
	/// �ر�TCP����
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	[in]		ulContextID		������ϢID	
	/// @return 	-
	/// @note 		by li.xl 2013/06/28 
	/// ==============================================
	bool e_CloseTCPContext(ULONG ulContextID);

	/// ==============================================
	/// @par ���� 
	/// ��ӡ��־
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	[in]		pszLog			Ҫ��ӡ����־��Ϣ
	/// @return 	-
	/// @note 		by li.xl 2013/07/23 
	/// ==============================================
	void e_PrintLog(const char* pszLog, ...);

private:
	/// ��������״̬
	bool					m_bNetRunStatus;
	/// TCP��������߳̾��
	HANDLE					m_hListenThread;
	/// TCP��������׽���
	SOCKET					m_sListenTCPSocket;
	/// TCP�����׽����¼����
	HANDLE					m_hPostAcceptEvent;
	/// IOCP��ɶ˿�ʵ�����
	HANDLE					m_hCompletionPort;
	/// ��������ʼ����Ϣ�ṹ
	T_NetInitStruct			m_sttNetInitStruct;
	/// �����߳̾������
	HANDLE					m_hWorkThread[MAX_PROCESSER_NUMBERS];
	/// ������Ϣ�б�������
	CContextList			m_ContextManager;
	/// �������ݹ������
	CBufferPool				m_CBufferManager;
	/// UDP�����׽���
	SOCKET					m_sUDPServerSocket;
	/// UDP�����ʹ�õ�����ϵ����
	T_ClientContext*		m_pUDPSeverContext;
	/// �������ݻص�����
	OnRecvDataCallBack		m_pFunRecvData;
	/// �������ݻص�����
	OnSendDataCallBack		m_pFunSendData;
	/// �ͻ���TCP�ɹ����ӻص�����
	OnConectionCallBack		m_pFunConection;
	/// �ͻ���TCP�Ͽ����ӻص�����
	OnDisConectionCallBack	m_pFunDisConection;
	/// ��ӡ��־�ص�����
	OnPrintLog				m_pFunPrintLog;
};

#endif	/// __THE_LEE_NET_IOCP_H_H__