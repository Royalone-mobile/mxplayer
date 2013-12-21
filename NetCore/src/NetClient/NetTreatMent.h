////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 
/// @par ģ����
/// �ͻ�������ͨѶҵ����
/// @par ����ļ�
/// NetTreatment.cpp
/// @par ������ϸ����
/// @par ���̰߳�ȫ��
/// [��˵��]
/// @par �쳣ʱ��ȫ��
/// [��˵��]
/// @note         -
/// @file         NetTreatMent.h
/// @brief        -
/// @author       Li.xl
/// @version      1.0
/// @date         2013/12/20
/// @todo         -
/// 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __FWAYNET_NETCLIENT_NETTREATMENT_H__
#define __FWAYNET_NETCLIENT_NETTREATMENT_H__

#include "NetBase.h"

class CNetTreatment: public CNetBase
{
public:
	CNetTreatment();
	~CNetTreatment();

public:
	/// ==============================================
	/// @par ���� 
	/// ֹͣTCP�����߳�
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	-
	/// @return 	-
	/// @note 		Creat By li.xl 2013/12/21 
	/// ==============================================
	void e_StopTCPProcess();

	/// ==============================================
	/// @par ���� 
	/// ֹͣUDP�����߳�
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	-
	/// @return 	-
	/// @note 		Creat By li.xl 2013/12/21 
	/// ==============================================
	void e_StopUDPProcess();

	/// ==============================================
	/// @par ���� 
	/// �������ݵ�TCP�ͻ���
	/// @param 
	/// [in,out]	<������>		<����˵��>
	/// [in]		sttBufferHead	TCP����ͷ
	/// [in]		pSendData		���͵�����
	/// [in]		nDataLen		���ݳ���
	/// @return 	-
	/// @note 		Creat By li.xl 2013/12/21
	/// ==============================================
	bool e_CreatSendTCPData(T_TCPBufferHead& sttBufferHead,
		BYTE* pSendData, int nDataLen);

	/// ==============================================
	/// @par ���� 
	/// �������ݵ�UDP�ͻ���
	/// @param 
	/// [in,out]	<������>		<����˵��>
	/// [in]		pszRemoteIP		Զ��IP
	///	[in]		usRemotePort	Զ�̶˿ں�
	/// [in]		sttBufferHead	UDP����ͷ
	/// [in]		pSendData		���͵�����
	/// [in]		nDataLen		���ݳ���
	/// @return 	-
	/// @note 		Creat By li.xl 2013/12/21
	/// ==============================================
	bool e_CreatSendUDPData(const char* pszRemoteIP, USHORT usRemotePort,
		T_UDPBufferHead& sttBufferHead, BYTE* pSendData, int nDataLen);
protected:
	/// ==============================================
	/// @par ���� 
	/// �����̴߳���
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	
	/// @return 	-
	/// @note 		Creat By li.xl 2013/12/20 
	/// ==============================================
	virtual void e_SendTCPLoop();

	/// ==============================================
	/// @par ���� 
	/// �����̴߳���
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	
	/// @return 	-
	/// @note 		Creat By li.xl 2013/12/20 
	/// ==============================================
	virtual void e_ReceiveTCPLoop();

	/// ==============================================
	/// @par ���� 
	/// �����̴߳���
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	-
	/// @return 	-
	/// @note 		Creat By li.xl 2013/12/20 
	/// ==============================================
	virtual void e_SendUDPLoop();

	/// ==============================================
	/// @par ���� 
	/// �����̴߳���
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	-
	/// @return 	-
	/// @note 		Creat By li.xl 2013/12/20 
	/// ==============================================
	virtual void e_ReceiveUDPLoop();

private:
	/// ==============================================
	/// @par ���� 
	/// 
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	
	/// @return 	-
	/// @note 		Creat By li.xl 2013/12/21 
	/// ==============================================
	void i_ProcessTCPRecData();

	/// ==============================================
	/// @par ���� 
	/// 
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	
	/// @return 	-
	/// @note 		Creat By li.xl 2013/12/21 
	/// ==============================================
	void i_ProcessUDPRecData();

	/// ==============================================
	/// @par ���� 
	/// ������յ��Ľṹ������
	/// @param 
	/// [in,out]	<������>		<����˵��>
	///	[in]		usNetType		���ͺͽ������ݵ��������� �μ�_eNetTransfType
	/// [in]		pszClientIP		�ͻ���IP
	/// [in]		usClientPort	�ͻ��˶˿ں�
	/// [in]		sDataType		��������
	/// [in]		nOBJType		����/�ṹ����
	/// [in]		sOBJCount		����/�ṹ����
	/// [in]		sSNum			����/�ṹ���鿪ʼ����(�����б���ʱ��sOBJCount���ʹ��)
	/// [in]		sENum			����/�ṹ�����������(�����б�������sOBJCount���ʹ��)
	/// [in]		nDatalen		���ݳ���
	/// @return 	-
	/// @note 		Creat By li.xl 2013/12/21 
	/// ==============================================
	bool i_ProcessReceivePackage(USHORT usNetType, const char* pszClientIP, 
		USHORT usClientPort, SHORT sDataType, int nOBJType, SHORT sOBJCount, 
		SHORT sSNum, SHORT sENum, int nDatalen, void* pData);

private:
	/// �������ݻ���
	char  m_szTCPRecvBuffer[MAX_NET_BUFFER_SIZE + EXTRA_BUFFER_SIZE];
	char  m_szUDPRecvBuffer[MAX_NET_BUFFER_SIZE + EXTRA_BUFFER_SIZE];
};

#endif /// __FWAYNET_NETCLIENT_NETTREATMENT_H__